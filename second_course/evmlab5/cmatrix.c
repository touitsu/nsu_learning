#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <time.h>
#include "cmatrix.h"


int32_t main(int32_t argc, char** argv) {
	float** matrix;
	float** reverseMatrix;
	float** multRes;
	int32_t n, m;
	clock_t start;

	n = atoi(argv[1]);
	m = atoi(argv[2]);

	matrix = getRandomMatrix(n);

	start = clock();
	reverseMatrix = getReverseMatrix(matrix, n, m);

	printf("Runtime: %lf seconds.\n", ((double)(clock() - start))/CLOCKS_PER_SEC);

	freeMatrix(matrix, n);
	freeMatrix(reverseMatrix, n);

	return 0;
}