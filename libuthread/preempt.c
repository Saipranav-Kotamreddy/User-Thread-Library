#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

#define TIME_INTERVAL_SECONDS 0
#define NO_FLAGS 0
#define HZ 100
#define NUM_MICROSECONDS_IN_SECOND 1000000
const int TIME_INTERVAL_MICROSECONDS = NUM_MICROSECONDS_IN_SECOND / HZ;

struct sigaction old_action;
struct itimerval prev_timer_interval;
sigset_t signals_to_block;
sigset_t old_signals;
bool premption_requested = false;

void preempt_disable(void)
{
	if (!premption_requested) {
		return;
	}
	sigprocmask(SIG_BLOCK, &signals_to_block, NULL);
}

void preempt_enable(void)
{
	if (!premption_requested) {
		return;
	}
	sigprocmask(SIG_UNBLOCK, &signals_to_block, NULL);
}

void handle_alarm () {
	uthread_yield();
}

void setup_alarm_handler() {
	struct sigaction preempt_action;
	preempt_action.sa_handler = handle_alarm;
	sigemptyset(&preempt_action.sa_mask);
	preempt_action.sa_flags = NO_FLAGS;
	sigaction(SIGVTALRM, &preempt_action, &old_action);
}

void setup_timer() {
	struct itimerval new_timer_interval;
	new_timer_interval.it_interval.tv_sec = TIME_INTERVAL_SECONDS;
	new_timer_interval.it_interval.tv_usec = TIME_INTERVAL_MICROSECONDS;
	new_timer_interval.it_value.tv_sec = TIME_INTERVAL_SECONDS;
	new_timer_interval.it_value.tv_usec = TIME_INTERVAL_MICROSECONDS;
	setitimer(ITIMER_VIRTUAL, &new_timer_interval, &prev_timer_interval);
}

void setup_signals_to_block() {
	sigemptyset(&signals_to_block);
	sigaddset(&signals_to_block, SIGVTALRM);
	sigprocmask(SIG_BLOCK, NULL, &old_signals);
}

void preempt_start(bool preempt)
{
	if (!preempt) {
		return;
	}
	premption_requested = true;
	setup_timer();
	setup_signals_to_block();
	setup_alarm_handler();
}

void preempt_stop(void)
{
	if (!premption_requested) {
		return;
	}
	sigaction(SIGVTALRM, &old_action, NULL);
	setitimer(ITIMER_VIRTUAL, &prev_timer_interval, NULL);
}

