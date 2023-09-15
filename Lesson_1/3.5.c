#include <stdio.h>
#include <math.h>
#define pi acos(-1.0)


int main() {
	int i;
	float s = 0.0;
	for (i = 0; i < 10; i++) {
		s += pi / 10 * sin(pi / 10 * i + pi / 20);
	}
	printf("%f", s);
	return 0;
}