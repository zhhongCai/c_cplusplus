#include "apue.h"
#include <sys/wait.h>

int 
main(void) {
	pid_t pid;
	
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {
		if (execl("/Users/caizh/github/c_cplusplus/apue/ch08/testinterpreter.sh",
				"testinterpreter.sh", "myarg1", "MYARG2", (char *)0) < 0) {
			err_sys("execl error");
		}
	}

	if (waitpid(pid, NULL, 0) < 0) {
		err_sys("waitpid error");
	}

	exit(0);
}
