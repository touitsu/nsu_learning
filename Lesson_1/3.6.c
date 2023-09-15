#include <stdio.h>


int main() {
	int i;
	float s = 0.0;
	for (i = 99; i > 0; i--) {
		s += 1 / (s + i * 2 + 3);
	}
	printf("%f", s);
	return 0;
}