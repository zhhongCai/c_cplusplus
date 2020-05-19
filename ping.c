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

/**
 * https://www.cnblogs.com/skyfsm/p/6348040.html 
 * https://github.com/alex-justes/ping/blob/master/ping.c
 */

#define PACKET_SEND_MAX_NUM 64

typedef struct ping_packet_status {
	// 发送标志,1为已发送
	int flag;
	// 包的序列号
	int seq;
	struct timeval begin_time;
	struct timeval end_time;
} ping_packet_status;

ping_packet_status ping_packet[PACKET_SEND_MAX_NUM];


int alive;
int rawsock;
int send_count;
int recv_count;
pid_t pid;
struct sockaddr_in dest;
struct timeval start_time;
struct timeval end_time;
struct timeval time_interval;

unsigned short cal_check_sum(unsigned short *datap, int len) 
{
	uint32_t sum = 0;

    while(len > 1) 
	{
        sum += *datap;
        datap++;
        len -= 2;
    }
    if(len)
        sum += (*(uint8_t *)datap);

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum = (sum >> 16) + sum;

    return ~sum;
}


int icmp_pack(struct icmp *icmp_head_data, int seq) 
{
	int i = 0;
	icmp_head_data->icmp_type = ICMP_ECHO;
	icmp_head_data->icmp_code = 0;
	icmp_head_data->icmp_cksum = 0;
	icmp_head_data->icmp_seq = seq;
	icmp_head_data->icmp_id = pid & 0xffff;

	struct timeval *tval = (struct timeval *)icmp_head_data->icmp_data;
	gettimeofday(tval, NULL);

	int len = sizeof(icmp_head_data) + sizeof(tval);
	icmp_head_data->icmp_cksum = cal_check_sum((unsigned short *) icmp_head_data, len);

	return len;
}

struct timeval cal_time_offset(struct timeval begin_time, struct timeval recv_time)
{
	struct timeval ans;
	ans.tv_sec = recv_time.tv_sec - begin_time.tv_sec;
	ans.tv_usec = recv_time.tv_usec - begin_time.tv_usec;

	if (ans.tv_usec < 0) 
	{
		ans.tv_sec--;
		ans.tv_sec += 1000000;
	}
	return ans;
}

int icmp_unpack(char *buf, int len) 
{
	int iphdr_len;
	struct timeval begin_time, recv_time, offset_time;
	int rtt;

	struct ip *ip_hdr = (struct ip *)buf;
	iphdr_len = ip_hdr->ip_hl * 4;
	struct icmp *icmp_hdr = (struct icmp *) (buf + iphdr_len);
	// icmp包长度
	len -= iphdr_len; 
	if (len < 8) 
	{
		fprintf(stderr, "INvalid icmp packet.Its length is less than 8\n");
		return -1;
	}

	if ((icmp_hdr->icmp_type == ICMP_ECHOREPLY) && (icmp_hdr->icmp_id == (pid & 0xffff)))
	{
		if ((icmp_hdr->icmp_seq < 0) || (icmp_hdr->icmp_seq > PACKET_SEND_MAX_NUM)) 
		{
			fprintf(stderr, "icmp packet seq is out of range!\n");
			return -1;
		}

		ping_packet[icmp_hdr->icmp_seq].flag = 0;
		begin_time = ping_packet[icmp_hdr->icmp_seq].begin_time;
		gettimeofday(&recv_time, NULL);
		offset_time = cal_time_offset(begin_time, recv_time);
		rtt = offset_time.tv_sec * 1000 + offset_time.tv_usec / 1000;

		printf("%d byte from %s: icmp_seq=%u ttl=%d rtt=%d ms\n",
			len, inet_ntoa(ip_hdr->ip_src), icmp_hdr->icmp_seq, ip_hdr->ip_ttl, rtt);
	} else {
		fprintf(stderr, "Invalid ICMP packet! Tts id is not matched!\n");
		return -1;
	}
	return 0;
}

void ping_send() 
{
	char send_buf[128];
	memset(send_buf, 0, sizeof(send_buf));
	gettimeofday(&start_time, NULL);
	while (alive) 
	{
		int size = 0;
		gettimeofday(&(ping_packet[send_count].begin_time), NULL);
		ping_packet[send_count].flag = 1;

		int pack_size = icmp_pack((struct icmp *) send_buf, send_count);
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

	while (alive) 
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
				ret = icmp_unpack(recv_buf, size);
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
	time_interval = cal_time_offset(start_time, end_time);

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
	int size = 128 * 1024;
	struct protoent* protocol = NULL;
	char dest_addr_str[80];
	memset(dest_addr_str, 0, 80);
	unsigned int inaddr = 1;
	struct hostent* host = NULL;

	pthread_t send_id, recv_id;

	if (argc < 2) {
		printf("Invalid IP address!\n");
		return -1;
	}

	protocol = getprotobyname("icmp");
	if (protocol == NULL) {
		printf("Fail to getprotocolbyname!\n");
		return -1;
	}
	
	memcpy(dest_addr_str, argv[1], strlen(argv[1]) + 1);

	rawsock = socket(AF_INET, SOCK_RAW, protocol->p_proto);
	if (rawsock < 0) 
	{
		printf("Fail to create socket!\n");
		return -1;
	}	

	pid = getpid();

	setsockopt(rawsock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	bzero(&dest, sizeof(dest));

	dest.sin_family = AF_INET;

	inaddr = inet_addr(argv[1]);

	if (inaddr == INADDR_NONE)
	{
		host = gethostbyname(argv[1]);
		if (host == NULL) 
		{
			printf("Fail to gethostbyname!\n");
			return -1;
		}

		memcpy((char *) &dest.sin_addr, host->h_addr, host->h_length);
	} else {
		memcpy((char *) &dest.sin_addr, &inaddr, sizeof(inaddr));
	}

	inaddr = dest.sin_addr.s_addr;
	printf("PING %s, (%d.%d.%d.%d) 56(84) bytes of data.\n",dest_addr_str,
		(inaddr&0x000000ff), (inaddr&0x0000ff00)>>8, 
		(inaddr&0x00ff0000)>>16, (inaddr&0xff000000)>>24);

	alive = 1;

	signal(SIGINT, icmp_sigint);

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