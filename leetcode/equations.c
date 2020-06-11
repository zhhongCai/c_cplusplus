#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
/**
 *leetcode 990
 *
 */ 


bool equationsPossible(char ** equations, int equationsSize);

short find_root(short x, short *eq_set);

void 
print_eq(char **, short);

int 
main(void) {
	char *eqs[] = {"a==a"};
	print_eq(eqs, 1);
	printf("%d\n", equationsPossible(eqs, 1));

	char *eqs2[] = {"a==b", "b==c", "c==a"};
	print_eq(eqs2, 3);
	printf("%d\n", equationsPossible(eqs2, 3));


	char *eqs3[] = {"a==b", "b==c", "c==a"};
	print_eq(eqs3, 3);
	printf("%d\n", equationsPossible(eqs3, 3));


	char *eqs4[] = {"a==b", "b==c", "c!=d"};
	print_eq(eqs4, 3);
	printf("%d\n", equationsPossible(eqs4, 3));

	char *eqs5[] = {"a==b", "b==c", "c!=a"};
	print_eq(eqs5, 3);
	printf("%d\n", equationsPossible(eqs5, 3));

	char *eqs6 []= {"a!=a"};
	print_eq(eqs6, 1);
	printf("%d\n", equationsPossible(eqs6, 1));

	char *eqs7[] = {"a!=b"};
	print_eq(eqs7, 1);
	printf("%d\n", equationsPossible(eqs7, 1));

	char *eqs8[] = {"e==e","d!=e","c==d","d!=e"};
	print_eq(eqs8, 4);
	printf("%d\n", equationsPossible(eqs8, 4));

	char *eqs9[] = {"e==e","d!=e","c==d","d!=e", "d==e"};
	print_eq(eqs9, 5);
	printf("%d\n", equationsPossible(eqs9, 5));

	char *eqs10[] = {"a==b","e==c","b==c","a!=e"};
	print_eq(eqs10, 4);
	printf("%d\n", equationsPossible(eqs10, 4));

}

void 
print_eq(char **eqs, short length) {
	for (int i = 0; i < length; i++) {
		printf("%s ", *(eqs + i));
	}
	printf(": "); 
}

bool equationsPossible(char ** equations, int equationsSize){

	short eq_set[26];
	for (int i = 0; i < 26; i++) {
		eq_set[i] = -1;
	}
	short j, k, jroot, kroot, kr; 
	char *tmp;
	for (short i = 0; i < equationsSize; i++) {
		tmp = *(equations + i);
		if (*(tmp + 1) == '=') {
			j = *(tmp) - 'a';
			k = *(tmp + 3) - 'a';
			jroot = find_root(j, eq_set);
			kroot = find_root(k, eq_set);

			if (jroot != -1) {
				if (kroot != -1) {
					short kr = *(eq_set + k);	
					*(eq_set + k) = jroot;
					*(eq_set + kr) = jroot;
				} else {
					*(eq_set + k) = jroot;
				}
			} else {
				if (kroot != -1) {
					*(eq_set + j) = kroot;
				} else {
					*(eq_set + j) = j;
					*(eq_set + k) = j;
				}
			}
		}
	}
	
	for (int i = 0; i < equationsSize; i++) {
		tmp = *(equations + i);
		if (*(tmp + 1) == '!') {
			j = *(tmp) - 'a';
			k = *(tmp + 3) - 'a';
			if (j == k) {
				return false;
			}
			jroot = find_root(j, eq_set);
			kroot = find_root(k, eq_set);
			if (jroot != -1 && jroot == kroot) {
				return false;
			}
		}

	}

	return true;

}

short 
find_root(short x, short *eq_set) {
	short root = -1;
	short *tmp = eq_set;
	while (*(eq_set + x) != root) {
		x = *(tmp + x);
		root = *(tmp + x);		
	}

	return root;
}
