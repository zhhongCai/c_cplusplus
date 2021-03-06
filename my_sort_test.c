#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "my_sort.h"

void print_array(int array[], int begin, int size) {
	for (int i = begin; i < size; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

void print_string(char *array[], int begin, int size) {
	for (int i = begin; i < size; i++) {
		printf("%s ", array[i]);
	}
	printf("\n");
}

int cmp_int(void *a, void *b) {
	return *(int *)a - *(int *)b;
}

int cmp_string(void *a, void *b) {
	return strcmp(*(char **)a, *(char **)b);
}

void test_int() {
	int size = 101;
	int array[size];

	srand(time(NULL));
	
	for (int i = 0; i < size; i++) {
		array[i] = 	rand()%size;
	}

	print_array(array, 0, size);
	my_qsort(array, size, sizeof(int), cmp_int);
	print_array(array, 0, size);
}

void test_string() {
	char *array[] = {"xabout", "why", "where", "import", "export", "qsort", "fine",
		"ok", "good", "job", "yes", "no"};
	print_string(array, 0, 12);
	my_qsort(array, 12, sizeof(char *), cmp_string);
	print_string(array, 0, 12);
}

void test_merge_sort_string() {
	char *array[] = {"about", "why", "where", "import", "export", "qsort", "fine",
		"ok", "good", "job", "yes", "no"};
	print_string(array, 0, 12);
	my_merge_sort(array, 12, sizeof(char *), cmp_string);
	print_string(array, 0, 12);
}


void test_merge_sort_int() {
	int size = 100;
	int array[size];

	srand(time(NULL));
	
	for (int i = 0; i < size; i++) {
		array[i] = 	rand()%size;
	}

	print_array(array, 0, size);
	my_merge_sort(array, size, sizeof(int), cmp_int);
	print_array(array, 0, size);
}

int main() {
	test_int();

	test_string();

	test_merge_sort_int();

	test_merge_sort_string();
	printf("done\n");

	exit(0);
}

