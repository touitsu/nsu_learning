#include <stdio.h>
#include <malloc.h>


int isinarr(int* start, int* end, int num) {
	for (int i = 1; i < end - start; i++) {
		if (*(start + i) == num)
			return 1;
	}
	return 0;
}

int* cleararr(int* start, int* end) {
	int* res = (int*)malloc(sizeof(int) * 1);
	*res = 1;
	for (int i = 0; i < end - start; i++) {
		if (!(isinarr(res, res + *res, *(start + i)))) {
			res = realloc(res, sizeof(int) * (1+*res));
			*(res + *res) = *(start + i);
			*res = *res + 1;
		}
	}
	return res;
}

int main() {
	int n;
	scanf_s("%d", &n);
	int* nums = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		scanf_s("%d", nums + i);
	nums = cleararr(nums, nums + n);
	for (int i = 1; i < *nums; i++)
		printf("%d\n", *(nums + i));
	free(nums);
	return 0;
}