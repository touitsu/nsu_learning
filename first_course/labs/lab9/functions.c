#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include "functions.h"

void dijkstra(int32_t n, int32_t cur, int32_t* minplens, int32_t* matrix, int8_t* visited, int32_t* previous, int8_t* ovflcnt) {
	int32_t curmin, curminidx;

	for (int32_t i = 0; i < n; i++) {
		if (*(matrix + cur * n + i) != -1 && *(visited + i) == 0) {
			if ((int64_t)(*(matrix + cur * n + i)) + (int64_t)(*(minplens + cur)) > INT32_MAX && *(minplens + i) < 0) {
				*(minplens + i) = INT32_MIN;
				*(ovflcnt) = *(ovflcnt)+1;
				*(previous + i) = cur;
			}
			else if ((int64_t)(*(matrix + cur * n + i) + (int64_t)(*(minplens + cur))) < (int64_t)(*(minplens + i)) || (*(minplens + i) < 0)) {
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
			if (*(minplens + i) + INT32_MAX < curmin && *(minplens + i) != -1) {
				curmin = *(minplens + i) + INT32_MAX;
				curminidx = i;
			}
		}
	}

	if (curminidx != INT32_MAX)
		dijkstra(n, curminidx, minplens, matrix, visited, previous, ovflcnt);
}


int32_t shrtpth(int32_t n, int32_t f, int32_t s, int32_t* previous, int32_t* matrix) {
	int32_t i, res;

	i = f - 1;
	res = 0;

	while (i != s - 1) {
		if ((int64_t)res + (int64_t)(*(matrix + i * n + *(previous + i))) > INT32_MAX)
			return INT32_MIN;
		res += *(matrix + i * n + *(previous + i));
		i = *(previous + i);
	}
}


int32_t* initarr(int32_t defnum, int32_t n) {
	int32_t* arr;

	arr = (int32_t*)malloc(sizeof(int32_t) * n * n);

	for (int32_t i = 0; i < n; i++)
		*(arr + i) = -1;

	return arr;
}