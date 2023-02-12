#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

enum{
	READY,
	RUNNING,
	BLOCKED
};

queue_t threadQueue;

queue_t blockedQueue;

uthread_ctx_t* loopContext;
struct uthread_tcb* currentThread;


struct uthread_tcb {
	/* TODO Phase 2 */
	uthread_ctx_t* context;
	void* stackPtr;
	int state;
};

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
	return currentThread;
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
	uthread_ctx_t* currentContext = currentThread->context;
	currentThread->state=READY;
	queue_enqueue(threadQueue, currentThread);
	queue_dequeue(threadQueue, (void**)&currentThread);
	currentThread->state=RUNNING;
	uthread_ctx_switch(currentContext, currentThread->context);
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
	setcontext(loopContext);
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	struct uthread_tcb* newThread= malloc(sizeof(struct uthread_tcb));
	newThread->context = malloc(sizeof(uthread_ctx_t));
	newThread->stackPtr = uthread_ctx_alloc_stack();
	newThread->state=READY;
	if(uthread_ctx_init(newThread->context, newThread->stackPtr, func, arg)){
		return -1;
	}
	if(queue_enqueue(threadQueue, newThread)){
		return -1;
	}
	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	preempt_start(preempt);
	/* TODO Phase 2 */
	threadQueue = queue_create();
	loopContext = malloc(sizeof(uthread_ctx_t));
	if(uthread_create(func, arg)){
		return -1;
	}
	while(queue_length(threadQueue)!=0){
		if(queue_dequeue(threadQueue, (void**)&currentThread)){
			return -1;
		}
		// reset timer
		currentThread->state=RUNNING;
		uthread_ctx_switch(loopContext, currentThread->context);
		free(currentThread->context);
		uthread_ctx_destroy_stack(currentThread->stackPtr);
		free(currentThread);
	}
	free(loopContext);
	queue_destroy(threadQueue);
	if (preempt) {
		preempt_stop();
	}
	return 0;
}

void uthread_block(void)
{
	// add the current thread to the blocked queue
	uthread_ctx_t* currentContext = currentThread->context;
	currentThread->state = BLOCKED;
	queue_enqueue(blockedQueue, currentThread);
	// take the next thread to run and run it
	queue_dequeue(threadQueue, (void**)&currentThread);
	currentThread->state=RUNNING;
	uthread_ctx_switch(currentContext, currentThread->context);
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	queue_delete(blockedQueue, uthread);
	uthread->state = READY;
	queue_enqueue(threadQueue, uthread);
}

