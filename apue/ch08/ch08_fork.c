#include "apue.h"

int globalval = 33;
char buf[] = "a string write to stdout\n";

int 
main(void) {
	pid_t pid;
	int var = 8;

	if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1) {
		err_sys("write error");
	}
	printf("before fork\n");

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {
		globalval++;
		var++;
	} else {
		sleep(2);
	}

	printf(pid == 0 ? "child" : "parent");
	printf(": pid = %ld, globalval = %d, va = %d\n",(long) getpid(), globalval, var);
	exit(0);
}
