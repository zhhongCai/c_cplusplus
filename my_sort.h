#include <string.h>

void my_qsort(void *base, int size, int elem_size, int (*cmpfn)(void *, void *));

void my_merge_sort(void *base, int size, int elem_size, int (*cmpfn)(void *, void *));
