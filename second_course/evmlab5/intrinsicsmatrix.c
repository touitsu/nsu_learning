#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <time.h>
#include "intrinsicsmatrix.h"
#include <immintrin.h>


int32_t main(int32_t argc, char** argv) {
	float* matrix;
	float* reverseMatrix;
	int32_t n, m;
	clock_t start;

	n = atoi(argv[1]);
	m = atoi(argv[2]);

	matrix = getRandomMatrix(n);
	
	start = clock();
	reverseMatrix = getReverseMatrix(matrix, n, m);
	printf("Runtime: %lf seconds.\n", ((double)(clock() - start)) / CLOCKS_PER_SEC);

	_aligned_free(matrix);
	_aligned_free(reverseMatrix);

	return 0;
}