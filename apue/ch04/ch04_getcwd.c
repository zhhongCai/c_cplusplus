#include "apue.h"

int 
main(void) {
	char *ptr;
	size_t size;

	if (chdir("/usr/lib") < 0) {
		err_sys("chdir fialed");
	}

	ptr = path_alloc(&size);
	if (getcwd(ptr, size) == NULL) {
		err_sys("getcwd failed");
	}

	printf("cwd = %s\n", ptr);
	exit(0);
}
