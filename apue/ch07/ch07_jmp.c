#include "apue.h"
#include <setjmp.h>

static void f1(int, int, int, int);
static void f2(void);

static jmp_buf jmpbuffer;
static int globval;

int 
main() {
	int autoval;
	register int regval;
	volatile int volaval;
	static int staticval;

	globval = 1;
	autoval = 2;
	regval = 3;
	volaval = 4;
	staticval = 5;

	if (setjmp(jmpbuffer) != 0) {
		printf("after longjmp: \n");
		printf("globval = %d, autoval = %d, regval = %d, volaval = %d, staticval = %d\n",
						globval, autoval, regval, volaval, staticval);
		exit(0);
	}
	
	globval = 95;
	autoval = 96;
	regval = 97;
	volaval = 98;
	staticval =99;

	f1(autoval, regval, volaval, staticval);

	exit(0);
	
}

static void 
f1(int i, int j, int k, int l) {
	printf("in f1():\n");
	printf("globval = %d, autoval = %d, regval = %d, volaval = %d, staticval = %d\n",
				globval, i, j, k, l);
	f2();
}

static void 
f2(void) {
	longjmp(jmpbuffer, 1);
}
