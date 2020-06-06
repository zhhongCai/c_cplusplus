#include "apue.h"
#include <setjmp.h>
#include <string.h>

#define TOK_ADD 5

jmp_buf jmpbuffer;

void do_line(char *);
void cmd_add(void);
int get_token(void);

int 
main(void) {
	char line[MAXLINE];

	if (setjmp(jmpbuffer) != 0) {
		err_quit("error quit now");
	}

	while (fgets(line, MAXLINE, stdin) != NULL) {
		do_line(line);
	}

	exit(0);
}

char *tok_ptr;
char *delim = ";";

void 
do_line(char *ptr) {
	int cmd;

	tok_ptr = ptr;
	while ((cmd = rand() % 6) > 1) {
		switch(cmd) {
		case TOK_ADD:
			cmd_add();
			break;
		default:
			break;
		}
	}
}

void 
cmd_add(void) {
	int token;
	token = get_token();
	if (token != TOK_ADD) {
		longjmp(jmpbuffer, 1);
	}
	printf("token = %d\n", token);
}

int 
get_token(void) {
	if (rand() % 3 == 0) {
		return -1;
	}
	printf("%s\n", tok_ptr);
	return TOK_ADD;
}
