#include <stdio.h>
#include <math.h>

int main() {
	int a, b = 0, i;
	scanf_s("%d", &a);
	int n = (int)log10(a) + 1;
	for (i = 0; i <= n; i++) {
		b += a % 10 * (int)pow(10, n - i - 1);
		a /= 10;
	}
	printf("%d", b);
	return 0;
}