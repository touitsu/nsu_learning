#include <stdio.h>
#include <malloc.h>


int findmax(int* start, int* end, int res) {
	if (start == end)
		return res;
	if (*start > res)
		return findmax(start + 1, end, *start);
	return findmax(start + 1, end, res);
}

int main() {
	int n;
	scanf_s("%d", &n);
	int* nums;
	nums = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++) 
		scanf_s("%d", nums + i);
	printf("%d", findmax(nums, nums + n, 0));
	free(nums);
	return 0;
}
