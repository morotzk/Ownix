#include "iom.h"

/* The waiting queues per device */
static struct q_node* entry_q[16] = {NULL_Q, };
/* The applied io logic */
u32_t io_logic = IO_LOGIC_SYNC;
/* actual synchrone io request fields */
u8_t ior_cmpl = 0;
int ior_success = 0;

void set_io_logic(u32_t iol) {
	if(iol != IO_LOGIC_ASYNC && iol != IO_LOGIC_SYNC) return;
	io_logic = iol;
}

void serve_ior(struct io_request* ior) {
	/* Synchronous io is used only by the kernel 
	 * when an executable files header shall be
	 * read.
	 */
	if(io_logic == IO_LOGIC_SYNC) {
		if(get_device_flags(ior->major_dev) & RAM_DEV) {
			ior->success = dispatch_ior(*ior);
		} else {
			ior_cmpl = 0;
			ior_success = 0;
			ior->success = dispatch_ior(*ior);
			if(ior->success < 0) return;
			/* busy wait for the io-completion */			
			while(!ior_cmpl) ;
			ior->success = ior_success;
		}
		return; 
	}

	/* We reached the maximum iorequests stored
	 * on the waiting queue, so finish the game
	 */
	if(q_size(entry_q[ior->major_dev]) == ENTRY_Q_SZ)
		finalize_syscall(ior->reqor_pid, EBUSY);
		
	/* Is the requested device a blocking device? */
	if(!(get_device_flags(ior->major_dev) & RAM_DEV)) {
		/* blocking io-request */
		
		/* Copy the iorequest to us */
		struct io_request* iorp = (struct io_request*)KMALLOC(sizeof(struct io_request));
		*iorp = *ior;		
		
		/* Enqueue it to the appropriate queue */
		q_enq(entry_q[ior->major_dev], (void*)iorp);
		
		/* The appr. queue was empty before enqueue? */
		if(q_size(entry_q[ior->major_dev]) == 1) {
			int success = dispatch_ior(*(struct io_request*)entry_q[ior->major_dev]->value);
			if(success < 0) {				
			 	KFREE(entry_q[ior->major_dev]->value); 
				entry_q[ior->major_dev] = q_rem(entry_q[ior->major_dev]);
				finalize_syscall(ior->reqor_pid, success);
				return;				
			}
		}
				
		hold(((struct io_request*)entry_q[ior->major_dev]->value)->reqor_pid, IOR_COMPLETED);
	} else {
		/* nonblocking io-request */		
		int success = dispatch_ior(*ior);
		finalize_syscall(ior->reqor_pid, success);
	}	
}

void ior_complete(iorid_t id, int success) {
	pid_t unhold_pid;
	int succ;

	/* In IO_LOGIC_SYNC we just set the completed flag to 1 and return */
	if(io_logic == IO_LOGIC_SYNC) {
		ior_cmpl = 1;
		ior_success = success;
		return;	
	}
	/* Satisfy the while loop's condition 
	 * if queue is not empty
	 */	
	succ = -1;
	/* Report the system call result to the
	 * process handler.
	 */
	unhold_pid = ((struct io_request*)entry_q[id]->value)->reqor_pid; 	
	finalize_syscall(unhold_pid, success);			
	/* Free the allocated space for the iorequest in the queue node */
	KFREE(entry_q[id]->value);	
	/* Free the head of the queue itself */
	entry_q[id] = q_rem(entry_q[id]);	
	if(q_size(entry_q[id]) == 0)
		return;		
	/* Process next iorequests until:
	 * 	(I)  : no more iorequests on the queue
	 *			OR
	 *	(II) : an iorequest dispatched successful
	 */	
	while(q_size(entry_q[id]) != 0 && succ < 0) {
		succ = dispatch_ior(*(struct io_request*)entry_q[id]->value);
		if(succ != 0) {
			finalize_syscall(((struct io_request*)entry_q[id]->value)->reqor_pid, succ);
			entry_q[id] = q_rem(entry_q[id]);						
		}
	}
	
	unhold(unhold_pid, IOR_COMPLETED);
}	

int dispatch_ior(struct io_request ior) {
	switch(ior.ior_type) {
		case IOR_TYPE_READ : 
			return device_read(ior.major_dev, ior.read_ior.buf, ior.read_ior.nbytes);				
			
		case IOR_TYPE_WRITE :
			return device_write(ior.major_dev, ior.write_ior.buf, ior.write_ior.nbytes);
			
		case IOR_TYPE_OPEN :
			return device_open(ior.major_dev);
			
		case IOR_TYPE_CLOSE :
			return device_close(ior.major_dev);
			
		case IOR_TYPE_LSEEK : 
			return device_lseek(ior.major_dev, ior.lseek_ior.position);			
								
	}
}
