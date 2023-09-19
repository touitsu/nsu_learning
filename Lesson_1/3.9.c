#include <stdio.h>
#include <math.h>
#define e 2.7182818284590451


int main() {
	float p0 = 1.29, z = 1.25 * pow(10,-4), p = 1.29, h;
	for (h = 0.0; p >= 1.0; h += 0.00001) {
		p = p0 * pow(e, -h * z);
	}
	printf("%f", h);
	return 0;
}