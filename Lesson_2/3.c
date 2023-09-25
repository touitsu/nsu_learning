#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


void fillrnd(int* start, int* end) {
	for (int* i = start; i < end; i++) 
		*i = rand();	
}

int main() {
	int n;
	scanf_s("%d", &n);
	int* nums;
	nums = (int*)malloc(sizeof(int) * n);
	fillrnd(nums, nums + n);
	for (int i = 0; i < n; i++) 
		printf("%d\n", *(nums + i));
	free(nums);
	return 0;
}