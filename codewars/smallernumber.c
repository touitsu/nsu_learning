#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

long long next_smaller_number(unsigned long long n) {
	int* num = (int*)malloc(sizeof(int));
	int len = 0;
	while (n > 0) {
		*(num + len) = (int)(n % 10);
		n /= 10;
		len++;
		num = (int*)realloc(num, sizeof(int) * (len + 1));
		if (num == NULL)
			exit(1);
	}
	int tmp, i = 0, is_lowered = 0;
	while ((i < len - 1) && !is_lowered){
		if ((*(num + i) < *(num + i + 1)) && !(i == len-2 && *(num + i) == 0)) {
			tmp = *(num + i);
			*(num + i) = *(num + i + 1);
			*(num + i + 1) = tmp;
			is_lowered = 1;
		}
		i++;
	}
	if (is_lowered) {
		unsigned long long int new_n = 0;
		for (int i = 0; i < len; i++) {
			new_n += *(num + i) * pow(10, i);
		}
		return new_n;
	}
	return -1;
}

int main() {
	unsigned long long int number;
	scanf_s("%lld", &number);
	next_smaller_number(number);
	return 0;
}