#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include "functions.h"


uint32_t sumarrnormalized(uint8_t* arr, int32_t n) {
	uint32_t res;

	res = 0;

	for (int32_t i = 0; i < n; i++)
		res += (*(arr + i)) / (*(arr + i));

	return res;
}


void printarr(uint8_t* arr, int32_t n) {
	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < n; j++)
			printf("%d ", *(arr + i * n + j));

		printf("\n");
	}


}



void invertmatrix(uint8_t* arr, int32_t n) {
	uint8_t tmp;
	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = i; j < n; j++) {
			tmp = *(arr + j * n + i);
			*(arr + j * n + i) = *(arr + i * n + j);
			*(arr + i * n + j) = tmp;
		}
	}

}


void getinp(uint8_t* arr, int32_t n, int32_t m) {
	int32_t from, to;

	for (int32_t i = 0; i < m; i++) {
		scanf_s("%d %d", &from, &to);
		*(arr + n * (from - 1) + to - 1) = 1;
	}
}


void dfs1(uint8_t* arr, uint8_t* visited, int32_t n, int32_t m, int32_t node, uint8_t* counter) {
	*(counter) += 1;
	*(visited + node) = *counter;
	for (int32_t i = 0; i < n; i++)
		if (*(arr + n * node + i) == 1 && *(visited + i) == 0)
			dfs1(arr, visited, n, m, i, counter);

	*(visited + node) = *counter;
	*(counter) += 1;
}


void dfs2(uint8_t* arr, uint8_t* res, uint8_t* visited, int32_t node, int32_t n) {
	*(visited + node) = 1;

	for (int32_t i = 0; i < n; i++)
		if (*(arr + node * n + i) == 1 && *(visited + i) == 0) {
			*(res + node * n + i) = 1;
			dfs2(arr, res, visited, i, n);
		}
}


void kosaraju(uint8_t* arr, uint8_t* visited, int8_t* res, int32_t n, int32_t m) {
	uint8_t counter;
	uint8_t* tmpvis;
	int32_t curmax;
	int32_t curi;

	counter = 0;

	for (int32_t i = 0; i < n && sumarrnormalized != n; i++)
		if (*(visited + i) == 0) {
			dfs1(arr, visited, n, m, i, &counter);
			counter--;
		}

	invertmatrix(arr, n);

	tmpvis = (uint8_t*)calloc(n, sizeof(uint8_t));
	curi = 0;

	while (curi != -1) {
		curi = -1;
		curmax = 0;
		for (int32_t i = 0; i < n; i++)
			if (*(visited + i) > curmax && *(tmpvis + i) == 0) {
				curmax = *(visited + i);
				curi = i;
			}

		if (curi != -1)
			dfs2(arr, res, tmpvis, curi, n);
	}

	free(tmpvis);
}