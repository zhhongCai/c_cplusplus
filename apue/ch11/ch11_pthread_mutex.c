#include "apue.h"
#include <pthread.h>

struct foo {
	int f_count;
	pthread_mutex_t f_lock;
	int f_id;
};

struct foo * 
foo_alloc(int id) {
	struct foo *fp;
	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->f_count = 1;
		fp->f_id = id;
		if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
			free(fp);
			return (NULL);
		}
	}
	return (fp);
}

void 
foo_hold(struct foo *fp) {
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;
	pthread_mutex_unlock(&fp->f_lock);
}

void 
foo_rele(struct foo *fp) {
	pthread_mutex_lock(&fp->f_lock);
	if (--fp->f_count == 0) {
		printf("foo_rele free fp\n");
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
		fp = NULL;
	} else {
		pthread_mutex_unlock(&fp->f_lock);
	}
}

void *
thr_fn1(void *arg) {
	printf("thread 1 running\n");
	if (arg) {
		return ((void *)-1);
	}
	struct foo *fp = (struct foo *)arg;
	foo_hold(fp);
	foo_hold(fp);
	foo_hold(fp);
	return ((void *)1);
}

void *
thr_fn2(void *arg) {
	printf("thread 2 running\n");
	if (arg) {
		return ((void *)-2);
	}
	struct foo *fp = (struct foo *)arg;
	foo_rele(fp);
	foo_rele(fp);
	return ((void *)2);
}


int 
main(void) {
	pthread_t tid, tid2;
	struct foo *fp;
	int err;
	void *tret;

	fp = foo_alloc(getpid());
	if (fp == NULL) {
		err_sys("foo_alloc error");
	}

	err = pthread_create(&tid, NULL, thr_fn1, (void *)fp);
	if (err != 0) {
		err_sys("pthread_create error");
	}
	
	err = pthread_create(&tid2, NULL, thr_fn2, (void *)fp);
	if (err != 0) {
		err_sys("pthread_create 2 error");
	}

	err = pthread_join(tid, &tret);
	printf("thread 1 return code %ld\n", (long) tret);

	err = pthread_join(tid2, &tret);
	printf("thread 2 return code %ld\n", (long) tret);

	if (fp != NULL) {
		printf("f_count = %d\n", fp->f_count);
		foo_rele(fp);	
	}

	if (fp == NULL) {
		printf("parent: foo free\n");
	} else {
		printf("f_count = %d\n", fp->f_count);
	}

	exit(0);
}
