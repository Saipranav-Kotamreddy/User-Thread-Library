#include <stdlib.h>
#include "queue.h"
#include "sem.h"
#include "private.h"

#define ERR -1
#define OK 0

struct semaphore {
    size_t curr_count;
    queue_t waiting_queue;
};

sem_t sem_create(size_t count)
{
    sem_t sem = malloc(sizeof(struct semaphore));
    sem->curr_count = count;
    sem->waiting_queue = queue_create();
    if (sem->waiting_queue == NULL) {
        free(sem);
        return NULL;
    }
    return sem;
}

int sem_destroy(sem_t sem)
{
    if (sem == NULL || queue_length(sem->waiting_queue) != 0) {
        return ERR;
    }
    queue_destroy(sem->waiting_queue);
    free(sem);
    return OK;
}

void give_resource(sem_t sem) {
    sem->curr_count -= 1;
    preempt_enable();
}

void add_to_waiting_queue(sem_t sem) {
    queue_enqueue(sem->waiting_queue, uthread_current());
    preempt_enable();
    uthread_block();	
}

int sem_down(sem_t sem)
{
    if (sem == NULL) {
        return ERR;
    }
    preempt_disable();
    if (sem->curr_count > 0) {
        give_resource(sem);
    } else {
        add_to_waiting_queue(sem);
    } 
    return OK;
}

void unblock_waiting_thread(sem_t sem) {
    struct uthread_tcb* new_thread;
    queue_dequeue(sem->waiting_queue, (void**) &new_thread);
    uthread_unblock(new_thread);
}

int sem_up(sem_t sem)
{
    if (sem == NULL) {
        return ERR;
    }
    preempt_disable();
    if (queue_length(sem->waiting_queue) == 0) {
        sem->curr_count++;
    } else {
        unblock_waiting_thread(sem);
    }
    preempt_enable();
    return OK;
}

