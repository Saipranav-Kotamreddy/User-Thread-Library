/*
tests the signal handler directly
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <sem.h>
#include <uthread.h>
#include <private.h>
#include <pthread.h>

#include <signal.h>

void thread2(void *arg)
{
	(void)arg;

	printf("thread2\n");
}

void thread1(void *arg)
{
	(void)arg;

    printf("thread1 - 1\n");
	uthread_create(thread2, NULL);
    printf("thread1 - 2\n");
    raise(SIGVTALRM);
    printf("thread1 - 3\n");
}

int main(void)
{
	uthread_run(true, thread1, NULL);
	return 0;
}*/

/*
    exp output: thread 1 - 1, thread 1 - 2, thread 2 - 1 (then stops)
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <sem.h>
#include <uthread.h>
#include <private.h>
#include <pthread.h>

#include <signal.h>
#include <unistd.h>

bool somevalue = true;

void thread2(void *arg)
{
	(void)arg;

	printf("thread2 - 1\n");
    exit(0);
}

void thread1(void *arg)
{
	(void)arg;

    printf("thread1 - 1\n");
	uthread_create(thread2, NULL);
    printf("thread1 - 2\n");
    while(1) { }
    printf("thread1 - 3\n");
}

int main(void)
{
	uthread_run(true, thread1, NULL);
	return 0;
}

*/

// #include <stdbool.h>
// #include <stdio.h>
// #include <stdlib.h>

// #include <sem.h>
// #include <uthread.h>
// #include <private.h>
// #include <pthread.h>

// #include <signal.h>
// #include <unistd.h>

// bool somevalue = true;

// void thread2(void *arg)
// {
// 	(void)arg;

// 	printf("thread2 - 1\n");
//     exit(0);
// }

// void thread1(void *arg)
// {
// 	(void)arg;

//     printf("thread1 - 1\n");
// 	uthread_create(thread2, NULL);
//     printf("thread1 - 2\n");
//     preempt_disable();
//     while(1) { }
//     printf("thread1 - 3\n");
// }

// int main(void)
// {
// 	uthread_run(true, thread1, NULL);
// 	return 0;
// }

/* Preempt test simple:
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
	
	//time_t time_to_end = time(NULL) + 1;
	clock_t time_before_inf_loop = clock();
	int num_seconds_to_loop = 1;
	while(!time_elapsed(time_before_inf_loop, num_seconds_to_loop)) {}

    printf("thread1 - 3\n");
}

int main(void)
{
	uthread_run(true, thread1, NULL);
	return 0;
}
*/

/*
More complex test:

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

*/

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
	printf("thread2 - 1\n");
	preempt_disable();
	loop_for_duration(1);
	printf("thread2 - 2 \n");
	preempt_enable();
	loop_for_duration(1);
	printf("thread2 - 3 \n");
}

void thread1(void *arg)
{
	(void)arg;
	uthread_create(thread2, NULL);
	
	printf("Thread1 - 1\n");
	preempt_disable();
	loop_for_duration(1);

	printf("Thread1 - 2\n");
	preempt_enable();
	loop_for_duration(1);

	printf("Thread1 - 3\n");
}

int main(void)
{
	uthread_run(true, thread1, NULL);
	return 0;
}