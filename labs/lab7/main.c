#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include "functions.h"


int32_t main() {
	int32_t n, m, tmp;
	int8_t* arr;
	linkedlist* res;
	int8_t* status;

	arr = NULL;
	res = NULL;
	status = NULL;
	m = -1;
	n = -1;

	scanf_s("%d", &n);

	if (n < 0 || n > 2000) {
		printf("bad number of vertices");
		saveexit(arr, res, status);
	}

	scanf_s("%d", &m);

	if (m == -1) {
		printf("bad number of lines");
		saveexit(arr, res, status);
	}

	if (m < 0 || m > n * (n + 1)/2) {
		printf("bad number of edges");
		saveexit(arr, res, status);
	}

	arr = (int8_t*)malloc(sizeof(int8_t) * n * n);
	res = (linkedlist*)malloc(sizeof(linkedlist));
	status = (int8_t*)malloc(sizeof(int8_t) * n);
	
	for (int8_t i = 0; i < n * n; i++)
		*(arr + i) = 0;

	for (int32_t i = 0; i < n; i++)
		*(status + i) = 0;

	res->n = -1;
	res->next = NULL;

	tmp = readinput(arr, n, m);

	if (tmp == 1) {
		printf("bad number of lines");
		saveexit(arr, res, status);
	}

	else if (tmp == 2) {
		printf("bad vertex");
		saveexit(arr, res, status);
	}

	for (int32_t i = 0; i < n; i++)
		if (*(status + i) == 0)
			topsort(arr, res, status, i, n);

	printlinkedlist(res);

	free(arr);
	free(status);
	freelinkedlist(res);
	return 0;
}