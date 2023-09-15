#include <stdio.h>
#include <math.h>


int factorial(int a) {
	if (a == 0) {
		return 1;
	}
	else {
		return a * factorial(a - 1);
	}
}

int main() {
	int x, n, i;
	float s = 0.0;
	scanf_s("%d %d", &x, &n);
	for (i = 0; i < n; i++) {
		s += (float)(pow(x, i) / factorial(i) * pow(-1, i));
	}
	printf("%f", s);
	return 0;
}