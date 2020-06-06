#include <grp.h>
#include <stddef.h>
#include <string.h>
#include "apue.h"


struct group *
getgrnam(const char *name) {
	struct group *ptr;

	setgrent();
	while ((ptr = getgrent()) != NULL) {
		if (strcmp(name, ptr->gr_name) == 0) {
			break;
		}
	}

	endgrent();
	return ptr;
}

int 
main(int argc, char *argv[]) {
	if (argc != 2) {
		err_quit("usage: a.out <groupname>");
	}

	struct group *ptr;

	if ((ptr = getgrnam(argv[1])) == NULL) {
		err_quit("groupname not exists");
	}

	printf("gr_name = %s\n", ptr->gr_name);
	printf("gr_passwd = %s\n", ptr->gr_passwd);
	printf("gr_gid = %d\n", ptr->gr_gid);
	printf("gr_mem[0] = %s\n", *ptr->gr_mem);

	exit(0);
}
