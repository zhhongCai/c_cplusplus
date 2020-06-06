#include <pwd.h>
#include <stddef.h>
#include <string.h>
#include "apue.h"


struct passwd *
getpwnam(const char *name) {
	struct passwd *ptr;

	setpwent();
	while ((ptr = getpwent()) != NULL) {
		if (strcmp(name, ptr->pw_name) == 0) {
			break;
		}
	}

	endpwent();
	return ptr;
}

int 
main(int argc, char *argv[]) {
	if (argc != 2) {
		err_quit("usage: a.out <username>");
	}

	struct passwd *ptr;

	if ((ptr = getpwnam(argv[1])) == NULL) {
		err_quit("username not exists");
	}

	printf("pw_name = %s\n", ptr->pw_name);
	printf("pw_passwd = %s\n", ptr->pw_passwd);
	printf("pw_uid = %d\n", ptr->pw_uid);
	printf("pw_gid = %d\n", ptr->pw_gid);
	printf("pw_gecos = %s\n", ptr->pw_gecos);
	printf("pw_dir = %s\n", ptr->pw_dir);
	printf("pw_shell = %s\n", ptr->pw_shell);
	printf("pw_class = %s\n", ptr->pw_class);
	printf("pw_change = %ld\n", ptr->pw_change);
	printf("pw_expire = %ld\n", ptr->pw_expire);

	exit(0);
}
