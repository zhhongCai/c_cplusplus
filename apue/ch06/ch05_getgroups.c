#include "apue.h"
#include <unistd.h>
#include <errno.h>
#include <grp.h>

int 
main(void) {
	int n, i;
	gid_t grouplist[32];
	
	if ((n = getgroups(0, grouplist)) == -1) {
		/*EFAULT 参数list数组地址不合法。EINVAL 参数size值不足以容纳所有的组*/
		if (errno == EFAULT) {
			printf("invalid grouplist\n");
		} else if (errno == EINVAL) {
			printf("size too small\n");
		}
		err_quit("getgroups error");
	}

	if ((n = getgroups(n, grouplist)) == -1) {
		/*EFAULT 参数list数组地址不合法。EINVAL 参数size值不足以容纳所有的组*/
		if (errno == EFAULT) {
			printf("invalid grouplist\n");
		} else if (errno == EINVAL) {
			printf("size too small\n");
		}

		err_quit("getgroups error");
	}

	printf("n = %d, group id list: \n", n);

	struct group *gptr;
	for (i = 0; i < n; i++) {
		printf("gid = %u", grouplist[i]);

		if ((gptr = getgrgid(grouplist[i])) == NULL) {
			printf("getgrgid error\n");				
			continue;
		}
		printf(" group name = %s\n", gptr->gr_name);				
	}

	exit(0);
}
