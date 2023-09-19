#include <stdio.h>
#include <malloc.h>
#include <math.h>


float lenofside(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

float gaussarea(float* start, float* end) {
	float area = 0.0;
	int sign = 1;
	for (int i = 0; i < (end - start - 1); i += 2) {
		area = (*(start + i) * (*(start + i + 3))) * sign;
		sign *= -1;
	}
}

int main() {
	int nofsides;
	scanf_s("%d", &nofsides);
	float* coords;
	coords = (float*)malloc(sizeof(float) * nofsides * 2 + sizeof(float) * 2);
	for (int i = 0; i < nofsides * 2; i+=1) {
		scanf_s("%f", coords + i);
	}
	*(coords + nofsides * 2 + 1) = *(coords);
	*(coords + nofsides * 2 + 2) = *(coords + 1);
	printf("%f", gaussarea(coords, coords + nofsides * 2 + 2));
	return 0;
}