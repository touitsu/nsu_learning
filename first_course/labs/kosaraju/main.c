#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include "functions.h"


int32_t main() {
	uint8_t* arr;
	uint8_t* visited;
	int8_t* res;
	int32_t n, m;

	scanf_s("%d\n%d", &n, &m);

	arr = (uint8_t*)calloc(n*n, sizeof(uint8_t));
	visited = (uint8_t*)calloc(n, sizeof(uint8_t));
	res = (int8_t*)calloc(n * n, sizeof(int8_t));

	getinp(arr, n, m);

	kosaraju(arr, visited, res, n, m);

	printarr(res, n);

	free(arr);
	free(visited);
	free(res);
	return 0;
}