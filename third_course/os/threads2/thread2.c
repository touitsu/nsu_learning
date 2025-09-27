#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* thread(void* arg) {
	int32_t* res;

	printf("Thread %llu imitates activity...\n", pthread_self());

	sleep(1);

	res = (int32_t*)malloc(sizeof(int32_t));

	*(res) = 42;

	pthread_exit((void*)res);
}

int32_t main() {
	pthread_t tid[1];
	int32_t* res;
	int32_t thcr_res;

	thcr_res = pthread_create(tid, NULL, thread, NULL);

	if (thcr_res) {
		return -1;
	}

	printf("Main threads waits for %llu to finish\n", *tid);

	pthread_join(*tid, (void**)&res);

	printf("Thread returned: %d\n", *res);

	free(res);

	return 0;
}
