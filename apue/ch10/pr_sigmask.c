#include "apue.h"
#include <signal.h>

void 
pr_mask(char *ptr) {
	sigset_t sigset;
	unsigned int errno_save;

	errno_save = errno;
	if (sigprocmask(0, NULL, &sigset) < 0) {
		err_sys("sigprocmask error");
	}
	
	printf("%s ", ptr);
	
	if (sigismember(&sigset, SIGINT)) {
		printf("SIGINT");
	}
	
	if (sigismember(&sigset, SIGQUIT)) {
		printf("SIGQUIT");
	}
	
	if (sigismember(&sigset, SIGUSR1)) {
		printf("SIGUSR1");
	}
	
	if (sigismember(&sigset, SIGUSR2)) {
		printf("SIGUSR2");
	}
	
	if (sigismember(&sigset, SIGINT)) {
		printf("SIGINT");
	}
	

	printf("\n");

	errno = errno_save;
}
