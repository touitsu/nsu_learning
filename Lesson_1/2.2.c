#include <stdio.h>


int min(int a, int b) {
	if (a > b) {
		return b;
	}
	else {
		return a;
	}
}

int max(int a, int b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}

int main() {
	int x1, y1, x2, y2, x3, y3, x4, y4;
	scanf_s("%d %d %d %d %d %d %d %d", &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4);
	printf("(%d,%d) (%d,%d)", min(x1, x3), min(y1, y3), max(x2, x4), max(y2, y4));
	return 0;
}