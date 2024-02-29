#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include "functions.h"


void dijkstra(int32_t n, int32_t cur, int64_t* minplens, int32_t* matrix, int8_t* visited, int32_t* previous) {
	int32_t curmin, curminidx;

	for (int32_t i = 0; i < n; i++) {
		if (*(matrix + cur * n + i) != -1) {
			if (*(matrix + cur * n + i) + *(minplens + cur) < *(minplens + i)) {
				*(minplens + i) = *(matrix + cur * n + i) + *(minplens + cur);
				*(previous + i) = cur;
			}
		}
	}
	*(visited + cur) = 1;
	
	curmin = INT32_MAX;
	curminidx = INT32_MAX;

	for (int32_t i = 0; i < n; i++) {
		if (*(visited + i) == 0) {
			if (*(minplens + i) <= curmin) {
				curmin = *(minplens + i);
				curminidx = i;
			}
		}
	}

	if (curminidx != INT32_MAX)
		dijkstra(n, curminidx, minplens, matrix, visited, previous);
}


int32_t main() {
	int32_t n, s, f, m, a, b, overflowcntr;
	int32_t* matrix;
	int64_t* minplens;
	int32_t* previous;
	int8_t* visited;
	int64_t tmp;

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

	matrix = (int32_t*)malloc(sizeof(int32_t) * n * n);

	for (int32_t i = 0; i < n * n; i++)
		*(matrix + i) = -1;

	for (int32_t i = 0; i < m; i++) {
		scanf_s("%d %d %lld", &a, &b, &tmp);

		if (tmp < 0 || tmp > INT32_MAX) {
			printf("bad length");
			free(matrix);
			return 0;
		}

		*(matrix + (a - 1) * n + (b - 1)) = tmp;
		*(matrix + (b - 1) * n + (a - 1)) = tmp;
	}

	minplens = (int64_t*)malloc(sizeof(int64_t) * n);
	
	for (int32_t i = 0; i < n; i++)
		*(minplens + i) = INT64_MAX;

	*(minplens + s - 1) = 0;

	visited = (int8_t*)malloc(sizeof(int8_t) * n);

	for (int32_t i = 0; i < n; i++)
		*(visited + i) = 0;

	previous = (int32_t*)malloc(sizeof(int32_t) * n);

	for (int32_t i = 0; i < n; i++)
		*(previous + i) = -1;
	
	dijkstra(n, s - 1, minplens, matrix, visited, previous);

	overflowcntr = 0;

	for (int32_t i = 0; i < n; i++) {
		if (*(minplens + i) == INT64_MAX)
			printf("oo ");
		else if (*(minplens + i) > INT32_MAX) {
			overflowcntr++;
			printf("INT_MAX+ ");
		}
		else
			printf("%d ", *(minplens + i));
	}

	printf("\n");

	if (*(minplens + f - 1) == INT64_MAX)
		printf("no path");
	else if (*(minplens + f - 1) > INT32_MAX && overflowcntr > 1)
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