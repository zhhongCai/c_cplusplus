#include <shadow.h>
#include <stddef.h>
#include <string.h>
#include "apue.h"


struct spwd *
getspnam(const char *name) {
	struct spwd *ptr;

	setspent();
	while ((ptr = getspent()) != NULL) {
		if (strcmp(name, ptr->sp_namp) == 0) {
			break;
		}
	}

	endspent();
	return ptr;
}

int 
main(int argc, char *argv[]) {
	if (argc != 2) {
		err_quit("usage: a.out <username>");
	}

	struct spwd *ptr;

	if ((ptr = getspnam(argv[1])) == NULL) {
		err_quit("username not exists");
	}

	printf("sp_namp = %s\n", ptr->sp_namp);
	printf("sp_pwdp = %s\n", ptr->sp_pwdp);
	printf("sp_lstchg = %d\n", ptr->sp_lstchg);
	printf("sp_min = %d\n", ptr->sp_min);
	printf("sp_max = %d\n", ptr->sp_max);
	printf("sp_warn = %d\n", ptr->sp_warn);
	printf("sp_inact = %d\n", ptr->sp_inact);
	printf("sp_expire = %d\n", ptr->sp_expire);
	printf("sp_flag = %d\n", ptr->sp_flag);

	exit(0);
}
