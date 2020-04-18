#include <stdio.h>
#include <stdlib.h>

double add(double a, double b) {
	return a + b;
}

double sub(double a, double b) {
	return a - b;
}

double mul(double a, double b) {
	return a * b;
}

double divide(double a, double b) {
	return a / b;
}

double (*oper_fun[])(double, double) = {add, sub, mul, divide};

int main() {
	int addOp = 0, subOp = 1, mulOp = 2, divOp = 3;
	double result = oper_fun[addOp](1.2, 22.3);
	printf("%f\n", result);
	exit(0);
}

