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
    preempt_disable();
    while(1) { }
    printf("thread1 - 3\n");
}

int main(void)
{
	uthread_run(true, thread1, NULL);
	return 0;
}
