#include <ph/sched.h>
#include <printf.h>

/* Schedulers ready queue(circular) */
struct circ_q_node* ready_queue = NULL_C_Q;
struct proc_context* curr_proc = NULL_PROC;

/* do hold's scheduler-specific work */
void sched_hold() {
	ready_queue = circ_q_rem(ready_queue);
	if(ready_queue == NULL_C_Q) {
		curr_proc = get_idle_proc();
		printf("Now running idle!\n");
	} else {		
		curr_proc = (struct proc_context*)ready_queue->value;
	}
}

void sched_ready(struct proc_context* proc) {
	u8_t first = 0;

	/* If ready queue is empty then create it */
	if(ready_queue == NULL_C_Q) {	
		ready_queue = circ_q_creat();
		first = 1;
	}

	/* Check its state first */	
	
	if(proc->schedinfo.sched_state == SCHED_STATE_CREATED) {
		/* Process created just now. */
	
		/* Setup its schedinfo structure */
		proc->schedinfo.sched_state = SCHED_STATE_READY;
		proc->schedinfo.default_quantum = QUANTUM;
		proc->schedinfo.pending_quantum = QUANTUM;
		proc->schedinfo.waitfor = 0;	
	} else {
		/* Process is in waiting state */
		proc->schedinfo.sched_state = SCHED_STATE_READY;
		proc->schedinfo.waitfor = 0;		
	}
	
	/* Add to the ready queue */
	if(first) {
		ready_queue->value = (void*)proc;
		curr_proc = proc;
	}
	else {
		circ_q_enq(ready_queue, (void*)proc);
		curr_proc = (struct proc_context*)ready_queue->value;
	}
}

void sched_enforce_run_next(struct proc_context* proc) {
	/* Add priority to kernel agent to
	 * complete its job.
	 */	 
	curr_proc = proc;
}

void sched_yield() {	
	sched_ready(curr_proc);	
}

struct proc_context* sched_get_curr_proc() {
	return curr_proc;
}

void sched_on_tick() {
	/* IDLE has infinite quantum */
	if(curr_proc->schedinfo.sched_state == SCHED_STATE_IDLE)
		return;

	if(curr_proc->schedinfo.pending_quantum == 0) {		
		curr_proc->schedinfo.sched_state = SCHED_STATE_READY;
		curr_proc->schedinfo.pending_quantum = QUANTUM;		
      ready_queue = ready_queue->next;		
		curr_proc = (struct proc_context*)ready_queue->value;		
		curr_proc->schedinfo.sched_state = SCHED_STATE_RUNNING;
	} else {
		--curr_proc->schedinfo.pending_quantum;
	}
}

