#include "mystack.h"

void test_int(int count) {
	mystack s;
	mystack_new(&s, sizeof(int));

	for (int i = 0; i < count; i++) {
		mystack_push(&s, &i);

	}

	int tmp;
	for (int i = 0; i < count; i++) {
		mystack_pop(&s, &tmp);
//		printf("tmp=%d\n", tmp);
	}

	mystack_dispose(&s);
}

void test_double(int count) {
	mystack s;
	mystack_new(&s, sizeof(double));

	double tmp = 0.0;
	for (int i = 0; i < count; i++) {
		tmp += i;
		mystack_push(&s, &tmp);

	}

	for (int i = 0; i < count; i++) {
		mystack_pop(&s, &tmp);
//		printf("tmp=%d\n", tmp);
	}

	mystack_dispose(&s);
}

int main() {
	int count = 1000000;

	test_int(count);

	test_double(count);

	exit(0);
}




