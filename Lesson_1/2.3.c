#include <stdio.h>
#include <stdlib.h> // I need this library just for linear solution
#include <math.h>	// This is also needed for linear solution


int main() {
	int a, b, c, d;
	scanf_s("%d %d %d %d", &a, &b, &c, &d);
	if ((a / c) * (b / d) >= (a / d) * (b / c)) { //solution with branching
		printf("Long side to long side");
	}
	else {
		printf("Long side to short side");
	}
	printf("\nLong side to %c side", (int)(115 - (int)ceil(((ceil((float)((a / c) * (b / d) - (a / d) * (b / c)))) / ceil((float)(abs((a / c) * (b / d) - (a / d) * (b / c)) + 0.1)))) * 7)); //linear solution bc I can solve it linearly)))
	return 0;
}