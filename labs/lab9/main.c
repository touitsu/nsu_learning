#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include "functions.h"


int32_t main() {
	int32_t n, s, f, m, a, b;
	int8_t ovflcnt;
	int32_t* matrix;
	int32_t* minplens;
	int32_t* previous;
	int8_t* visited;
	int32_t tmp;

	scanf_s("%d", &n);
	scanf_s("%d %d", &s, &f);
	scanf_s("%d", &m);

	if (n < 0 || n > 5000) {
		printf("bad number of vertices");
		return 0;
	}

	if (m < 0 || m > n * (n + 1) / 2) {
		printf("bad number of edges");
		return 0;
	}

	if ((s < 1 || s > n) || (f < 1 || f > n)) {
		printf("bad vertex");
		return 0;
	}

	matrix = initarr(-1, n * n);

	for (int32_t i = 0; i < m; i++) {
		scanf_s("%d %d %d", &a, &b, &tmp);

		if (tmp < 0) {
			printf("bad length");
			free(matrix);
			return 0;
		}

		*(matrix + (a - 1) * n + (b - 1)) = tmp;
		*(matrix + (b - 1) * n + (a - 1)) = tmp;
	}

	minplens = initarr(-1, n);
	
	*(minplens + s - 1) = 0;

	visited = (int8_t*)malloc(sizeof(int8_t) * n);

	for (int32_t i = 0; i < n; i++)
		*(visited + i) = 0;

	previous = (int32_t*)malloc(sizeof(int32_t) * n);

	for (int32_t i = 0; i < n; i++)
		*(previous + i) = i;

	ovflcnt = 0;
	
	dijkstra(n, s - 1, minplens, matrix, visited, previous, &ovflcnt);


	for (int32_t i = 0; i < n; i++) {
		if (*(minplens + i) == -1)
			printf("oo ");
		else if (*(minplens + i) == INT32_MIN) {
			printf("INT_MAX+ ");
		}
		else
			printf("%d ", *(minplens + i));
	}

	printf("\n");

	if (*(minplens + f - 1) == -1)
		printf("no path");
	else if (ovflcnt > 1 && (int64_t)(*(matrix + s*n + f)) && shrtpth(n, f, s, previous, matrix) == INT32_MIN)
		printf("overflow");
	else {
		printf("%d ", f);
		for (int32_t i = *(previous + f - 1); i != s - 1; i = *(previous + i)) {
			printf("%d ", i + 1);
		}
		printf("%d\n", s);
	}

	free(matrix);
	free(minplens);
	free(previous);
	free(visited);

	return 0;
}