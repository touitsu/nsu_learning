#include <stdio.h>


int main() {
	int i,s = 20*20;
	for (i = 19; i >= 1; i--) {
		s -= i * i;
	}
	printf("%d", s);
	return 0;
}