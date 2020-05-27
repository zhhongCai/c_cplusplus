#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "my_icmp.h"

/**
 * icmp echo报文:
 * 发送icmp ttl=1,返回icmp 超时,确定第一跳路由 
 * 发送icmp ttl=2,返回icmp 超时,确定第二跳路由 
 * 发送icmp ttl=k,返回icmp 超时,确定第k跳路由
 * 发送icmp ttl=k+1,返回icmp 端口不可达,到达目标主机，结束
 */

int datalen;
u_short ident;
u_short port = 32768 + 666;
int rawsock;
struct sockaddr_in *dest;
struct sockaddr whereto;

void send_probe(int seq, int ttl) 
{
	char send_buf[128];
	memset(send_buf, 0, sizeof(send_buf));
	int size = 0;

	setsockopt(rawsock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
	
	int pack_size = icmp_pack((struct icmp *) send_buf, ttl, seq & 0xffff);
	size = sendto(rawsock, send_buf, pack_size, 0, (struct sockaddr *) &dest, sizeof(dest));
	if (size < 0) 
	{
		fprintf(stderr, "send icmp packet fail!\n");
	}

	printf("\ntraceroute: wrote to %s:  %d chars, ret=%d\n", inet_ntoa(dest->sin_addr), pack_size, size);
}

int icmp_packet_ok(char *buf, int len, u_char seq) 
{
	int iphdr_len;
	struct timeval begin_time, recv_time, offset_time;
	int rtt;

	struct ip *ip_hdr = (struct ip *)buf;
	iphdr_len = ip_hdr->ip_hl << 2;
	struct icmp *icmp_hdr = (struct icmp *) (buf + iphdr_len);
	// icmp包长度
	len -= iphdr_len; 
	if (len < 8) 
	{
		fprintf(stderr, "INvalid icmp packet.Its length is less than 8\n");
		return -1;
	}
	// printf("type=%d, code=%d\n", icmp_hdr->icmp_type, icmp_hdr->icmp_code);

	if ((icmp_hdr->icmp_type == ICMP_TIMXCEED && icmp_hdr->icmp_code == ICMP_TIMXCEED_INTRANS) ||
	    icmp_hdr->icmp_type == ICMP_UNREACH) {
		struct ip *hip;
		struct udphdr *up;

		return icmp_hdr->icmp_code + 1;
	}
	return 0;
}

void
print(buf, cc, from)
	char *buf;
	int cc;
	struct sockaddr_in *from;
{
	struct ip *ip;
	int hlen;

	ip = (struct ip *) buf;
	hlen = ip->ip_hl << 2;
	cc -= hlen;

	printf(" %s", inet_ntoa(from->sin_addr));
}


double
deltaT(t1p, t2p)
	struct timeval *t1p, *t2p;
{
	register double dt;

	dt = (double)(t2p->tv_sec - t1p->tv_sec) * 1000.0 +
	     (double)(t2p->tv_usec - t1p->tv_usec) / 1000.0;
	return (dt);
}

int main(int argc, char* argv[]) 
{
	if (argc < 2) {
		printf("Invalid IP address!\n");
		return -1;
	}

	(void) bzero((char *)&whereto, sizeof(struct sockaddr));

	dest = (struct sockaddr_in *)&whereto;
	unsigned int inaddr = set_dest(argv[1], dest);
	if (inaddr < 0) 
	{
		return -1;
	}

	char recv_buf[512];
	memset(recv_buf, 0, sizeof(recv_buf));
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	struct sockaddr_in from;
	ident = (getpid() & 0xffff) | 0x8000;
	u_char seq = 1;	
	int got_there = 0;
	int unreachable = 0;

	ping_packet_status ping_packet[64];

	

	struct protoent *pe;
	if ((pe = getprotobyname("icmp")) == NULL) {
		fprintf(stderr, "icmp: unknown protocol\n");
		exit(10);
	}

	rawsock = socket(AF_INET, SOCK_RAW, pe->p_proto);
	if (rawsock < 0) 
	{
		printf("Create raw socket fail!\n");
		return -1;
	}

	for (int ttl = 1; ttl < 128; ttl++, seq++) 
	{
		u_long lastaddr = 0;

		for (int probe = 0; probe < 3; ++probe) {
			struct timeval t1, t2;
			(void) gettimeofday(&t1, NULL);
			send_probe(seq, ttl);

			fd_set read_fd;
			FD_ZERO(&read_fd);
			FD_SET(rawsock, &read_fd);
			int ret = select(rawsock + 1, &read_fd, (fd_set *)0, (fd_set *)0, &tv);
			switch (ret)
			{
			case -1:
				fprintf(stderr, "fail to select!\n");
				break;
			case 0:
				// printf("\n * \n", ret);
				break;
			default:
			{
				__uint32_t fromlen = sizeof(from);
				int len = recvfrom(rawsock, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&from, &fromlen);
				int i = 0;
				(void) gettimeofday(&t2, NULL);
				if ((i = icmp_packet_ok(recv_buf, len, seq))) {
					if (from.sin_addr.s_addr != lastaddr) {
						print(recv_buf, len, &from);
						lastaddr = from.sin_addr.s_addr;
					}
					printf("  %g ms", deltaT(&t1, &t2));
					switch(i - 1) {
					case ICMP_UNREACH_PORT:
						++got_there;
						break;
					case ICMP_UNREACH_NET:
						++unreachable;
						printf(" !N");
						break;
					case ICMP_UNREACH_HOST:
						++unreachable;
						printf(" !H");
						break;
					case ICMP_UNREACH_PROTOCOL:
						++got_there;
						printf(" !P");
						break;
					case ICMP_UNREACH_NEEDFRAG:
						++unreachable;
						printf(" !F");
						break;
					case ICMP_UNREACH_SRCFAIL:
						++unreachable;
						printf(" !S");
						break;
					}
					break;
				}
			}	
			if (got_there || unreachable >= 64) {}
				printf("got_there=%d, uncreachable=%d", got_there, unreachable);
				exit(0);	
			}
		}
		sleep(1);
		printf(" \n");
	}
	
	return 0;
}