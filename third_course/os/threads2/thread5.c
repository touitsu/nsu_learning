#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void* thread(void* arg) {
	printf("Thread %llu imitates activity...\n", pthread_self());

	pthread_detach(pthread_self());

	pthread_exit(NULL);
}

int32_t main() {
	pthread_t tid[1];
	int32_t thcr_res;

	while (1) {

		thcr_res = pthread_create(tid, NULL, thread, NULL);

		if (thcr_res) {
			return -1;
		}

	}
	return 0;
}
