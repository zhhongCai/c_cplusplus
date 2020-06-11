#include "apue.h"
#include <pthread.h>
#include <time.h>

int 
main(void) {
	int err;
	struct timespec tout;
	struct tm *tmp;
	char buf[64];
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&lock);
	printf("mutext is locked\n");

	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("current time is %s\n", buf);

	tout.tv_sec += 10;

	// macos 没有pthread_mutex_timedlock
	// err = pthread_mutex_timedlock(&lock, &tout);

	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("time is now %s\n", buf);

	if (err == 0) {
		printf("mutex locked again!\n");
	} else {
		printf("can't lock mutex again: %s\n", strerror(err));
	}

	exit(0);
}
