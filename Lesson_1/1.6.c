#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main() {
	int h, m;
	scanf_s("%d", &h);
	scanf_s("%d", &m);
	printf("%d", ((h % 12 * 5 + 15 * (h % 12 * 5 + 15 - m) / abs((h % 12 * 5 + 15 - m))) + 60 - m) % 60);
	return 0;
}