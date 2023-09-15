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
	int i, j, n;
	float s = 0.0, sub_s = 1.0;
	scanf_s("%d", &n);
	for (i = 0; i < n; i++) {
		sub_s = 1.0;
		for (j = i; j < 2 * n; j++) {
			sub_s *= (pow(-1, i) * (j + 1)) / (factorial(j - i + 1));
		}
		s += sub_s;
	}
	printf("%f", s);
	return 0;
}