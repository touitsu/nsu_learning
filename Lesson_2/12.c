#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


char* getstring() {
	char* arr = (char*)malloc(sizeof(char));
	char c;
	int len = 0;
	c = getchar();
	while (c != '\n') {
		len++;
		arr = (char*)realloc(arr, sizeof(char) * (len + 1));
		if (arr == NULL)
			exit(1);
		*(arr + len - 1) = c;
		c = getchar();
	}
	*(arr + len + 1) = '\0';
	return arr;
}

int* strtointarr(char* arr) {
	int* res = (int*)malloc(sizeof(int));
	int i = 0;
	while (*(arr + i) != '\0') {
		res = realloc(res, sizeof(int) * (i + 2));
		if (res == NULL)
			exit(1);
		*(res + i + 1) = *(arr + i) - 48;
		i++;
		*res = i;
	}
	return res;
}

int* movearray(int* arr, int n) {
	for (int i = *arr - 1; i > n; i--)
		*(arr + i) = *(arr + i - n);
	for (int i = 1; i < n; i++)
		*(arr + i) = 0;
	return arr;
}

int* sum(int* start1, int* start2) {
	int min_len = *start2, max_len = *start1;
	int* res;
	for (int i = 1; i < min_len; i++)
		*(start1 + *start1 - i) = *(start1 + *start1 - i) + *(start2 + *start2 - i);
	res = (int*)malloc(sizeof(int) * (max_len + 1));
	*res = max_len;
	int leftover = 0;
	for (int i = max_len - 1; i > 0; i--) {
		*(start1 + i) += leftover;
		leftover = *(start1 + i) / 10;
		*(start1 + i) %= 10;
		*(res + (*res - max_len) + i) = *(start1 + i);
	}
	if (leftover > 0) {
		res = (int*)realloc(res, sizeof(int) * (*res + 1));
		if (res == NULL)
			exit(1);
		*res += 1;
		movearray(res, 1);
		*(res + 1) = leftover;
	}
	return res;
}

int main() {
	int* inum1 = strtointarr(getstring());
	int* inum2 = strtointarr(getstring());
	int* res;
	if (*inum1 > *inum2) 
		res = sum(inum1, inum2);
	else
		res = sum(inum2, inum1);
	free(inum1);
	free(inum2);
	for (int i = 1; i < *res; i++)
		printf("%d", *(res + i));
	free(res);
	return 0;
}