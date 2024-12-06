#pragma once

#include <stdio.h>
#include <immintrin.h>


float* initMatrix(int32_t n) {
	float* res;
	int32_t nsqr;

	nsqr = (n + (8 - n % 8)) * (n + (8 - n % 8));

	res = (float*)_aligned_malloc(nsqr * sizeof(float), 32);

	for (int32_t i = 0; i < nsqr; i++) {
		*(res + i) = 0;
	}

	return res;
}


float* scanMatrix(int32_t n) {
	float* res;

	res = initMatrix(n);

	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < n; j++) {
			scanf("%f", (res + i * (n + (8 - n % 8)) + j));
		}
	}

	return res;
}


float* getRandomMatrix(int32_t n) {
	float* res;

	res = initMatrix(n);

	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < n; j++) {
			*(res + i * (n + (8 - n % 8)) + j) = rand();
		}
	}

	return res;
}


void printMatrix(float* matrix, int32_t n) {
	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < n; j++) {
			printf("%f ", *(matrix + i * (n + (8 - n % 8)) + j));
		}
		printf("\n");
	}
}


float* getIMatrix(int32_t n) {
	float* res;

	res = initMatrix(n);

	for (int32_t i = 0; i < n; i++) {
		*(res + i * (n + (8 - n % 8)) + i) = 1;
	}

	return res;
}


float* multiplyMatrixByScalar(float* matrix, int32_t n, float scalar) {
	float* res;
	int32_t nsqr;

	nsqr = (n + (8 - n % 8)) * (n + (8 - n % 8));

	res = initMatrix(n);

	for (int32_t i = 0; i < nsqr; i += 8) {
		_mm256_store_ps(res + i, _mm256_mul_ps(_mm256_set1_ps(scalar), _mm256_load_ps(matrix + i)));
	}

	return res;
}


void multiplyMatrixByScalarAndWriteToIt(float* matrix, int32_t n, float scalar) {
	int32_t nsqr;

	nsqr = (n + (8 - n % 8)) * (n + (8 - n % 8));

	for (int32_t i = 0; i < nsqr; i += 8) {
		_mm256_store_ps(matrix + i, _mm256_mul_ps(_mm256_set1_ps(scalar), _mm256_load_ps(matrix + i)));
	}
}


float* sumMatrixWithMatrix(float* matrixA, float* matrixB, int32_t n) {
	float* res;
	int32_t nsqr;

	nsqr = (n + (8 - n % 8)) * (n + (8 - n % 8));

	res = initMatrix(n);

	for (int32_t i = 0; i < nsqr; i += 8) {
		_mm256_store_ps(res + i, _mm256_add_ps(_mm256_load_ps(matrixA + i), _mm256_load_ps(matrixB + i)));
	}

	return res;
}


void sumMatrixWithMatrixAndWriteToFirst(float* matrixA, float* matrixB, int32_t n) {
	int32_t nsqr;

	nsqr = (n + (8 - n % 8)) * (n + (8 - n % 8));

	for (int32_t i = 0; i < nsqr; i += 8) {
		_mm256_store_ps(matrixA + i, _mm256_add_ps(_mm256_load_ps(matrixA + i), _mm256_load_ps(matrixB + i)));
	}
}


float* multiplyMatrixWithMatrix(float* matrixA, float* matrixB, int32_t n) {
	float* res;
	float* tmp1;
	float* tmp2;
	__m256 tmp3;

	res = initMatrix(n);

	for (int32_t i = 0; i < n; i++) {
		tmp1 = res + i * (n + (8 - n % 8));
		for (int32_t k = 0; k < n; k++) {
			tmp2 = matrixB + k * (n + (8 - n % 8));
			tmp3 = _mm256_set1_ps(*(matrixA + i * (n + (8 - n % 8)) + k));
			for (int32_t j = 0; j < (n + (8 - n % 8)); j += 8) {
				_mm256_store_ps(tmp1 + j, _mm256_fmadd_ps(tmp3, _mm256_load_ps(tmp2 + j), _mm256_load_ps(tmp1 + j)));
			}
		}
	}

	return res;
}


void multiplyMatrixWithMatrixAndWriteToFirst(float** matrixA, float* matrixB, int32_t n) {
	float* res;
	float* tmp1;
	float* tmp2;
	__m256 tmp3;

	res = initMatrix(n);

	for (int32_t i = 0; i < n; i++) {
		tmp1 = res + i * (n + (8 - n % 8));
		for (int32_t k = 0; k < n; k++) {
			tmp2 = matrixB + k * (n + (8 - n % 8));
			tmp3 = _mm256_set1_ps(*((*matrixA) + i * (n + (8 - n % 8)) + k));
			for (int32_t j = 0; j < n; j += 8) {
				_mm256_store_ps(tmp1 + j, _mm256_fmadd_ps(tmp3, _mm256_load_ps(tmp2 + j), _mm256_load_ps(tmp1 + j)));
			}
		}
	}

	_aligned_free(*matrixA);

	*matrixA = res;
}


float* getReverseMatrix(float* matrix, int32_t n, int32_t iterations) {
	float* res;
	float* matrixB;
	float* matrixBA;
	float* matrixR;
	float* matrixRExp;
	float* matrixI;
	float* matrixTmp;
	float norm1, normi, tmp;

	norm1 = 0;
	normi = 0;

	for (int32_t i = 0; i < n; i++) {
		tmp = 0;
		for (int32_t j = 0; j < n; j++) {
			tmp += *(matrix + i * (n + (8 - n % 8)) + j);
		}

		tmp > normi ? normi = tmp : tmp;
	}

	for (int32_t j = 0; j < n; j++) {
		tmp = 0;
		for (int32_t i = 0; i < n; i++) {
			tmp += *(matrix + i * (n + (8 - n % 8)) + j);
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

	_aligned_free(matrixB);
	_aligned_free(matrixBA);
	_aligned_free(matrixR);
	_aligned_free(matrixRExp);
	_aligned_free(matrixI);

	return res;
}