#include <stdio.h>


int main() {
	int i;
	float a,res;
	scanf_s("%f", &a);
	a *= a;
	a *= a;
	res = a;
	a *= a;
	res *= a;
	a *= a;
	res *= a;
	printf("%f\n", res);
	return 0;
}