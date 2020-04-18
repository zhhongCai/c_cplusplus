#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

float average(int n_values, ...) {
	va_list var_arg;
	float sum = 0;
	int count;

	va_start(var_arg, n_values);

	for(count = 0; count < n_values; count++) {
		sum += va_arg(var_arg, int);
	}

	va_end(var_arg);

	return sum / n_values;
}

int main(void) {
	printf("average=%f\n", average(3, 12, 23, 45));
	exit(0);
}
