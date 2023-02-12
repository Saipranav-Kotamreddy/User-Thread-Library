#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction old_action;
struct itimerval prev_timer_interval;
sigset_t ss;

void preempt_disable(void)
{
	/* TODO Phase 4 */
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
}

void handle_alarm () {
	uthread_yield();
}

void preempt_start(bool preempt)
{
	// todo: handle this case
	if (!preempt) {
		return;
	}
	
	struct sigaction preempt_action;
	preempt_action.sa_handler = handle_alarm;
	sigemptyset(&preempt_action.sa_mask);
	preempt_action.sa_flags = 0;
	sigaction(SIGVTALRM, &preempt_action, &old_action);

	struct itimerval new_timer_interval;
	new_timer_interval.it_interval.tv_sec = 0;
	new_timer_interval.it_interval.tv_usec = 10000;
	new_timer_interval.it_value.tv_sec = 0;
	new_timer_interval.it_value.tv_usec = 10000;
	int ret = setitimer(ITIMER_VIRTUAL, &new_timer_interval, &prev_timer_interval);
	if (ret) {
		printf("Error!\n");
	}
}

void preempt_stop(void)
{
	// Todo: handle case where preempt = false
	sigaction(SIGVTALRM, &old_action, NULL);
	setitimer(ITIMER_VIRTUAL, &prev_timer_interval, NULL);
}

