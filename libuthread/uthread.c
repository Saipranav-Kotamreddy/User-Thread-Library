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

queue_t threadQueue;
uthread_ctx_t* loopContext;

struct uthread_tcb {
	/* TODO Phase 2 */
	uthread_ctx_t* context;
	void* stackPtr;
	int state;
};

//struct uthread_tcb *uthread_current(void)
//{
	/* TODO Phase 2/3 */
//}

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
	printf("Exit\n");
	setcontext(loopContext);
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	struct uthread_tcb* newThread= malloc(sizeof(struct uthread_tcb));
	newThread->context = malloc(sizeof(uthread_ctx_t));
	newThread->stackPtr = uthread_ctx_alloc_stack();
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
	if(preempt){
		return -1;
	}
	/* TODO Phase 2 */
	threadQueue = queue_create();
	loopContext = malloc(sizeof(uthread_ctx_t));
	if(uthread_create(func, arg)){
		return -1;
	}
	struct uthread_tcb* activeThread;
	while(queue_length(threadQueue)!=0){
		if(queue_dequeue(threadQueue, (void**)&activeThread)){
			return -1;
		}
		uthread_ctx_switch(loopContext, activeThread->context);
		free(activeThread->context);
		uthread_ctx_destroy_stack(activeThread->stackPtr);
		free(activeThread);
	}
	free(loopContext);
	queue_destroy(threadQueue);
	return 0;
}

void uthread_block(void)
{
	/* TODO Phase 3 */
}

//void uthread_unblock(struct uthread_tcb *uthread)
//{
	/* TODO Phase 3 */
//}

