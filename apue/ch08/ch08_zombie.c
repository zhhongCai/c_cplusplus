#include "apue.h"

int 
main() {
	pid_t pid;

	if ((pid = fork()) < 0) {
		err_sys("fork error\n");
	} else if (pid == 0) {
		printf("child end\n");
	} else {
		sleep(30);
		system("ps -o pid,ppid,state,tty,command");
		printf("parent end\n");
	}

	exit(0);
}
