#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <intrin.h>
#include <time.h>
#include <string.h>

#define L1_SIZE 98304
#define L2_SIZE 786432
#define L3_SIZE 2097152


int32_t* makeForwardArray(int32_t size) {
	int32_t* res = _aligned_malloc(sizeof(int32_t) * size, 32);

	for (size_t i = 0; i < size; i++) {
		*(res + i) = i + 1;
	}

	return res;
}


int32_t* makeBackwardArray(int32_t size) {
	int32_t* res = _aligned_malloc(sizeof(int32_t) * size, 32);

	*res = size - 1;

	for (size_t i = 1; i < size; i++) {
		*(res + i) = i - 1;
	}

	return res;
}


int32_t* makeRandomArray(int32_t size) {
	int32_t* res = _aligned_malloc(sizeof(int32_t) * size, 32);
	int32_t randIdx, curIdx;
	size_t j, i;

	memset(res, -1, size*sizeof(int32_t));

	curIdx = 0;

	for (i = 0; i < size; i++) {
		randIdx = rand() % size;

		if (*(res + randIdx) == -1 && randIdx != curIdx) {
			*(res + curIdx) = randIdx;
			curIdx = randIdx;
		}

		else {
			for (j = 0; (*(res + j) != -1 || j == curIdx) && j < size; j++);
			*(res + curIdx) = j;
			curIdx = j;
		}
	}

	return res;
}


void freeArray(int32_t* arr) {
	_aligned_free(arr);
}


void printArr(int32_t* arr, int32_t size) {
	for (size_t i = 0; i < size; i++) {
		printf("%d ", *(arr + i));
	}
	printf("\n");
}


void warmUpCache() {
	int32_t* arr;
	int32_t k;
	
	arr = makeForwardArray(L3_SIZE);

	k = 0;

	for (int32_t i = 0; i < L3_SIZE; i++) {
		k = *(arr + k);
	}

	printf("%d warm up\n", k);

	freeArray(arr);
}


int32_t main() {
	int32_t* arr;
	int32_t k, i;
	uint64_t startTicks, curTicks, resTicks;
	FILE* file;
	char* size_buff;
	char* ticks_buff;

	size_buff = (char*)malloc(sizeof(char) * 32);
	ticks_buff = (char*)malloc(sizeof(char) * 32);
	
	file = fopen("output.csv", "w");

	fwrite("array size;ticks to read element\n", sizeof(char), 33, file);
	
	for (size_t size = 10; size < L3_SIZE * 2; size *= 1.2) {
		arr = makeRandomArray(size);
		
		curTicks = 0;
		resTicks = UINT64_MAX;

		itoa(size, size_buff, 10);

		fwrite(size_buff, sizeof(char), strlen(size_buff), file);
		fwrite(";", sizeof(char), 1, file);

		for (size_t n = 0; n < 1000; n++) {
			warmUpCache();
			k = 0;

			startTicks = __rdtsc();
			for (i = 0; i < size && i < 1000; i++) {
				k = *(arr + k);
			}
			curTicks = (__rdtsc() - startTicks) / i;

			if (curTicks < resTicks) {
				resTicks = curTicks;
			}
		}
	
		itoa(resTicks, ticks_buff, 10);

		fwrite(ticks_buff, sizeof(char), strlen(ticks_buff), file);
		fwrite("\n", sizeof(char), 1, file);

		printf("%d %d %lld\n", k, size, resTicks);
		
		freeArray(arr);
	}

	fclose(file);
	free(size_buff);
	free(ticks_buff);
	
	return 0;
}
