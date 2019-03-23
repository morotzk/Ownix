#include <ph/process.h>
#include <kheap.h>
#include <lib/string.h>
#include <ph/sched.h>
#include <protect.h>
#include <mem_util.h>
#include <printf.h>
#define sz							sizeof
#define NO_FREE_PID				32

/* The process table */
struct proc_tab ptab;
/* IDLE process descriptor */
struct proc_context idle_proc_context;

void idle_proc();
static pid_t create_proc_context(char* pname);
static pid_t search_free_pid(); 
static void reserve_pid(pid_t pid); 
static void free_pid(pid_t pid); 

void init_proc_handler() {
	pid_t pid;
	u32_t idle_stk;
	
	for(pid = 0; pid < PROC_POOL_SZ; ++pid) {
		ptab.ppool[pid].pid = pid;
	}
	
	/* Initially every pid is free except pid 0 
	 * pid 0 is marked as used to not confuse fork 
	 * later when fork will be implemented
	 */
	ptab.free_bmp = 0xFFFFFFFE;
   
	/* setup idle process context */
	idle_proc_context.schedinfo.sched_state = SCHED_STATE_IDLE;
	idle_proc_context.ec.kernel_mode = 1;
	idle_stk = ((u32_t)KMALLOC(1024)) + 1024;	
	idle_proc_context.ec.restore_kstack_esp = idle_stk - sz(intr_stackframe_t);
	idle_proc_context.ec.kernel_context = (intr_stackframe_t*)idle_proc_context.ec.restore_kstack_esp;	
	idle_proc_context.ec.kernel_context->cs = KERNEL_CS;
	idle_proc_context.ec.kernel_context->eip = (u32_t)&idle_proc;
	idle_proc_context.ec.kernel_context->ds = KERNEL_DS;
	idle_proc_context.ec.kernel_context->es = KERNEL_DS;
	idle_proc_context.ec.kernel_context->fs = KERNEL_DS;
	idle_proc_context.ec.kernel_context->gs = KERNEL_DS;	
	idle_proc_context.ec.kernel_context->eflags = 0x200;
	idle_proc_context.ec.kernel_context->eax =
	idle_proc_context.ec.kernel_context->ebx =
	idle_proc_context.ec.kernel_context->ecx =
	idle_proc_context.ec.kernel_context->edx =
	idle_proc_context.ec.kernel_context->esi =
	idle_proc_context.ec.kernel_context->edi =
	idle_proc_context.ec.kernel_context->esp =
	idle_proc_context.ec.kernel_context->ebp = 0;
	idle_proc_context.ec.kernel_context->ss = KERNEL_DS;
	idle_proc_context.ec.kernel_context->old_esp = 0;
	idle_proc_context.ec.kernel_context->restore_kstack_esp = idle_proc_context.ec.restore_kstack_esp;	
	idle_proc_context.ec.intr_level = -10;
}

struct proc_context* create_proc(char* pname) {
	struct proc_context* pcp;

	pid_t pid = create_proc_context(pname);
	pcp = &ptab.ppool[pid];

	pcp->ec.kernel_tss_esp = ((u32_t)KMALLOC(8192)) + 8192;
	pcp->ec.restore_kstack_esp = pcp->ec.kernel_tss_esp - sizeof(intr_stackframe_t);
	pcp->ec.user_context = (intr_stackframe_t*)pcp->ec.restore_kstack_esp;
	pcp->ec.user_context->restore_kstack_esp = pcp->ec.restore_kstack_esp;
	pcp->ec.user_context->eax = pcp->ec.user_context->ebx =
	pcp->ec.user_context->ecx = pcp->ec.user_context->edx =
	pcp->ec.user_context->esi = pcp->ec.user_context->edi = 0;

	pcp->ec.user_context->cs = USER_CS + 0x3;
	pcp->ec.user_context->ds = USER_DS + 0x3;
	pcp->ec.user_context->es = USER_DS + 0x3;
	pcp->ec.user_context->fs = USER_DS + 0x3;
	pcp->ec.user_context->gs = USER_DS + 0x3;
	pcp->ec.user_context->ss = USER_DS + 0x3;
	pcp->ec.user_context->eflags = 0x200;
	
	/* set the stack top at virt addr 4gb - 1; */
	pcp->ec.user_context->old_esp = 0xffffffff;
	pcp->ec.user_context->esp = pcp->ec.user_context->old_esp;
	pcp->ec.user_context->ebp = pcp->ec.user_context->old_esp;
	
	/* user mode */
	pcp->ec.kernel_mode = 0;	
	pcp->ec.intr_level = 0;	
	/* create the process's empty virtual address space */
	pcp->vm.pd = create_vas();	
	/* create the virtual mapping for the process stack */
	create_mem_map(&(pcp->vm), MAP_TYPE_STACK, 0xffffffff, 0x1000, 0);
	
	return pcp;
}

static pid_t search_free_pid() {
	return first_set_bit(ptab.free_bmp);	
}

static void reserve_pid(pid_t pid) {
	ptab.free_bmp &= ~(1 << pid);
}

static void free_pid(pid_t pid) {
	ptab.free_bmp |= 1 << pid;
}

/* A kernel agent is interrupted. */
void hold(pid_t pid, u8_t event) {	
	if(ptab.ppool[pid].ec.kernel_mode == 0)
		ptab.ppool[pid].ec.kernel_mode = 1;
			
	ptab.ppool[pid].schedinfo.sched_state = SCHED_STATE_WAITING;
	ptab.ppool[pid].schedinfo.waitfor = event;		
	sched_hold();	
	/* enforce the context switching */
	if(sched_get_curr_proc()->ec.intr_level > 1) {
		context_handler(sched_get_curr_proc()->ec.kernel_context->restore_kstack_esp, 0, 0);
	} else {
		context_handler(sched_get_curr_proc()->ec.user_context->restore_kstack_esp, 1, sched_get_curr_proc()->vm.pd);
	}
}

void unhold(pid_t pid, u8_t event) {
	if(ptab.ppool[pid].schedinfo.sched_state != SCHED_STATE_WAITING) 
		panic("Try to unhold a not waiting process!");	
	if(ptab.ppool[pid].schedinfo.waitfor != event)
		panic("Process not wait for this event!");
		
	mark_ready_sched(pid);	
	/* enforce the context switching */
	if(sched_get_curr_proc()->ec.intr_level > 1) {
		context_handler(sched_get_curr_proc()->ec.kernel_context->restore_kstack_esp, 0, 0);
	} else {
		context_handler(sched_get_curr_proc()->ec.user_context->restore_kstack_esp, 1, sched_get_curr_proc()->vm.pd);
	}
}

void mark_ready_sched(pid_t pid) {
	if(ptab.ppool[pid].ec.kernel_mode == 1) {
		sched_enforce_run_next(&ptab.ppool[pid]);
	} else {
		sched_ready(&ptab.ppool[pid]);
	}
}

static pid_t create_proc_context(char* pname) {
	pid_t free_pid = search_free_pid();

	if(free_pid == NO_FREE_PID)
		panic("Process table full. Cannot create new process.");
		
	reserve_pid(free_pid);	
	strcpy(ptab.ppool[free_pid].proc_name, pname);
	ptab.ppool[free_pid].schedinfo.sched_state = SCHED_STATE_CREATED;
	return free_pid;
}

void delete_proc_context(pid_t pid) {
	struct q_node* mmap_list;
	struct mmapd* mmapdp;	 
	u8_t i;
	/* Close opened files! */
	for(i = 0; i < PROC_OPEN_MAX; ++i) {
		if(ptab.ppool[pid].ftab.ofile[i].fdp != NULL_FILED) {
	 		close(&ptab.ppool[pid].ftab.ofile[i]);
	 		ptab.ppool[pid].ftab.ofile[i].fdp = NULL_FILED;
	 	}
	}	 
	ptab.ppool[pid].ftab.fbmp = 0xff;
 	memset((void*)&(ptab.ppool[pid].ftab), 0, sizeof(struct file_tab)); 
 	/* free allocated kernel stack */
	KFREE((void*)ptab.ppool[pid].ec.kernel_tss_esp);	
	/* destroy process's' VAS */	
	delete_vas(ptab.ppool[pid].vm.pd);		
	/* delete the process's mappings
	 * and close memory mapped files
	 */
	mmap_list = ptab.ppool[pid].vm.mmap_list;
	while(mmap_list != NULL_Q) {
		mmapdp = (struct mmapd*)mmap_list->value;
		if(mmapdp->mmap_file.fdp != NULL_FILED) {
			close(&mmapdp->mmap_file);
		}
		KFREE((void*)mmapdp);
		mmap_list = q_rem(mmap_list);
	}	
	/* Tell the scheduler to remove process from
	 * its ready queue.
	 */
	sched_hold();
	/* pid is free now again */	
	free_pid(pid);
}

/* Build system call return value */
void finalize_syscall(pid_t pid, s32_t retval) {
	if(ptab.ppool[pid].ec.intr_level > 1) {
		ptab.ppool[pid].ec.kernel_context->eax = retval;
	} else {
		ptab.ppool[pid].ec.user_context->eax = retval;
	}
}

void complete_syscall() {
	if(sched_get_curr_proc()->ec.intr_level  <= 1 && 
	   sched_get_curr_proc()->ec.intr_level >= 0 &&
	   sched_get_curr_proc()->ec.kernel_mode == 1) 	{
			
		sched_get_curr_proc()->ec.kernel_mode = 0;
		sched_yield();
	}
}

void idle_proc() {
	asm volatile("sti");
	while(1) ;		
}

struct proc_context* get_idle_proc() {
	return &idle_proc_context;
}

