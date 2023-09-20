#include <stdio.h>
#include <math.h>


int issimple(int num, int divisor) {
	if (divisor >= ((int)sqrt(num)) + 1)
		return 1;
	else if (num % divisor == 0)
		return 0;
	else
		return issimple(num, divisor + 1);
}

int main() {
	int num;
	scanf_s("%d", &num);
	printf("%d", issimple(num, 2));
	return 0;
}