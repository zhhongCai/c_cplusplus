#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_sort.h"

void *get_elem(void *base, int index, int elem_size) {
	return (char *)base + index * elem_size;
}

void swap(void *base, int p, int p2, int elem_size) {
	void *pp = get_elem(base, p, elem_size);
	void *pp2 = get_elem(base, p2, elem_size);
	void *tmp = malloc(elem_size);
	memcpy(tmp, pp2, elem_size);
	memcpy(pp2, pp, elem_size);
	memcpy(pp, tmp, elem_size);
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

void my_qsort(void *base, int size, int elem_size, int (*cmpfn)(void *, void *)) {

	do_qsort(base, 0, size - 1, elem_size, cmpfn);	
}

