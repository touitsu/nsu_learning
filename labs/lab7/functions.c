#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include "functions.h"


int8_t readinput(int8_t* arr, int32_t n, int32_t m) {
	char c;
	int32_t from, to;

	for (int32_t i = 0; i < m; i++) {
		from = 0;
		to = 0;
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

		if (c == EOF && (from == 0 || to == 0))
			return 1;

		if (from < 1 || from > n || to < 1 || to > n)
			return 2;

		*(arr + n * (from - 1) + to - 1) = 1;
	}

	return 0;
}


void freelinkedlist(linkedlist* ll) {
	if (ll) {
		freelinkedlist(ll->next);
		free(ll);
	}
}


void insert(linkedlist* root, linkedlist* element) {
	if (root->next == NULL)
		root->next = element;

	else
		insert(root->next, element);

}


void printlinkedlist(linkedlist* root) {
	if (root) {
		printlinkedlist(root->next);
		if (root->n != -1)
			printf("%d ", root->n);
	}
}


void saveexit(int8_t* arr, linkedlist* res, int8_t* status) {
	if (arr)
		free(arr);

	if (status)
		free(status);

	freelinkedlist(res);
	exit(0);
}


void topsort(int8_t* arr, linkedlist* res, int8_t* status, int32_t node, int32_t n) {
	linkedlist* next;

	*(status + node) = 1;
	for (int32_t i = 0; i < n; i++) {
		if (*(arr + node * n + i) == 1 && *(status + i) == 0)
			topsort(arr, res, status, i, n);

		else if (*(arr + node * n + i) == 1 && *(status + i) == 1) {
			printf("impossible to sort");
			saveexit(arr, res, status);
		}
	}


	*(status + node) = 2;
	next = (linkedlist*)malloc(sizeof(linkedlist));

	next->n = node + 1;
	next->next = NULL;

	insert(res, next);
}