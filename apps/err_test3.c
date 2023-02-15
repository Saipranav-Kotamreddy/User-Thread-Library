/* Test to ensure calling preemption functions when preemption is not set does not cause error */
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


bool time_elapsed(clock_t start_time, int duration) {
	return (clock() - start_time) / CLOCKS_PER_SEC >= duration;
}

void loop_for_duration(int duration) {
	clock_t time_before_inf_loop = clock();
	while(!time_elapsed(time_before_inf_loop, duration)) {}
}

void thread2(void *arg)
{
	(void)arg;
	printf("Thread2 - 1\n");
}

void thread1(void *arg)
{
	(void)arg;
	uthread_create(thread2, NULL);
	
	printf("Thread1 - 1\n");
	preempt_enable();
	loop_for_duration(1);
	printf("Thread1 - 2\n");
	preempt_disable();
	loop_for_duration(1);
	printf("Thread1 - 3\n");
	preempt_stop();
	loop_for_duration(1);
	printf("Thread1 - 4\n");
}

int main(void)
{
	uthread_run(false, thread1, NULL);
	return 0;
}