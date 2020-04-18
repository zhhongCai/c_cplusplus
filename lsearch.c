#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *lsearch(void *base, void *key, int len, int elemSize, int (*cmpfn)(void *, void *)) {
	void *elemAddr;
	for(int i = 0; i < len; i++) {
		elemAddr = (char *)base + i * elemSize;
		if (cmpfn(elemAddr, key) == 0) {
			return elemAddr;
		}
	}

	return NULL;
}

int int_cmp(void *elemAddr, void *key) {
	return *(int *)elemAddr - *(int *)key;
}

//int str_cmp(void **elemAddr, void **key) {
int str_cmp(void *elemAddr, void *key) {
	return strcmp(*(char **) elemAddr, *(char **)key);
}

void test_int() {
	int array[] = {2, 3, 6, 34, 98, 23};
	int len = 6, key = 34;

	int *found = lsearch(array, &key, len, sizeof(int), int_cmp);

	if (found == NULL) {
		printf("not found\n");
		return;
	}

	printf("found = %d\n", *found);

}

void test_str_arr() {
	char *arr[] = {"AA", "BB", "CC", "Dd", "Ee"};
	char *key = "Dd";
	int len = 5;

	char **found = lsearch(arr, &key, len, sizeof(char *), str_cmp);

	if (found == NULL) {
		printf("not found\n");
		return;
	}

	printf("found = %s\n", *(char **)found);

}

int main(void) {
	test_int();

	test_str_arr();

	exit(0);
}


