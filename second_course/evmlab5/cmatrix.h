#pragma once

#include <stdio.h>
#include <stdlib.h>


float** getRandomMatrix(int32_t n) {
	float** res;

	res = (float**)malloc(sizeof(float*) * n);

	for (int32_t i = 0; i < n; i++) {
		*(res + i) = (float*)malloc(sizeof(float) * n);
	}

	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < n; j++) {
			*(*(res + i) + j) = ((float)rand()) / 10;
		}
	}

	return res;
}

float** initMatrix(int32_t n) {
	float** res;

	res = (float**)malloc(sizeof(float*) * n);

	for (int32_t i = 0; i < n; i++) {
		*(res + i) = (float*)malloc(sizeof(float) * n);
	}

	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < n; j++) {
			*(*(res + i) + j) = 0;
		}
	}

	return res;
}


void freeMatrix(float** matrix, int32_t n) {
	for (int32_t i = 0; i < n; i++) {
		free(*(matrix + i));
	}
	free(matrix);
}


float** scanMatrix(int32_t n) {
	float** res;

	res = initMatrix(n);

	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < n; j++) {
			scanf("%f", (*(res + i) + j));
		}
	}

	return res;
}


float** getIMatrix(int32_t n) {
	float** res;

	res = initMatrix(n);

	for (int32_t i = 0; i < n; i++) {
		res[i][i] = 1;
	}

	return res;
}


void printMatrix(float** matrix, int32_t n) {
	for (int32_t i = 0; i < n; i++) {
		printf("\n");
		for (int32_t j = 0; j < n; j++) {
			printf("%f ", *(*(matrix + i) + j));
		}
	}
	printf("\n");
}


float** multiplyMatrixWithMatrix(float** matrixA, float** matrixB, int32_t n) {
	float** res;
	float* tmp1;
	float* tmp2;
	float tmp3;

	res = initMatrix(n);

	for (int32_t i = 0; i < n; i++) {
		tmp1 = *(res + i);
		for (int32_t k = 0; k < n; k++) {
			tmp2 = *(matrixB + k);
			tmp3 = *(*(matrixA + i) + k);
			for (int32_t j = 0; j < n; j++) {
				*(tmp1 + j) += tmp3 * *(tmp2 + j);
			}
		}
	}

	return res;
}


void multiplyMatrixWithMatrixAndWriteToFirst(float*** matrixA, float** matrixB, int32_t n) {
	float** res;
	float* tmp1;
	float* tmp2;
	float tmp3;

	res = initMatrix(n);

	for (int32_t i = 0; i < n; i++) {
		tmp1 = *(res + i);
		for (int32_t k = 0; k < n; k++) {
			tmp2 = *(matrixB + k);
			tmp3 = *(*((*matrixA) + i) + k);
			for (int32_t j = 0; j < n; j++) {
				*(tmp1 + j) += tmp3 * *(tmp2 + j);
			}
		}
	}

	freeMatrix(*matrixA, n);

	*matrixA = res;
}


float** multiplyMatrixByScalar(float** matrix, int32_t n, float scalar) {
	float** res;

	res = initMatrix(n);

	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < n; j++) {
			*(*(res + i) + j) = (*(*(matrix + i) + j)) * scalar;
		}
	}

	return res;
}


void multiplyMatrixByScalarAndWriteToIt(float** matrix, int32_t n, float scalar) {
	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < n; j++) {
			*(*(matrix + i) + j) = (*(*(matrix + i) + j)) * scalar;
		}
	}
}


float** sumMatrixWithMatrix(float** matrixA, float** matrixB, int32_t n) {
	float** res;

	res = initMatrix(n);

	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < n; j++) {
			*(*(res + i) + j) = *(*(matrixA + i) + j) + *(*(matrixB + i) + j);
		}
	}

	return res;
}


void sumMatrixWithMatrixAndWriteToFirst(float** matrixA, float** matrixB, int32_t n) {
	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < n; j++) {
			*(*(matrixA + i) + j) = *(*(matrixA + i) + j) + *(*(matrixB + i) + j);
		}
	}
}


float** getReverseMatrix(float** matrix, int32_t n, int32_t iterations) {
	float** res;
	float** matrixB;
	float** matrixBA;
	float** matrixR;
	float** matrixRExp;
	float** matrixI;
	float norm1, normi, tmp;

	norm1 = 0;
	normi = 0;

	for (int32_t i = 0; i < n; i++) {
		tmp = 0;
		for (int32_t j = 0; j < n; j++) {
			tmp += *(*(matrix + i) + j);
		}

		tmp > normi ? normi = tmp : tmp;
	}

	for (int32_t j = 0; j < n; j++) {
		tmp = 0;
		for (int32_t i = 0; i < n; i++) {
			tmp += *(*(matrix + i) + j);
		}

		tmp > norm1 ? norm1 = tmp : tmp;
	}

	matrixB = multiplyMatrixByScalar(matrix, n, 1 / (normi * norm1));


	matrixBA = multiplyMatrixWithMatrix(matrixB, matrix, n);
	multiplyMatrixByScalarAndWriteToIt(matrixBA, n, -1);

	matrixI = getIMatrix(n);

	matrixR = sumMatrixWithMatrix(matrixI, matrixBA, n);

	matrixRExp = initMatrix(n);

	sumMatrixWithMatrixAndWriteToFirst(matrixRExp, matrixR, n);

	res = sumMatrixWithMatrix(matrixI, matrixR, n);

	for (int32_t i = 1; i < iterations; i++) {
		multiplyMatrixWithMatrixAndWriteToFirst(&matrixRExp, matrixR, n);
		sumMatrixWithMatrixAndWriteToFirst(res, matrixRExp, n);
	}

	multiplyMatrixWithMatrixAndWriteToFirst(&res, matrixB, n);

	freeMatrix(matrixB, n);
	freeMatrix(matrixBA, n);
	freeMatrix(matrixR, n);
	freeMatrix(matrixRExp, n);
	freeMatrix(matrixI, n);

	return res;
}