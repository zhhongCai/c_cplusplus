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
 * 发送udp (随机不用的端口)+返回icmp实现:
 * 发送udp ttl=1,返回icmp 超时,确定第一跳路由 
 * 发送udp ttl=2,返回icmp 超时,确定第二跳路由 
 * 发送upd ttl=k,返回icmp 超时,确定第k跳路由
 * 发送udp ttl=k+1,返回icmp 端口不可达,到达目标主机，结束
 */
struct opacket {
	struct ip ip;
	struct udphdr udp;
	u_char seq;
	u_char ttl;
	struct timeval tv;
}; 

struct opacket *outpacket;
int datalen;
u_short ident;
u_short port = 32768 + 666;
int rawsock;
struct sockaddr_in *dest;
struct sockaddr whereto;

void send_probe(int seq, int ttl) 
{
	struct opacket *op = outpacket; 
	struct ip *ip = &op->ip;
	struct udphdr *udp = &op->udp;
	int i;

	ip->ip_off = 0;
	ip->ip_sum = 0;
	ip->ip_hl = sizeof(*ip) >> 2;
	ip->ip_p = IPPROTO_UDP;
	ip->ip_len = datalen;
	ip->ip_ttl = ttl;
	ip->ip_v = IPVERSION;
	ip->ip_id = htons(ident + seq);

	udp->uh_sport = htons(ident);
	udp->uh_dport = htons(port + seq);
	udp->uh_ulen = htons((u_short)(datalen - sizeof(struct ip)));
	udp->uh_sum = 0;

	op->seq = seq;
	op->ttl = ttl;
	gettimeofday(&op->tv, NULL);

	// udp->uh_sum = check_sum((unsigned short *)udp, sizeof(struct udphdr));
	// ip->ip_sum = check_sum((unsigned short *)ip, sizeof(struct ip));
	//setsockopt(rawsock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)
	i = sendto(rawsock, (char *)outpacket, datalen, 0, &whereto, sizeof(struct sockaddr));

	if (i<0) {
		perror("sendto");
	}
	// printf("traceroute: wrote to %s:  %d chars, udp port=%d, ret=%d\n", inet_ntoa(dest->sin_addr), datalen, (port + seq), i);
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
	// printf("\ntype=%d, code=%d\n", icmp_hdr->icmp_type, icmp_hdr->icmp_code);
	return (icmp_hdr->icmp_type == ICMP_TIMXCEED? -1 : icmp_hdr->icmp_code + 1);;
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

	printf("\n%s", inet_ntoa(from->sin_addr));
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
	// printf("inaddr=%d, inaddr=%s\n", inaddr, inet_ntoa(dest->sin_addr));

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
	int on = 1;

	ping_packet_status ping_packet[64];

	datalen = sizeof(struct opacket);
	outpacket = (struct opacket *)malloc((unsigned)datalen);
	if (!outpacket) {
		perror("traceroute: malloc");
		exit(1);
	}
	
	(void) bzero((char *)outpacket, datalen);
	outpacket->ip.ip_dst = dest->sin_addr;
	outpacket->ip.ip_tos = 0;
	outpacket->ip.ip_v = IPVERSION;
	outpacket->ip.ip_id = 0;

	rawsock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (rawsock < 0) 
	{
		printf("Create raw socket fail!\n");
		return -1;
	}
	if (setsockopt(rawsock, SOL_SOCKET, SO_SNDBUF, (char *)&datalen, sizeof(datalen)) < 0) {
		perror("traceroute: SO_SNDBUF");
		exit(6);
	}
	// 手动设置IP首部
	if (setsockopt(rawsock, IPPROTO_IP, IP_HDRINCL, (char *)&on, sizeof(on)) < 0) {
		perror("traceroute: IP_HDRINCL");
		exit(6);
	}

	struct protoent *pe;
	int s;
	if ((pe = getprotobyname("icmp")) == NULL) {
		fprintf(stderr, "icmp: unknown protocol\n");
		exit(10);
	}
	if ((s = socket(AF_INET, SOCK_RAW, pe->p_proto)) < 0) {
		perror("traceroute: icmp socket");
		exit(5);
	}

	for (int ttl = 1; ttl < 64; ttl++, seq++) 
	{
		u_long lastaddr = 0;

		for (int probe = 0; probe < 3; ++probe) {
			struct timeval t1, t2;
			(void) gettimeofday(&t1, NULL);
			send_probe(seq, ttl);

			fd_set read_fd;
			FD_ZERO(&read_fd);
			FD_SET(s, &read_fd);
			int ret = select(s + 1, &read_fd, (fd_set *)0, (fd_set *)0, &tv);
			switch (ret)
			{
			case -1:
				fprintf(stderr, "fail to select!\n");
				break;
			case 0:
				printf(" * ");
				break;
			default:
			{
				__uint32_t fromlen = sizeof(from);
				int len = recvfrom(s, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&from, &fromlen);
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
				} else {
					printf(" *");
				}
			}	
			if (got_there || unreachable >= 2)
				exit(0);	
			}
		}
	}
	
	return 0;
}