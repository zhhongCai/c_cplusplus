#include "apue.h"

static void 
sig_alarm(int signo) {
}

unsigned int
mysleep(unsigned int seconds) {
	struct sigaction newact, oldact;
	sigset_t newmask, oldmask, suspmask;
	unsigned int unslept;

	newact.sa_handler = sig_alarm;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	sigaction(SIGALRM, &newact, &oldact);

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);

	alarm(seconds);
	suspmask = oldmask;
	
	sigdelset(&suspmask, SIGALRM);

	sigsuspend(&suspmask);

	unslept = alarm(0);

	sigaction(SIGALRM, &oldact, NULL);

	sigprocmask(SIG_SETMASK, &oldmask, NULL);

	return (unslept);

}

int 
main(void) {
	mysleep(3);
	printf("done\n");
}
