#include <stdio.h>
#include <math.h>


int issimple(int num, int divisor) {
	if (divisor >= ((int)sqrt(num)) + 1)
		return 1;
	else if (num % divisor == 0)
		return 0;
	return issimple(num, divisor + 1);
}

int main() {
	int num;
	scanf_s("%d", &num);
	if (issimple(num, 2))
		printf("Number %d is simple.", num);
	else
		printf("Number %d is not simple.", num);
	return 0;
}