#include <stdio.h>


int main() {
	int k;
	scanf_s("%d", &k);
	printf("%d", k / 3 % 10);
	return 0;
}