#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "my_sort.h"

void *get_elem(void *base, int index, int elem_size) {
	return (char *)base + index * elem_size;
}
/*
void swap(void *base, int p, int p2, int elem_size) {
	void *pp = get_elem(base, p, elem_size);
	void *pp2 = get_elem(base, p2, elem_size);
	void *tmp = malloc(elem_size);
	memcpy(tmp, pp2, elem_size);
	memcpy(pp2, pp, elem_size);
	memcpy(pp, tmp, elem_size);
}
*/

void swap(void *base, int p, int p2, int elem_size) {
	char *pp = get_elem(base, p, elem_size);
	char *pp2 = get_elem(base, p2, elem_size);
	char tmp;
	for (int i = 0; i < elem_size; i++) {
		tmp = *pp;
		*pp = *pp2;
		*pp2 = tmp;
		pp++;
		pp2++;
	} 
}


int my_partition(void *base, int begin, int end, int elem_size, int (*cmpfn)(void *, void*)) {
	// 取第一个元素为轴点
	void *pivot = (char *)base + begin * elem_size;

	int low = begin;
	int hight = end;
	
	while (low < hight) {
		// 从右往左找到比pivot小的位置
		while (hight > low && cmpfn(get_elem(base, hight, elem_size), pivot) >= 0){
			hight--;	
		}	
		// 从左往右找到比pivot大的位置
		while (low < hight && cmpfn(get_elem(base, low, elem_size), pivot) <= 0) {
			low++;
		}
		// 找到交换low,hight
		if (hight > low) {
//			printf("low=%d, hight=%d\n", low, hight);
			swap(base, hight, low, elem_size); 
		}		
	}
	if (low > begin) {
		swap(base, low, begin, elem_size);
	}
//	printf("after parition: ");
//	printf("begin=%d, end=%d, low=%d, hight=%d, pivot_index=%d\n", begin, end, low, hight, low);

	return low;	
}

void do_qsort(void *base, int begin, int end, int elem_size, int (*cmpfn)(void *, void*)) {
	if (end <= begin) {
		return;
	}	

	// 分区，返回轴点位置
	int pivot_index = my_partition(base, begin, end, elem_size, cmpfn);

	// 对左半部分排序
	do_qsort(base, begin, pivot_index - 1 , elem_size, cmpfn);	
	// 对右半部分排序
	do_qsort(base, pivot_index + 1, end, elem_size, cmpfn);	
}

/**
 * quick sort
 */
void my_qsort(void *base, int size, int elem_size, int (*cmpfn)(void *, void *)) {

	do_qsort(base, 0, size - 1, elem_size, cmpfn);	
}


void do_merge(void *base, int from, int to, int elem_size, int (*cmpfn)(void *, void *)) {
	if (from >= to) {
		return;
	}
	int middle = from + (to - from) / 2; 
	void *tmp = malloc((to - from + 1) * elem_size);
	assert(tmp != NULL);
	
	int left_index = from;
	int right_index = middle + 1;
	for (int i = 0; i <= to - from; i++) {
		void *elem = get_elem(tmp, i, elem_size);	
		if (left_index > middle) {
			void *right = get_elem(base, right_index, elem_size);
			memcpy(elem, right, elem_size);
			right_index++;	
			continue;
		}
		if (right_index > to) {
			void *left = get_elem(base, left_index, elem_size);
			memcpy(elem, left, elem_size);
			left_index++;	
			continue;
		}

		void *left = get_elem(base, left_index, elem_size);
		void *right = get_elem(base, right_index, elem_size);
		if (cmpfn(left, right) > 0) {
			memcpy(elem, right, elem_size);
			right_index++;	
		} else {
			memcpy(elem, left, elem_size);
			left_index++;
		}
	
	}

	for (int i = 0; i <= to - from; i++) {
		void *tmp_elem = get_elem(tmp, i, elem_size);	
		void *base_elem = get_elem(base, from + i, elem_size);	
		memcpy(base_elem, tmp_elem, elem_size);
	}
//	printf("from=%d, to=%d, middle=%d, left_index=%d, right_index=%d\n", from, to, middle, left_index, right_index);

	free(tmp);
}

void do_my_merge_sort(void *base, int from, int to, int elem_size, int (*cmpfn)(void *, void*)) {
	if (from >= to) {
		return;	
	}
	if (to - from == 1) {
		if (cmpfn(get_elem(base, from, elem_size), get_elem(base, to, elem_size)) > 0) {
			swap(base, from, to, elem_size);
		}
		return;	
	}
	int middle = from + (to - from) / 2; 

	do_my_merge_sort(base, from, middle, elem_size, cmpfn);
	do_my_merge_sort(base, middle + 1, to, elem_size, cmpfn);

	do_merge(base, from, to, elem_size, cmpfn);
}


/**
 * merge sort
 */
void my_merge_sort(void *base, int size, int elem_size, int (*cmpfn)(void *, void*)) {
	do_my_merge_sort(base, 0, size - 1, elem_size, cmpfn);
}


