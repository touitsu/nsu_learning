#include <stdio.h>
#include <math.h>


int main() {
	int i;
	float s=0.0;
	for (i = 50; i > 0; i--) {
		s = sqrt(s + i);
	}
	printf("%f", s);
	return 0;
}