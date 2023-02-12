#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <sem.h>
#include <uthread.h>
#include <private.h>
#include <pthread.h>

#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

bool keep_looping = true;
void thread2(void *arg)
{
	(void)arg;
	printf("thread2 - 1\n");
}

bool time_elapsed(clock_t start_time, int duration) {
	return (clock() - start_time) / CLOCKS_PER_SEC >= duration;
}

void thread1(void *arg)
{
	(void)arg;
	uthread_create(thread2, NULL);
	
	printf("Thread1 - 1\n");
	preempt_disable();

	clock_t time_before_inf_loop = clock();
	int num_seconds_to_loop = 1;
	while(!time_elapsed(time_before_inf_loop, num_seconds_to_loop)) {}

	printf("Thread1 - 2\n");
	preempt_enable();

	time_before_inf_loop = clock();
	while(!time_elapsed(time_before_inf_loop, num_seconds_to_loop)) {}

    printf("thread1 - 3\n");
}

int main(void)
{
	uthread_run(true, thread1, NULL);
	return 0;
}