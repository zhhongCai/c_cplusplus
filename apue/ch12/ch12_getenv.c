#include "apue.h"
#include <limits.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXSTRINGSZ 4096

static pthread_key_t key;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;
pthread_mutex_t env_mutex = PTHREAD_MUTEX_INITIALIZER;

extern char **environ;

static void 
thread_init(void) {
	pthread_key_create(&key, free);
}

char *getenv(const char *name) {
	int i, len;
	char *envbuf;

	pthread_once(&init_done, thread_init);
	pthread_mutex_lock(&env_mutex);

	envbuf = (char *) pthread_getspecific(key);
	if (envbuf == NULL) {
		envbuf = malloc(MAXSTRINGSZ);
		if (envbuf == NULL) {
			pthread_mutex_unlock(&env_mutex);
			return (NULL);
		}
	}

	len = strlen(name);
	for (i = 0; environ[i] != NULL; i++) {
		if ((strncmp(name, environ[i], len) == 0) &&
						(environ[i][len] == '=')) {
			strncpy(envbuf, &environ[i][len+1], MAXSTRINGSZ - 1);
			pthread_mutex_unlock(&env_mutex);
			return (envbuf);
		}
	}

	pthread_mutex_unlock(&env_mutex);

	return (NULL);
}

void *
func1(void *arg) {
	char *name = (char *)arg;
	printf("in thread: %s: %s\n", name, getenv(name));	
	
	return (void *)0;
}

int 
main(int argc, char *argv[]) {
	if (argc < 2) {
		err_sys("usage: ./a.out envname");
	}

	int i, err;
	pthread_t tid[argc];

	for (i = 1; i < argc; i++) {
		err = pthread_create(&tid[i-1], NULL, func1, argv[i]);
		if (err != 0) {
			err_sys("create thread error");
		}
	}

	for (i = 0; i < argc - 1; i++) {
		pthread_join(tid[i], NULL);	
	}

	printf("done\n");
	
	exit(0);
}
