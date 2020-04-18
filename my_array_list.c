#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "my_array_list.h"

void list_new(my_array_list *list, int size, int elem_size) {
	assert(size > 0 && elem_size > 0);
	list->size = size;
	list->current_size = 0;
	list->elem_size = elem_size;
	list->values = (void *)malloc(size * elem_size);
	assert(list->values != NULL);
}

void list_grow(my_array_list *list) {
	list->size *= 2;
	printf("list grow new size=%d\n", list->size);
	list->values = realloc(list->values, list->size * list->elem_size);	
	assert(list->values != NULL);
}

void list_dispose(my_array_list *list) {
	free(list->values);
}

void list_add(my_array_list *list, void *value_addr) {
	if (list->current_size == list->size) {
		list_grow(list);
	}

	void *target = (char *)list->values + list->current_size * list->elem_size;
	memcpy(target, value_addr, list->elem_size);
	list->current_size++;
}

int list_remove(my_array_list *list, int index) {
	assert(index < list->current_size && index >= 0);
	
	void *target, *source;
	for (int i = index; i < list->current_size; i++) {
		if (i == list->current_size - 1) {
			break;
		}
		target = (char *)list->values + i * list->elem_size;
		source = (char *)list->values + (i + 1) * list->elem_size;
		memcpy(target, source, list->elem_size);
	}	
	list->current_size--;

	return 1;
}

int list_remove_val(my_array_list *list, void *value, int (*cmpfn)(void *, void *)) {
	int index = list_contain(list, value, cmpfn);
	if (index < 0) {
		return -1;
	}
	return list_remove(list, index);	
}

void * list_get(my_array_list *list, int index) {
	assert(index < list->current_size && index >= 0);
	
	void *source = (char *)list->values + index * list->elem_size;
	return source;
}


int list_contain(my_array_list *list, void *value, int (*cmpfn)(void *, void *)) {
	void *target;
	for (int i = 0; i < list->current_size; i++) {
		target = (char *)list->values + i * list->elem_size;
//		if (memcmp(value, target, list->elem_size) == 0) {
		if (cmpfn(value, target) == 0) {
			return i;
		}
	}
	return -1;
}

void list_print(my_array_list *list, void (*print_elem_value)(void *)) {
	for (int i = 0; i < list->current_size; i++) {
		print_elem_value((char *)list->values + i * list->elem_size);
	}
	printf("\n");
}

