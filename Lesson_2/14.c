#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


int** fill(int** arr, int size, int layer, int lstn) {
	for (int i = layer; i < size - layer; i++, lstn++)
		arr[layer][i] = lstn;	
	for (int i = 1 + layer; i < size - layer; i++, lstn++)
		arr[i][size - layer - 1] = lstn;
	for (int i = size - layer - 2; i > layer; i--, lstn++)
		arr[size - layer - 1][i] = lstn;
	for (int i = size - layer - 1; i > layer; i--, lstn++)
		arr[i][layer] = lstn;
	if (lstn >= size * size)
		return arr;
	return (fill(arr, size, layer + 1, lstn));
}

int main() {
	int n;
	scanf_s("%d", &n);
	int** arr = (int**)malloc(sizeof(int*) * n);
	for (int i = 0; i < n; i++)
		arr[i] = (int*)malloc(sizeof(int) * n);
	arr = fill(arr, n, 0, 1);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%5d ", arr[i][j]);
		}
		printf("\n");
	}
	for (int i = 0; i < n; i++)
		free(*(arr + i));
	free(arr);
	return 0;
}