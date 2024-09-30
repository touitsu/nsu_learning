#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include "functions.h"


int8_t readinput(vertex * arr, int32_t n, int32_t m) {
	char c;
	int32_t from, to, len;

	for (int32_t i = 0; i < m; i++) {
		from = 0;
		to = 0;
		len = 0;
		c = 0;

		while (c == 0 || c == ' ' || c == '\n')
			c = fgetc(stdin);

		while (c != ' ' && c != '\n' && c != EOF) {
			from *= 10;
			from += c - '0';
			c = fgetc(stdin);
		}

		while (c == 0 || c == ' ' || c == '\n')
			c = fgetc(stdin);

		while (c != ' ' && c != '\n' && c != EOF) {
			to *= 10;
			to += c - '0';
			c = fgetc(stdin);
		}

		while (c == 0 || c == ' ' || c == '\n')
			c = fgetc(stdin);

		while (c != ' ' && c != '\n' && c != EOF) {
			if ((int64_t)(len) * 10 > INT32_MAX)
				return 2;

			len *= 10;
			len += c - '0';
			c = fgetc(stdin);
		}

		if (c == EOF && (from == 0 || to == 0))
			return 1;

		if (from < 1 || from > n || to < 1 || to > n)
			return 3;

		(*(arr + i)).from = from;
		(*(arr + i)).to = to;
		(*(arr + i)).len = len;
	}

	return 0;
}


void saveexit(vertex* arr, vertex* res) {
	free(arr);
	if (res)
		free(res);

	fclose(stdin);
	fclose(stdout);
	exit(0);
}


int32_t arrsum(int8_t* arr, int32_t n) {
	int32_t res;
	
	res = 0;

	for (int32_t i = 0; i < n; i++)
		res += *(arr + i);

	return res;
}


vertex* prim(vertex* arr, int32_t* reslen, int32_t n, int32_t m) {
	int32_t min, minidx;
	int8_t* visited;
	vertex* res;
	
	min = INT32_MAX;
	minidx = 0;
	*reslen = 0;
	res = (vertex*)malloc(sizeof(vertex) * n);
	visited = (int8_t*)malloc(sizeof(int8_t) * n);

	for (int32_t i = 0; i < n; i++)
		*(visited + i) = 0;

	while (arrsum(visited, n) != n && minidx != -1) {
		min = INT32_MAX;
		minidx = -1;

		for (int32_t i = 0; i < m; i++)
			if (((*(visited + (*(arr + i)).to - 1) == 0 && *(visited + (*(arr + i)).from - 1) != 0) ||
				(*(visited + (*(arr + i)).from - 1) == 0 && *(visited + (*(arr + i)).to - 1) != 0) || 
				*reslen == 0) &&
				(*(arr + i)).len <= min) {
				min = (*(arr + i)).len;
				minidx = i;
			}

		if (minidx != -1) {
			*(res + (*reslen)++) = *(arr + minidx);

			*(visited + (*(arr + minidx)).from - 1) = 1;
			*(visited + (*(arr + minidx)).to - 1) = 1;
		}
	}

	if ((arrsum(visited, n) == n && n != 0) || (m == 0 && n == 1)) {
		free(visited);
		return res;
	}

	free(visited);
	return NULL;
}