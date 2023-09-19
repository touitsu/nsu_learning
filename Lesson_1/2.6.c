#include <stdio.h>
#include <stdlib.h>


int main() {
	int a, b, c, d, e, f;
	scanf_s("%d %d %d %d %d %d", &a, &b, &c, &d, &e, &f);
	if ((abs(e - a) == 2 && abs(f - b) == 1) || (abs(e - a) == 1 && abs(f - b) == 2)) {
		if (e == c && f == d) {
			printf("Valid move.\n");
		}
		else if (e == c || f == d || (abs(e - c) / abs(f - d) == 1)) {
			printf("The knight is threatened.\n");
		}
		else {
			printf("Valid move.\n");
		}
	}
	else {
		printf("Invalid move.\n");
	}
	return 0;
}