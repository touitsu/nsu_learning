#include <stdio.h>

int main() {
	int a, b;
	scanf_s("%d", &a);
	scanf_s("%d", &b);
	a += b;
	b = a - b;
	a -= b;
	printf("%d %d", a, b);
	return 0;
}