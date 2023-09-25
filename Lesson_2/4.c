#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


int max(int a, int b) {
	if (a > b) 
		return a;
	return b;
}

int countneven(int* start, int* end) {
	int res = 0, m = 0;
	for (int* i = start; i < end; i++) {
		if(*(i)%2 == 0){
			m = max(res, m);
			res = 0;
		}
		else 
			res ++;
	}
	m = max(res, m);
	return m;
}

int main() {
	int n;
	scanf_s("%d", &n);
	int* nums;
	nums = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++) 
		scanf_s("%d", nums + i);
	printf("%d", countneven(nums, nums + n));
	free(nums);
	return 0;
}