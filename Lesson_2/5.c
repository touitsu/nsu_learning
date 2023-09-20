#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>


float lenofside(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

float gaussarea(float* start, float* end) { //tak-to eto fignya tochnee, tak kak ne uzaet sqrt, no tochki nado vvodit` posledovatel`no
	float area = 0.0;
	for (int i = 0; i < end - start; i += 2) {
		if (i + 3 > end - start) 
			area += (*(start + i)) * (*(start + 1));
		
		else 
			area += (*(start + i)) * (*(start + i + 3));
	}
	for (int i = 1; i < end - start; i += 2) {
		if (i + 3 > end - start)
			area -= (*(start + i)) * (*(start));

		else
			area -= (*(start + i)) * (*(start + i + 1));
	}
	return abs(area) * 0.5;
}

float geronarea(float* start, float* end) {
	float area = 0.0;
	for (int i = 0; i < (end - start)/2 - 2; i++) {
		float a = lenofside(*start, *(start + 1), *(start + 2 + (i * 2)), *(start + 3 + (i * 2)));
		float b = lenofside(*(start + 2 + (i * 2)), *(start + 3 + (i * 2)), *(start + 4 + (i * 2)), *(start + 5 + (i * 2)));
		float c = lenofside(*start, *(start + 1), *(start + 4 + (i * 2)), *(start + 5 + (i * 2)));
		float p =  (a+b+c)/2;
		area += sqrt(p * (p - a) * (p - b) * (p - c));
	}
	return area;
}

int main() {
	int nofsides;
	scanf_s("%d", &nofsides);
	float* coords;
	coords = (float*)malloc(sizeof(float) * (nofsides * 2));
	for (int i = 0; i < nofsides * 2; i+=1)
		scanf_s("%f", coords + i);
	printf("\n%f", geronarea(coords, coords + nofsides * 2));
	free(coords);
	return 0;
}