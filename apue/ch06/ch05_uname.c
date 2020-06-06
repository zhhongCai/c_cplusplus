#include <sys/utsname.h>
#include "apue.h"

int 
main(void) {
	struct utsname info;

	if (uname(&info) == -1) {
		err_quit("uname error");				
	}

	printf("sysname = %s\n", info.sysname);
	printf("nodename = %s\n", info.nodename);
	printf("release = %s\n", info.release);
	printf("version = %s\n", info.version);
	printf("machine = %s\n", info.machine);

	exit(0);
}
