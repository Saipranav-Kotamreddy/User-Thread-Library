/* This program is very simple and shows that no thread can hog the CPU when preemption is enabled */
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

void thread2(void *arg)
{
    (void)arg;
    printf("Thread2 - 1\n");
    exit(1);
}

void thread1(void *arg)
{
    (void)arg;
    uthread_create(thread2, NULL);
    printf("Thread1 - 1 \n");
    while(1) {}
    printf("Thread1 - 2 \n");
}

int main(void)
{
    uthread_run(true, thread1, NULL);
    return 0;
}