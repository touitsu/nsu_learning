#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>



int* create_edge(int* arr, int size) {
	int lstn = 1;
	for (int i = 0; i < size; i++, lstn++)
		*(arr + i) = lstn;
	for (int i = 1; i < size; i++, lstn++)
		*(arr - 1 + size * (i + 1)) = lstn;
	for (int i = size * size - 2; i > size * (size - 1) - 1; i--, lstn++)
		*(arr + i) = lstn;
	for (int i = 1; i < size - 1; i++, lstn++)
		*(arr + size * (size - 1 - i)) = lstn;
	return arr;
}

int* addlstn(int* arr, int size, int lstn) {
	for (int i = 0; i < size * size; i++) {
		if (*(arr + i) != NULL) {
			*(arr + i) += lstn;
		}
	}
	return arr;
}
int* fill(int* arr, int size, int lstn) {
	int* res = (int*)malloc(sizeof(int) * size * size);
	int layer = 0;
	int* edge = (int*)malloc(sizeof(int) * size - layer * 2);
	for (int i = 0; i < size * size; i++) {
		if (*(edge + i) != NULL) {
			*(res + i) = *(edge + i);
		}
	}
	free(edge);
	while (size > layer*2) {
		int* edge = (int*)malloc(sizeof(int) * size - layer * 2);
		edge = create_edge(edge, size - layer * 2);
		addlstn(edge, lstn);
		for (int j = layer; j < (size - 2 * layer) * (size - 2 * layer); j++) {
			for (int i = layer; i < (size - 2 * layer) * (size - 2 * layer); i++) {
				*(res + size * layer)
			}
		}
		layer ++;
	}
	return res;
}

int main() {
	int n;
	scanf_s("%d", &n);
	int* arr = (int*)malloc(sizeof(int) * n * n);
	arr = fill(arr, n, 1);
	for (int i = 0; i < n*n; i++) {
		printf("%5d ", arr[i]);
		if (i % n == n - 1) {
			printf("\n");
		}
	}
	free(arr);
	return 0;
}