#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void* thread(void* arg) {
	char* res;

	printf("Thread %llu imitates activity...\n", pthread_self());

	sleep(1);

	res = (char*)malloc(sizeof(char) * 20);

	strcpy(res, "hello world");

	pthread_exit((void*)res);
}

int32_t main() {
	pthread_t tid[1];
	char* res;
	int32_t thcr_res;

	thcr_res = pthread_create(tid, NULL, thread, NULL);

	if (thcr_res) {
		return -1;
	}

	printf("Main threads waits for %llu to finish\n", *tid);

	pthread_join(*tid, (void**)&res);

	printf("Thread returned: %s\n", res);

	free(res);

	return 0;
}
