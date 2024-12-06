#include <stdio.h>
#include <cblas.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>


float* initMatrix(int32_t n) {
    float* res;

    res = (float*)malloc(sizeof(float) * n * n);

    for (int32_t i = 0; i < n * n; i++) {
        *(res + i) = 0;
    }

    return res;
}


void printMatrix(float* matrix, int32_t n) {
    for (int32_t i = 0; i < n; i++) {
        for (int32_t j = 0; j < n; j++) {
            printf("%f ", *(matrix + i * n + j));
        }
        printf("\n");
    }
}


float* getRandomMatrix(int32_t n) {
    float* res;

    res = initMatrix(n);

    for (int32_t i = 0; i < n * n; i++) {
        *(res + i) = rand();
    }

    return res;
}


float* getIMatrix(int32_t n) {
    float* res;

    res = initMatrix(n);

    for (int32_t i = 0; i < n; i++) {
        *(res + i * n + i) = 1;
    }

    return res;
}


float* initMatrixFromMatrix(float* matrix, int32_t n) {
    float* res;

    res = initMatrix(n);

    for (int32_t i = 0; i < n * n; i++) {
        *(res + i) = *(matrix + i);
    }

    return res;
}


float* getReverseMatrix(float* matrix, int32_t n, int32_t iterations) {
    float* res;
    float* restmp;
    float* matrixB;
    float* matrixBA;
    float* matrixR;
    float* matrixRExp;
    float* matrixRExptmp;
    float* matrixI;
    float norm1, normi, tmp;

    norm1 = 0;
    normi = 0;

    for (int32_t i = 0; i < n; i++) {
        tmp = 0;
        for (int32_t j = 0; j < n; j++) {
            tmp += *(matrix + i * n + j);
        }

        tmp > normi ? normi = tmp : tmp;
    }

    for (int32_t j = 0; j < n; j++) {
        tmp = 0;
        for (int32_t i = 0; i < n; i++) {
            tmp += *(matrix + i * n + j);
        }

        tmp > norm1 ? norm1 = tmp : tmp;
    }

    matrixB = initMatrixFromMatrix(matrix, n);

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 0, matrixB, n, matrixB, n, 1 / (normi * norm1), matrixB, n);

    matrixBA = initMatrix(n);

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1, matrixB, n, matrix, n, 0, matrixBA, n);

    matrixI = getIMatrix(n);

    matrixR = getIMatrix(n);

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, -1, matrixBA, n, matrixI, n, 1, matrixR, n);

    matrixRExp = initMatrixFromMatrix(matrixR, n);

    res = initMatrix(n);

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1, matrixR, n, matrixI, n, 0, res, n);

    matrixRExptmp = initMatrix(n);
    restmp = initMatrix(n);

    for (int32_t i = 1; i < iterations; i++) {
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1, matrixRExp, n, matrixR, n, 0, matrixRExptmp, n);
        free(matrixRExp);
        matrixRExp = matrixRExptmp;
        matrixRExptmp = initMatrix(n);
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1, matrixI, n, matrixRExp, n, 1, res, n);
    }

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1, res, n, matrixB, n, 0, restmp, n);

    free(matrixB);
    free(matrixBA);
    free(matrixR);
    free(matrixRExp);
    free(matrixRExptmp);
    free(matrixI);
    free(res);

    return restmp;
}


float* scanMatrix(int32_t n) {
    float* res;

    res = initMatrix(n);

    for (int32_t i = 0; i < n * n; i++) {
        scanf("%f", res + i);
    }

    return res;
}


int32_t main(int32_t argc, char** argv) {
    int n, m;
    float* matrix;
    float* res;
    clock_t start;

    n = atoi(argv[1]);
    m = atoi(argv[2]);

    matrix = getRandomMatrix(n);

    start = clock();
    res = getReverseMatrix(matrix, n, m);
    printf("Runtime: %lf seconds.\n", ((double)(clock() - start)) / CLOCKS_PER_SEC);

    free(matrix);
    free(res);

    return 0;
}
