#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


int issymmetric(int* start, int* end) {
	if (start == end || end < start)
		return 1;
	else if (*start != *end)
		return 0;
	return issymmetric(start + 1, end - 1);
}

int main() {
	int n;
	scanf_s("%d", &n);
	int* nums;
	nums = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		scanf_s("%d", nums + i);
	int i, j;
	scanf_s("%d %d", &i, &j);
	if (issymmetric(nums + i - 1, nums + j - 1))
		printf("This part of array is symmetric.");
	else
		printf("This part of array is not symmetric.");
	free(nums);
	return 0;
}
