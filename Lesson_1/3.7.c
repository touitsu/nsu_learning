#include <stdio.h>
#include <math.h>


int main() {
	float x, a = 0.0, b = 1.0;
	while (b-a > 0.00001) {
		x = (b + a) / 2;
		if (signbit(a) != signbit(x)) {
			b = x;
		}
		else {
			a = x;
		}
	}
	printf("%f", x);
	return 0;
}