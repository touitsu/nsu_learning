#include <stdio.h>


int main() {
	float t;
	scanf_s("%f", &t);
	if ((int)(t) % 10 == 3 || (int)(t) % 10 == 4 || (int)(t) % 10 == 8 || (int)(t) % 10 == 9) {
		printf("Red light.");
	}
	else {
		printf("Green light.");
	}
	return 0;
}