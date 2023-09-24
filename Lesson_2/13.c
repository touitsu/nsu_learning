#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>


int* strtointarr(char* start, char* end) {
	int len = (int)strlen(start);
	int* res = (int*)malloc(sizeof(int) * (len + 1));
	*res = len + 1;
	for (int i = 0; i < len; i++)
		*(res + i + 1) = *(start + i) - 48;
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
			arr = realloc(arr, sizeof(int) * *arr);
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
	return arr;
}

int main() {
	char* num = malloc(sizeof(char) * 100);
	scanf_s("%99s", num, 100);
	int* inum = strtointarr(num, num + 100);
	free(num);
	inum = fact(inum);
	if(*inum == 2 && *(inum+1) == 0){
		printf("1");
		return 0;
	}
	for (int i = 1; i < *inum; i++)
		printf("%d", *(inum + i));
	free(inum);
	return 0;
}