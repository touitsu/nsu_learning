#include <stdio.h>


int csod(int num) {
	if (num == 0) 
		return 0;
	return num % 10 + csod(num / 10);
}

int main() {
	int num;
	scanf_s("%d", &num);
	printf("%d", csod(num));
	return 0;
}