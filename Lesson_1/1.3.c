#include <stdio.h>


int main() {
	int a1, a2, a3, b1, b2;
	scanf_s("%d %d %d %d %d", &a3, &a2, &a1, &b2, &b1);
	printf("%d%d%d", a3 + (a2 + b2) / 10, (a2 + b2) % 10 + (a1 + b1) / 10, (a1 + b1) % 10);
	return 0;
}