#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

int32_t global_var = 0;

void* mythread(void* arg) {
	int32_t local_var;
	static int32_t local_static_var;
	const int32_t local_const_var = 0;

	local_var = 0;
	local_static_var = 0;

	printf("mythread [pid: %d, parent pid: %d, thread id: %d, self: %d]: Hello from mythread!\n", getpid(), getppid(), gettid(), pthread_self());
	printf("mythread (ID: %d) local_var: %p, local_static_var: %p, local_const_var: %p, global_var: %p\n", gettid(), &local_var, &local_static_var, &local_const_var, &global_var);

	sleep(100);

	return NULL;
}

void create_thread(pthread_t* tid) {
	int32_t err;
	
	err = pthread_create(tid, NULL, mythread, NULL);
	if (err) {
		printf("main: pthread_create failed\n");
	}
}

int32_t main() {
	pthread_t tids[5];
	void** res;

	printf("main [pid: %d, parent pid: %d, thread id: %d]: Hello from main!\n", getpid(), getppid(), gettid());

	for (int8_t i = 0; i < 5; i++) {
		create_thread(tids + i);
	}

	for (int8_t i = 0; i < 5; i++) {
		pthread_join(*(tids + i), res);
	}

	return 0;
}


