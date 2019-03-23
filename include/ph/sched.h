#ifndef SCHED_H
#define SCHED_H
#include <ph/contexts.h>
#include <ph/process.h>
#include <ds/circ_queue.h>

/* scheduling states */
#define SCHED_STATE_CREATED	0
#define SCHED_STATE_READY	1
#define SCHED_STATE_RUNNING	2
#define SCHED_STATE_TERMINATED	3
#define SCHED_STATE_WAITING	4
#define SCHED_STATE_IDLE	5

#define QUANTUM			100

void sched_hold();
void sched_ready(struct proc_context* proc);
void sched_enforce_run_next(struct proc_context* proc);
void sched_yield();
struct proc_context* sched_get_curr_proc();
/* Called from the timer intr handler */
void sched_on_tick();

#endif
