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

int main(int argc, char* argv[]) 
{
	if (argc < 2) {
		printf("Invalid IP address!\n");
		return -1;
	}

	struct sockaddr_in dest;
	unsigned int inaddr = set_dest(argv[1], &dest);
	if (inaddr < 0) 
	{
		return -1;
	}


	char send_buf[128];
	memset(send_buf, 0, sizeof(send_buf));
	char recv_buf[512];
	memset(recv_buf, 0, sizeof(recv_buf));
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 200000;

	ping_packet_status ping_packet[64];

	for (int ttl = 1; ttl < 64; ttl++) 
	{
		int rawsock = raw_sock();
		if (rawsock < 0) 
		{
			printf("Create raw socket fail!\n");
			return -1;
		}

		setsockopt(rawsock, SOL_SOCKET, IP_TTL, &ttl, sizeof(ttl));

		gettimeofday(&(ping_packet[ttl].begin_time), NULL);
		ping_packet[ttl].flag = 1;
		ping_packet[ttl].seq = 1;
		int pack_size = icmp_pack((struct icmp *) send_buf, 1, ttl);
		int size = sendto(rawsock, send_buf, pack_size, 0, (struct sockaddr *) &dest, sizeof(dest));
		if (size < 0) 
		{
			fprintf(stderr, "send icmp packet fail!\n");
			continue;
		}
		printf("sendto return size =%d\n", size);

		fd_set read_fd;
		FD_ZERO(&read_fd);
		FD_SET(rawsock, &read_fd);
		int ret = select(rawsock + 1, &read_fd, NULL, NULL, &tv);
		switch (ret)
		{
		case -1:
			fprintf(stderr, "fail to select!\n");
			break;
		case 0:
			break;
		default:
			{
				int recv_size = recv(rawsock, recv_buf, sizeof(recv_buf), 0);
				if (size < 0) {
					fprintf(stderr, "recv data fail!\n");
					continue;
				}
				ret = icmp_unpack(recv_buf, recv_size, ttl, &ping_packet[ttl]);
				if (ret == -1) 
				{
					continue;
				}
			}
			break;
		}		
	}
	
	return 0;
}