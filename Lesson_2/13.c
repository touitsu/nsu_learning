#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>


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

int* fact(int* arr) {
	int n = 0;
	for (int i = *arr - 1; i > 0; i--)
		n += *(arr + i) * pow(10, *arr - i - 1);
	for (int i = n - 1; i > 1; i--) {
		for (int j = *arr - 1; j > 0; j--)
			*(arr + j) *= i;
		int leftover = 0;
		for (int j = *arr - 1; j > 0; j--) {
			*(arr + j) += leftover;
			leftover = *(arr + j) / 10;
			*(arr + j) %= 10;
		}
		if (leftover > 0) {
			*arr += (int)log10(leftover) + 1;
			arr = (int*)realloc(arr, sizeof(int) * *arr);
			if (arr == NULL)
				exit(1);
			movearray(arr, (int)log10(leftover) + 1);
			*(arr + (int)log10(leftover) + 1) = leftover;
			leftover = 0;
			for (int j = *arr - 1; j > 0; j--) {
				*(arr + j) += leftover;
				leftover = *(arr + j) / 10;
				*(arr + j) %= 10;
			}
		}
	}
	if (n == 0)
		*(arr + 1) = 1;
	return arr;
}

int main() {
	int* inum = strtointarr(getstring());
	inum = fact(inum);
	for (int i = 1; i < *inum; i++)
		printf("%d", *(inum + i));
	free(inum);
	return 0;
}