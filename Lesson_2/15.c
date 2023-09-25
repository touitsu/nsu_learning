#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


int* fill(int* arr, int size) {
	int layer = 0;
	int lstn = 1;
	while (size > layer * 2) {
		for (int i = layer; i < size - layer; i++, lstn ++)
			*(arr + layer * size + i) = lstn;
		for (int i = layer + 1; i < size - layer; i++, lstn++)
			*(arr + i * size + size - layer - 1) = lstn;
		for (int i = size - layer - 2; i > layer; i--, lstn++)
			*(arr + size * (size - layer - 1) + i) = lstn;
		for (int i = size - layer - 1; i > layer; i--, lstn++)
			*(arr + i * size + layer) = lstn;
		layer ++;
	}
	return arr;
}

int main() {
	int n;
	scanf_s("%d", &n);
	int* arr = (int*)malloc(sizeof(int) * n * n);
	arr = fill(arr, n);
	for (int i = 0; i < n*n; i++) {
		printf("%5d ", *(arr + i));
		if (i % n == n - 1) {
			printf("\n");
		}
	}
	free(arr);
	return 0;
}