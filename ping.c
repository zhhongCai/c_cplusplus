#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include "my_icmp.h"

/**
 * https://www.cnblogs.com/skyfsm/p/6348040.html 
 * https://github.com/alex-justes/ping/blob/master/ping.c
 */

ping_packet_status ping_packet[PACKET_SEND_MAX_NUM];

int alive;
int rawsock;
int send_count = 0;
int recv_count = 0;
pid_t pid;
struct sockaddr_in dest;
struct timeval start_time;
struct timeval end_time;
struct timeval time_interval;

void ping_send() 
{
	char send_buf[128];
	memset(send_buf, 0, sizeof(send_buf));
	gettimeofday(&start_time, NULL);
	while (alive && send_count < PACKET_SEND_MAX_NUM) 
	{
		int size = 0;
		gettimeofday(&(ping_packet[send_count].begin_time), NULL);
		ping_packet[send_count].flag = 1;
		ping_packet[send_count].seq = send_count;

		int pack_size = icmp_pack((struct icmp *) send_buf, send_count, pid & 0xffff);
		size = sendto(rawsock, send_buf, pack_size, 0, (struct sockaddr *) &dest, sizeof(dest));
		send_count++;
		if (size < 0) 
		{
			fprintf(stderr, "send icmp packet fail!\n");
			continue;
		}

		sleep(1);
	}	
}

void ping_recv() 
{
	struct timeval tv;
	tv.tv_usec = 200;
	tv.tv_sec = 0;
	fd_set read_fd;
	char recv_buf[512];
	memset(recv_buf, 0, sizeof(recv_buf));

	while (alive && recv_count < PACKET_SEND_MAX_NUM) 
	{
		int ret = 0;
		FD_ZERO(&read_fd);
		FD_SET(rawsock, &read_fd);	
		ret = select(rawsock + 1, &read_fd, NULL, NULL, &tv);
		switch (ret)
		{
		case -1:
			fprintf(stderr, "fail to select!\n");
			break;
		case 0:
			break;
		default:
			{
				int size = recv(rawsock, recv_buf, sizeof(recv_buf), 0);
				if (size < 0) 
				{
					fprintf(stderr, "recv data fail!\n");
					continue;
				}
				ret = icmp_unpack(recv_buf, size, pid & 0xffff, &ping_packet[recv_count]);
				if (ret == -1) 
				{
					continue;
				}
				recv_count++;
			}
			break;
		}
	}
}

void icmp_sigint(int signo) 
{
	alive = 0;
	gettimeofday(&end_time, NULL);
	time_interval = time_offset(start_time, end_time);

}

void ping_stats_show() 
{
	long time = time_interval.tv_sec * 1000 + time_interval.tv_usec / 1000;
	 /*注意除数不能为零，这里send_count有可能为零，所以运行时提示错误*/
	printf("%d packets transmitted, %d recieved, %d%c packet loss, time %ldms\n",
		send_count, recv_count, (send_count-recv_count)*100/send_count, '%', time);
}



int main(int argc, char* argv[]) 
{
	if (argc < 2) 
	{
		printf("Invalid IP address!\n");
		return -1;
	}
	
	rawsock = raw_sock();
	if (rawsock < 0) 
	{
		return -1;
	}

	pid = getpid();

	int size = 128 * 1024;
	setsockopt(rawsock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	unsigned int inaddr = set_dest(argv[1], &dest);
	if (inaddr < 0) 
	{
		return -1;
	}

	printf("PING %s, (%d.%d.%d.%d) 8 bytes of data.\n", argv[1],
		(inaddr&0x000000ff), (inaddr&0x0000ff00)>>8, 
		(inaddr&0x00ff0000)>>16, (inaddr&0xff000000)>>24);

	alive = 1;

	signal(SIGINT, icmp_sigint);

	pthread_t send_id, recv_id;
	if (pthread_create(&send_id, NULL, (void*) ping_send, NULL))
	{
		printf("Fail to create ping send thread!\n");
		return -1;
	}
	if (pthread_create(&recv_id, NULL, (void*) ping_recv, NULL))
	{
		printf("Fail to create ping recv thread!\n");
		return -1;
	}

	pthread_join(send_id, NULL);
	pthread_join(recv_id, NULL);

	ping_stats_show();

	close(rawsock);

	return 0;
}