
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
    printf("thread 2 - attempting to destroy\n");
    int ret = sem_destroy(sem);
    printf("thread 2 - returned value: %d\n", ret);
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
    int ret = sem_destroy(sem);
    printf("Main - Returned value of sem destroy: %d\n", ret);
	return 0;
}