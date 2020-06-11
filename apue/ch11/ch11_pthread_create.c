#include "apue.h"
#include <pthread.h>

pthread_t ntid;

void 
printfids(const char *s) {
	pid_t pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();

	printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid,
					(unsigned long) tid, (unsigned long) tid);
}

void *
thr_fn(void *arg) {
	printfids((char *)arg);
	return ((void *)0);
}

int 
main(void) {
	int err;

	err = pthread_create(&ntid, NULL, thr_fn, "test arg");
	if (err != 0) {
		err_exit(err, "can't create thread");
	}

	printfids("main thread:");
	sleep(1);
	exit(0);
}
