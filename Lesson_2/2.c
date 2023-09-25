#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


int nod(int a,  int b) {
	if (a != 0 && b != 0) {
		if (a > b) 
			a %= b;
		else 
			b %= a;
		nod(a, b);
	}
	else
		return a + b;
}

int  main() {
	int n, cur_n;
	scanf_s("%d", &n);
	int* nums;
	nums = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		scanf_s("%d", nums + i);
	cur_n = nod(*nums, *(nums + 1));
	for (int i = 2; i < n - 2; i++) 
		cur_n = nod(cur_n, *(nums + 2));
	printf("%d", cur_n);
	free(nums);
	return 0;
}