#include "apue.h"
#include <pthread.h>

int 
makethread(void *(*fn)(void *), void *arg) {
	int err;
	pthread_t tid;
	pthread_attr_t attr;

	err = pthread_attr_init(&attr);
	if (err != 0) {
		return (err);
	}

	err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (err == 0) {
		err = pthread_create(&tid, &attr, fn, arg);
	}

	pthread_attr_destroy(&attr);
	return (err);
}

void * 
thr_fn(void *arg) {
	printf("in detach thread arg = %s\n", (char *)arg);
	return ((void *)0);
}

int 
main(void) {
	int err;
	err = makethread(thr_fn, "hello thread");
	if (err != 0) {
		err_sys("create thread error");
	}

	sleep(1);
	exit(0);
}
