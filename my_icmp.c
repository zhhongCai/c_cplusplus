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

unsigned short check_sum(unsigned short *datap, int len) 
{
	uint32_t sum = 0;

    while(len > 1) 
	{
        sum += *datap;
        datap++;
        len -= 2;
    }
    if(len)
	{
		sum += (*(uint8_t *)datap);
	}

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum = (sum >> 16) + sum;

    return ~sum;
}


int icmp_pack(struct icmp *icmp_head_data, int seq, int icmpid) 
{
	int i = 0;
	icmp_head_data->icmp_type = ICMP_ECHO;
	icmp_head_data->icmp_code = 0;
	icmp_head_data->icmp_cksum = 0;
	icmp_head_data->icmp_seq = seq;
	icmp_head_data->icmp_id = icmpid;

	struct timeval *tval = (struct timeval *)icmp_head_data->icmp_data;
	gettimeofday(tval, NULL);

	int len = sizeof(icmp_head_data) + sizeof(tval);
	icmp_head_data->icmp_cksum = check_sum((unsigned short *) icmp_head_data, len);

	return len;
}

struct timeval time_offset(struct timeval begin_time, struct timeval end_time)
{
	struct timeval ans;
	ans.tv_sec = end_time.tv_sec - begin_time.tv_sec;
	ans.tv_usec = end_time.tv_usec - begin_time.tv_usec;

	if (ans.tv_usec < 0) 
	{
		ans.tv_sec--;
		ans.tv_sec += 1000000;
	}
	return ans;
}

int icmp_unpack(char *buf, int len, int icmpid, struct ping_packet_status *current_packet) 
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

	if ((icmp_hdr->icmp_type == ICMP_ECHOREPLY) && (icmp_hdr->icmp_id == icmpid))
	{
		if ((icmp_hdr->icmp_seq < 0 && icmp_hdr->icmp_seq >= PACKET_SEND_MAX_NUM)) 
		{
			fprintf(stderr, "icmp packet seq is out of range!\n");
			return -1;
		}

		current_packet->flag = 0;
		begin_time = current_packet->begin_time;
		gettimeofday(&recv_time, NULL);
		offset_time = time_offset(begin_time, recv_time);
		rtt = offset_time.tv_sec * 1000 + offset_time.tv_usec / 1000;

		printf("%d byte from %s: icmp_seq=%u ttl=%d rtt=%d ms\n",
			len, inet_ntoa(ip_hdr->ip_src), icmp_hdr->icmp_seq, ip_hdr->ip_ttl, rtt);
	} else {
		fprintf(stderr, "Invalid ICMP packet! Tts id is not matched!\n");
		return -1;
	}
	return 0;
}

int raw_sock() 
{
	struct protoent* protocol = getprotobyname("icmp");
	if (protocol == NULL) {
		printf("Fail to getprotocolbyname!\n");
		return -1;
	}
	int rawsock = socket(AF_INET, SOCK_RAW, protocol->p_proto);
	if (rawsock < 0) 
	{
		printf("Fail to create socket!\n");
		return -1;
	}	
	return rawsock;
}

unsigned int set_dest(char *ipaddr, struct sockaddr_in *dest) {
	bzero(dest, sizeof(struct sockaddr_in));

	dest->sin_family = AF_INET;

	unsigned int inaddr = 1;
	inaddr = inet_addr(ipaddr);

	if (inaddr == INADDR_NONE)
	{
		struct hostent *host = NULL;
		host = gethostbyname(ipaddr);
		if (host == NULL) 
		{
			printf("Fail to gethostbyname!\n");
			return -1;
		}

		memcpy((char *) &(dest->sin_addr), host->h_addr, host->h_length);
	} else {
		memcpy((char *) &(dest->sin_addr), &inaddr, sizeof(inaddr));
	}

	inaddr = dest->sin_addr.s_addr;
	return inaddr;
}
