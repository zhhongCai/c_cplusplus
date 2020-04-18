#include "my_array_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_int_value(void *elem_addr) {
	printf("%d ", *(int *)elem_addr);
}

void print_string(void *elem_addr) {
	printf("%s ", (char *)elem_addr);
}

int cmp_int(void *source, void *target) {
	return *(int *)source - *(int *)target;
}

int cmp_string(void *source, void *target) {
	return strcmp((char *)source, (char *)target);
}

void test_int(int count) {
	my_array_list list;

	list_new(&list, 4, sizeof(int));

	for (int i = 0; i < count; i++) {
		list_add(&list, &i);
	}
	list_print(&list, print_int_value);

	int t = 5;	
	if (list_contain(&list, &t, cmp_int) >= 0) {
		printf("contain 5\n");
	}

	int *tmp = (int *)list_get(&list, 4);
	printf("tmp = %d\n", *tmp);

	list_remove_val(&list, tmp, cmp_int);

	if (list_contain(&list, tmp, cmp_int) < 0) {
		printf("%d remove succes\n", *tmp);
	}
	list_print(&list, print_int_value);

	list_dispose(&list);
}

void test_string(int count) {
	my_array_list list;

	list_new(&list, 2, sizeof(char *));
	
	for (int i = 0; i < count; i++) {
		char tmp[10];
		sprintf(tmp, "string%d", i);
		list_add(&list, &tmp);	

		if (list_contain(&list, &tmp, cmp_string) >= 0) {
			printf("contain %s\n", tmp);
		} else {
			printf("add error!!!%s\n", tmp);
		}
	}	

	list_print(&list, print_string);

	char *tt = (char *)list_get(&list, 6);
	printf("tt = %s", tt);
	
	list_remove_val(&list, tt, cmp_string);
	if (list_contain(&list, &tt, cmp_string) == -1) {
		printf(" remove success\n");
	}

	char *tmp2 = "string7";
	int index = list_contain(&list, tmp2, cmp_string);
	if (index >= 0){
		printf("%s is in list\n", (char *)list_get(&list, index));	
	}
	
	list_print(&list, print_string);

}

int main() {
	int count = 10;

	test_int(count);

	test_string(count);
	exit(0);
}
