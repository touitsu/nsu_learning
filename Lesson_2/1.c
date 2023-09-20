#include <stdio.h>
#include <malloc.h>
#include <math.h>


int check(int a) {
	if (pow((int)sqrt(a), 2) == a) 
		return 1;
	return 0;
	
}

int main() {
	int n, res = 0;
	scanf_s("%d", &n);
	int* nums;
	nums = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++) 
		scanf_s("%d",nums + i);
	for (int i = 0; i < n; i++) 
		res += check(*(nums + i));
	printf("%d", res);
	free(nums);
	return 0;
}