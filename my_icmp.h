#ifndef MY_ICMP_H 
#define MY_ICMP_H

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

typedef struct ping_packet_status {
	// 发送标志,1为已发送
	int flag;
	// 包的序列号
	int seq;
	struct timeval begin_time;
	struct timeval end_time;
} ping_packet_status;


#define PACKET_SEND_MAX_NUM 64

unsigned short check_sum(unsigned short *datap, int len);

int icmp_pack(struct icmp *icmp_head_data, int seq, int icmpid); 

struct timeval time_offset(struct timeval begin_time, struct timeval end_time);

int icmp_unpack(char *buf, int len, int icmpid, struct ping_packet_status *ping_packet); 

int raw_sock();


unsigned int set_dest(char *ipaddr, struct sockaddr_in *dest);

#endif