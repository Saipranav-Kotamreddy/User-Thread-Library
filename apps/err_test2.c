#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <sem.h>
#include <uthread.h>
#include <private.h>
#include <pthread.h>

sem_t sem;

int main(void)
{
    int ret1 = sem_up(NULL);
    int ret2 = sem_down(NULL);
    printf("Returned: %d %d\n", ret1, ret2);
    sem = sem_create(0);
    ret1 = sem_up(sem);
    ret2 = sem_down(sem);
    printf("Returned: %d %d\n", ret1, ret2);
	return 0;
}