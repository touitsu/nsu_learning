#include <stdio.h>
#include <math.h>

int csod(int num) {
	if (num == 0)
		return 0;
	return num % 10 + csod(num / 10);
}

int ccr(int num) {
	if (log10(num) <= 1)
		return num;
	return ccr(csod(num));
}

int main() {
	int num;
	scanf_s("%d", &num);
	printf("%d", ccr(num));
	return 0;
}