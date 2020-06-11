#include "apue.h"
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void 
prepare(void) {
	int err;

	printf("preparing locks...\n");
	if ((err = pthread_mutex_lock(&lock)) != 0) {
		err_cont(err, "can't lock lock in prepare handler");
	}
	if ((err = pthread_mutex_lock(&lock2)) != 0) {
		err_cont(err, "can't lock lock2 in prepare handler");
	}
}

void parent(void) {
	int err;

	printf("parent unlocking locks...\n");

	if ((err = pthread_mutex_unlock(&lock)) != 0) {
		err_cont(err, "can't unlock lock in prarent hanler");
	}
	if ((err = pthread_mutex_unlock(&lock2)) != 0) {
		err_cont(err, "can't unlock lock2 in prarent hanler");
	}
}

void 
child(void) {
	int err;
	
	printf("parent unlocking locks...\n");

	if ((err = pthread_mutex_unlock(&lock)) != 0) {
		err_cont(err, "can't unlock lock in child hanler");
	}
	if ((err = pthread_mutex_unlock(&lock2)) != 0) {
		err_cont(err, "can't unlock lock2 in child hanler");
	}
}

void *
thr_fn(void *arg) {
	printf("thread started...\n");
	pause();
	return (0);
}

int 
main(void) {
	int err;
	pid_t pid;
	pthread_t tid;

	if ((err = pthread_atfork(prepare, parent, child)) != 0) {
		err_exit(err, "can't install fork handler");
	}

	if ((err = pthread_create(&tid, NULL, thr_fn, 0)) != 0) {
		err_exit(err, "can't create thread");
	}

	sleep(2);
	printf("parent about to fork...\n");

	if ((pid = fork()) < 0) {
		err_quit("fork failed");
	} else if (pid == 0) {
		printf("child returnd from fork\n");
	} else {
		printf("parent  returnd from fork\n");
	}

	exit(0);
}
