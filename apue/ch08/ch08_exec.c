#include "apue.h"
#include <sys/wait.h>

char *env_init[] = { "USER-unknown", "PATH=/tmp", NULL };

int 
main(void) {
	pid_t pid;

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {
		if (execle("/Users/caizh/github/c_cplusplus/apue/ch08/ch08_echoall", "ch08_echoall", 
				"myarg1", "MYARG2", (char *)0, env_init) < 0) {
			err_sys("execle error");
		}
	}

	if (waitpid(pid, NULL, 0) < 0) {
		err_sys("wait error");
	}

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {
		if (execlp("ch08_echoall", "ch08_echoall", "only 1 arg", (char *)0) < 0) {
			err_sys("execlp error");
		}
	}

	exit(0);
}
