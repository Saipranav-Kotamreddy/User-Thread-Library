#include <stddef.h>
#include <stdlib.h>
// remove stdio import later:
#include <stdio.h>
#include "queue.h"
#include "sem.h"
#include "private.h"

#include <pthread.h>

#define ERR -1
#define OK 0

struct semaphore {
	size_t curr_count;
	queue_t waiting_queue;
	pthread_spinlock_t lock;
};

sem_t sem_create(size_t count)
{
	sem_t sem = malloc(sizeof(struct semaphore));
	sem->curr_count = count;
	sem->waiting_queue = queue_create();
	pthread_spin_init(&(sem->lock), 1); 
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

int sem_down(sem_t sem)
{
	if (sem == NULL) {
		return ERR;
	}
	pthread_spin_lock(&(sem->lock));
	if (sem->curr_count > 0) {
		sem->curr_count -= 1;
		pthread_spin_unlock(&(sem->lock));
	} else {
		queue_enqueue(sem->waiting_queue, uthread_current());
		pthread_spin_unlock(&(sem->lock));
		uthread_block();
	}
	return OK;
}

int sem_up(sem_t sem)
{
	if (sem == NULL) {
		return ERR;
	}
	pthread_spin_lock(&(sem->lock));
	if (queue_length(sem->waiting_queue) == 0) {
		sem->curr_count++;
	} else {
		struct uthread_tcb* new_thread;
		queue_dequeue(sem->waiting_queue, (void**)&new_thread);
		uthread_unblock(new_thread);
	}
	pthread_spin_unlock(&(sem->lock));
	return OK;
}

