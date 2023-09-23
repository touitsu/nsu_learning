#include <stdio.h>
#include <malloc.h>
#include <string.h>


int max(int a, int b) {
	if (a > b)
		return a;
	return b;
}

int* strtointarr(char* start, char* end) {
	int len = (int)strlen(start);
	int* res = (int*)malloc(sizeof(int) * (len + 1));
	*res = len+1;
	for (int i = 0; i < len; i++)
		*(res + i + 1) = *(start + i) - 48;
	return res;
}

int* sum(int* start1, int* start2) {
	int min_len = *start2, max_len = *start1;
	int* res;
	for (int i = 1; i < min_len; i++)
		*(start1 + *start1 - i) = *(start1 + *start1 - i) + *(start2 + *start2 - i);
	if (*(start1 + 1) >= 9) {
		res = (int*)malloc(sizeof(int) * (max_len + 2));
		*res = max_len + 1;
	}
	else {
		res = (int*)malloc(sizeof(int) * (max_len + 1));
		*res = max_len;
	}
	int leftover = 0;
	for (int i = max_len - 1; i > 0; i--) {
		*(start1 + i) += leftover;
		leftover = *(start1 + i) / 10;
		*(start1 + i) %= 10;
		*(res + (*res - max_len) + i) = *(start1 + i);
	}
	if (*res > max_len)
		*(res + 1) = leftover;
	return res;
}

int main() {
	char* num1 = malloc(sizeof(char) * 100);
	char* num2 = malloc(sizeof(char) * 100);
	scanf_s("%99s", num1, 100);
	scanf_s("%99s", num2, 100);
	int* inum1 = strtointarr(num1, num1 + 100);
	int* inum2 = strtointarr(num2, num2 + 100);
	free(num1);
	free(num2);
	int* res;
	if (max(*inum1, *inum2) == *inum1) 
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