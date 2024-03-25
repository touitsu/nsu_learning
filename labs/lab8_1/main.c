#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include "functions.h"


int32_t main() {
	int32_t n, m, tmp, reslen;
	vertex* arr;
	vertex* res;

	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);

	scanf_s("%d\n", &n);
	scanf_s("%d\n", &m);

	arr = (vertex*)malloc(sizeof(vertex) * m);
	res = NULL;

	if (n < 0 || n > 5000) {
		printf("bad number of vertices\n");
		saveexit(arr, res);
	}

	if (m < 0 || m > n * (n - 1) / 2) {
		printf("bad number of edges\n");
		saveexit(arr, res);
	}

	tmp = readinput(arr, n, m);
	if (tmp != 0) {
		if (tmp == 1)
			printf("bad number of lines\n");

		else if (tmp == 2)
			printf("bad length\n");

		else if (tmp == 3)
			printf("bad vertex\n");

		saveexit(arr, res);
	}

	res = prim(arr, &reslen, n, m);

	if (!res) {
		printf("no spanning tree\n");
		saveexit(arr, res);
	}

	for (int32_t i = 0; i < reslen; i++)
		printf("%d %d\n", (*(res + i)).from, (*(res + i)).to);

	saveexit(arr, res);
	return 0;
}