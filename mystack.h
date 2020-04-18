#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct {
	void *elems;
	int elem_size;
	int current_len;
	int size;
} mystack;

void mystack_new(mystack *s, int elem_size) {
	assert(elem_size > 0);

	s->elem_size = elem_size;
	s->current_len = 0;
	s->size = 4;

	s->elems = malloc(s->size * s->elem_size);

	assert(s->elems != NULL);
}

void mystack_dispose(mystack *s) {
	free(s->elems);
}

void mystack_grow(mystack *s) {
	s->size *= 2;

	printf("mystack_grow new size=%d\n", s->size);

	s->elems = realloc(s->elems, s->size * s->elem_size);

	assert(s->elems != NULL);
}

void mystack_push(mystack *s, void *elem_addr) {
	if (s->current_len == s->size) {
		mystack_grow(s);
	}
	
	void *target = (char *)s->elems + s->current_len * s->elem_size;
	memcpy(target, elem_addr, s->elem_size);

	s->current_len++;
}

void mystack_pop(mystack *s, void *elem_addr) {
	void *source = (char *)s->elems + (s->current_len - 1) * s->elem_size;
	memcpy(elem_addr, source, s->elem_size);
	s->current_len--;
}


