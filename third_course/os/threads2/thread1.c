#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* thread(void* arg) {
	printf("Thread %llu imitates activity...\n", pthread_self());

	sleep(1);

	return NULL;
}

int32_t main() {
	pthread_t tid[1];
	void** res;
	int32_t thcr_res;

	thcr_res = pthread_create(tid, NULL, thread, NULL);

	if (thcr_res) {
		return -1;
	}

	printf("Main threads waits for %llu to finish\n", *tid);

	pthread_join(*tid, res);

	return 0;
}
