#include <stdio.h>


int main() {
	int i;
	float a,a4,a8,a16,res;
	scanf_s("%f", &a);
	res = a;
	for (i = 0; i < 2; i++) {
		a = a * a; // a^2, a^4
	}
	res = a;
	a *= a;
	res *= a;
	a *= a;
	res *= a;
	printf("%f\n", res);
	return 0;
}