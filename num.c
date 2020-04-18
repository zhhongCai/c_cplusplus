#include<stdio.h>
#include<stdlib.h>

int main() {
	short a = -1;
	int b = a;
	long c = a;
	float d = 7.0;
	short e = *(short *)&d;
	printf("b=%x,c=%x,e=%d\n", b, c, e);
	exit(0);
}
