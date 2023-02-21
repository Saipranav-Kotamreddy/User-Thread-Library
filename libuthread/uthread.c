#include <stdlib.h>
#include "private.h"
#include "uthread.h"
#include "queue.h"

enum{
    READY,
    RUNNING,
    BLOCKED
};
enum {
    SUCCESS,
    ERR
};
struct uthread_tcb {
    uthread_ctx_t* context;
    void* stack_ptr;
    int state;
};

queue_t thread_queue;
queue_t blocked_queue;
uthread_ctx_t* loop_context;
struct uthread_tcb* current_thread;

struct uthread_tcb *uthread_current(void)
{
    return current_thread;
}

void uthread_yield(void)
{
    /* Add thread to the back of the "ready" queue */
    preempt_disable();
    uthread_ctx_t* current_context = current_thread->context;
    current_thread->state = READY;
    queue_enqueue(thread_queue, current_thread);

    /* Run the thread at the front of the "ready" queue */
    queue_dequeue(thread_queue, (void**) &current_thread);
    current_thread->state = RUNNING;
    preempt_enable();
    uthread_ctx_switch(current_context, current_thread->context);
}

void uthread_exit(void)
{
    setcontext(loop_context);
}

int uthread_create(uthread_func_t func, void *arg)
{
    /* Create new thread */
    struct uthread_tcb* new_thread = malloc(sizeof(struct uthread_tcb));
    new_thread->context = malloc(sizeof(uthread_ctx_t));
    new_thread->stack_ptr = uthread_ctx_alloc_stack();
    new_thread->state=READY;

    /* Initialize thread context and add it to queue */
    if (uthread_ctx_init(new_thread->context, new_thread->stack_ptr, func, arg) != SUCCESS) {
        return ERR;
    }

    preempt_disable();
    if (queue_enqueue(thread_queue, new_thread) != SUCCESS) {
        preempt_enable();
        return ERR;
    }

    preempt_enable();
    return SUCCESS;
}

int initialize_main_thread(uthread_func_t func, void *arg) {
    thread_queue = queue_create();
    loop_context = malloc(sizeof(uthread_ctx_t));
    if (uthread_create(func, arg) != SUCCESS) {
        return ERR;
    } else {
        return SUCCESS;
    }
}

int switch_to_next_available_thread() {
    preempt_disable();
    if (queue_dequeue(thread_queue, (void**) &current_thread) != SUCCESS) {
        return ERR;
    }
    current_thread->state = RUNNING;
    preempt_enable();
    uthread_ctx_switch(loop_context, current_thread->context);
    return SUCCESS;
}

void delete_current_thread() {
    preempt_disable();
    free(current_thread->context);
    uthread_ctx_destroy_stack(current_thread->stack_ptr);
    free(current_thread);
    preempt_enable();
}

void deallocate_main_thread() {
    preempt_disable();
    free(loop_context);
    queue_destroy(thread_queue);
    preempt_stop();
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
    /* Initialize the thread queue and loop context */
    int state = initialize_main_thread(func, arg);
    if (state != SUCCESS) {
        return ERR;
    }

    /* Run each available thread */
    preempt_start(preempt);
    while (queue_length(thread_queue)!=SUCCESS) {
        state = switch_to_next_available_thread();
        if (state != SUCCESS) {
            return ERR;
        }
        delete_current_thread();
    }

    /* deallocate the thread queue and loop context */
    deallocate_main_thread();
    return SUCCESS;
}

void uthread_block(void)
{
    /* add the current thread to the blocked queue */
    preempt_disable();
    uthread_ctx_t* current_context = current_thread->context;
    current_thread->state = BLOCKED;
    queue_enqueue(blocked_queue, current_thread);
    
    /* take the next thread to run and run it */
    queue_dequeue(thread_queue, (void**) &current_thread);
    current_thread->state = RUNNING;
    preempt_enable();
    uthread_ctx_switch(current_context, current_thread->context);
}

void uthread_unblock(struct uthread_tcb *uthread)
{
    queue_delete(blocked_queue, uthread);
    uthread->state = READY;
    queue_enqueue(thread_queue, uthread);
}

