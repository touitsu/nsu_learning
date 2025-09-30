#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void* thread(void* arg) {
	printf("Thread %llu imitates activity...\n", pthread_self());

	pthread_exit(NULL);
}

int32_t main() {
	pthread_t tid[1];
  pthread_attr_t attr;
	int32_t thcr_res;
  uint64_t cnt;
  size_t stack_size;

  cnt = 0;

  pthread_attr_init(&attr);

  stack_size = 0;

  pthread_attr_getstacksize(&attr, &stack_size);

	while (1) {

    cnt++;
		thcr_res = pthread_create(tid, NULL, thread, NULL);
    printf("Create thread %llu with stack size %llu\n", cnt, (uint64_t)stack_size);

	}

  pthread_attr_destroy(&attr);

	return 0;
}
