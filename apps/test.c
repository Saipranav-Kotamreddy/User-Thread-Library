/*
    Testing block, unblock:

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>
#include <private.h>

struct uthread_tcb* threadBlocked;

void thread2(void *arg) {
    (void)arg;
    printf("Thread 2 is running and done!\n");
    uthread_unblock(threadBlocked);
}

void thread1(void *arg)
{
	(void)arg;

    uthread_create(thread2, NULL);

	printf("Hello world!\n");
    threadBlocked = uthread_current();
    uthread_block();
    printf("Thread 1 is done!\n");
}

int main(void)
{
	uthread_run(false, thread1, NULL);

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

sem_t sem;
void thread2(void *arg)
{
	(void)arg;

    printf("thread 2 - 1\n");
    sem_up(sem);
    printf("thread 2 - done\n");
}

void thread1(void *arg)
{
	(void)arg;

    printf("thread 1 - 1\n");
	uthread_create(thread2, NULL);
    printf("thread 1 - 2\n");
	sem_down(sem);
    printf("thread 1 - 3\n");
}

int main(void)
{
	sem = sem_create(0);
    uthread_run(false, thread1, NULL);
    sem_destroy(sem);
	return 0;
}