#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include "functions.h"

int32_t main() {
	TREE* tr;
	int32_t n, tmp;

	tr = NULL;

	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);

	scanf_s("%d\n", &n);

	for (int32_t i = 0; i < n; i++) {
		scanf_s("%d ", &tmp);

		tr = insert(tr, tmp);
	}

	printf("%d", getheight(tr));

	freetree(tr);

	fclose(stdin);
	fclose(stdout);

	return 0;
}