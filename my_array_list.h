#ifndef MY_ARRAY_LIST_H
#define MY_ARRAY_LIST_H


typedef struct {
	void *values;
	int elem_size;
	int size;
	int current_size;
} my_array_list;

void list_new(my_array_list *list, int size, int elem_size);
void list_dispose(my_array_list *list);

void list_add(my_array_list *list, void *value_addr);
int list_remove(my_array_list *list, int index);
int list_remove_val(my_array_list *list, void *value, int (*cmpfn)(void *, void *));
void * list_get(my_array_list *list, int index);
int list_contain(my_array_list *list, void *value, int (*cmpfn)(void *, void *));

void list_print(my_array_list *list, void (*print_elem_value)(void *)); 

#endif
