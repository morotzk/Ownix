#include <sys/types.h>
#include <protect.h>
#include <printf.h>
#include <tss.h>
#include <mem_util.h>
#include <ph/sched.h>
#include "iom/iom.h"
#include <sys/system.h>

#define NR_INT_VECS 			256
#define PRIV_LEV_KERN			0x00000000
#define PRIV_LEV_USER			0x00000003
#define PRIV_MASK			0x00000003

#define NULL_HANDLER 			((intr_handler_t)0)
#define ZERO_HANDLERS			memset((void*)&intr_handlers, 0, sizeof(intr_handler_t)*NR_INT_VECS)
#define int_addr(x) 			((u32_t)x)

intr_handler_t intr_handlers[NR_INT_VECS];	/* high level interrupt handlers */
gatedesc_t gate_tab[256];			/* idt entry table	*/
segdesc_t gdt_tab[NR_GDT_ENTRIES];		/* gdt entry table	*/
tss_t tss; 					/* task state segment */
extern u32_t kernel_stack_top;
static u8_t multitasking = 0;			/* tasking mode */
/* exception names to write out in a human-readable format */
const char* exceptions[17] =
{
	"Divide error(fault)",
	"Debugger exception",
	"NMI",
	"Breakpoint exception",
	"INTO",
	"Bounds exception",
	"Invalid Instruction exception",
	"Numeric coprocessor not available",
	"Double fault",
	"Coprocessor operand segment",
	"Invalid task state",
	"Segment not present",
	"Stack fault",
	"General protection fault",
	"Page fault",
	"Numeric coprocessor error",
	"Alignment check"
};  

static void init_desc(u32_t nr, u32_t base, u32_t limit, u8_t access, u8_t gran);
static void init_gate(u32_t vec_nr, u32_t offset, u8_t dpl_type);
static void stackframe_dump(intr_stackframe_t sf);
static int get_intr_priv_level(u32_t cs);

void set_prot_env()
{
	 register int i;

	/* Setup gdt entries	*/
	init_desc(0, 0, 0, 0, 0);
	init_desc(1, 0, 0xffffffff, 0x9A, 0xC); /* 0100	*/
	init_desc(2, 0, 0xffffffff, 0x92, 0xC);
	init_desc(3, 0, 0xffffffff, 0xFA, 0xC);
	init_desc(4, 0, 0xffffffff, 0xF2, 0xC);
	init_desc(TSS_INDEX, (u32_t)&tss, (u32_t)&tss + sizeof(tss), 0xE9, 0x00);
	
	 remap_irq( 32, 40 );
	
	 /* Exceptions					*/
	 init_gate(0,int_addr(divide_exception),		0x8E); 
	 init_gate(1,int_addr(debugger_exception),		0x8E );
    init_gate(2,int_addr(reserved_exception),		0x8E);   
	 init_gate(3,int_addr(breakpoint_exception),		0x8E);
	 init_gate(4,int_addr(into_exception),			0x8E);
	 init_gate(5,int_addr(bounds_exception),		0x8E);
    init_gate(6,int_addr(invalid_op_exception),		0x8E); 
	 init_gate(7,int_addr(coproc_not_exception),		0x8E); 
    init_gate(8,int_addr(double_fault_exception),		0x8E); 
	 init_gate(9,int_addr(coproc_seg_exception),		0x8E);
	 init_gate(10,int_addr(invalid_tss_exception),		0x8E);
	 init_gate(11,int_addr(segnot_present_exception),	0x8E);
	 init_gate(12,int_addr(stack_fault_exception),		0x8E);
	 init_gate(13,int_addr(general_protection_exception),	0x8E);
	 init_gate(14,int_addr(page_fault_exception),		0x8E);
	 init_gate(15,int_addr(reserved_exception),		0x8E);
	 init_gate(16,int_addr(num_coproc_exception),		0x8E);
	 
	 /* Reserved for Intel use it later */
	 init_gate(17,int_addr(reserved_exception),0x8E);
	 init_gate(18,int_addr(reserved_exception),0x8E);
	 init_gate(19,int_addr(reserved_exception),0x8E);
	 init_gate(20,int_addr(reserved_exception),0x8E);
	 init_gate(21,int_addr(reserved_exception),0x8E);
	 init_gate(22,int_addr(reserved_exception),0x8E);
	 init_gate(23,int_addr(reserved_exception),0x8E);
	 init_gate(24,int_addr(reserved_exception),0x8E);
	 init_gate(25,int_addr(reserved_exception),0x8E);
	 init_gate(26,int_addr(reserved_exception),0x8E);
	 init_gate(27,int_addr(reserved_exception),0x8E);
	 init_gate(28,int_addr(reserved_exception),0x8E);
	 init_gate(29,int_addr(reserved_exception),0x8E);
	 init_gate(30,int_addr(reserved_exception),0x8E);
	 init_gate(31,int_addr(reserved_exception),0x8E);
	 
	 /* IRQ Master */
	 init_gate(32,int_addr(hw_interrupt0),0x8E);
	 init_gate(33,int_addr(hw_interrupt1),0x8E);
	 init_gate(34,int_addr(hw_interrupt2),0x8E);
	 init_gate(35,int_addr(hw_interrupt3),0x8E);
	 init_gate(36,int_addr(hw_interrupt4),0x8E);
	 init_gate(37,int_addr(hw_interrupt5),0x8E);
	 init_gate(38,int_addr(hw_interrupt6),0x8E);
	 init_gate(39,int_addr(hw_interrupt7),0x8E);
	 
	 /* IRQ Slave */
	init_gate(40,int_addr(hw_interrupt8),0x8E);
	init_gate(41,int_addr(hw_interrupt9),0x8E);
	init_gate(42,int_addr(hw_interrupt10),0x8E);
	init_gate(43,int_addr(hw_interrupt11),0x8E);
	init_gate(44,int_addr(hw_interrupt12),0x8E);
	init_gate(45,int_addr(hw_interrupt13),0x8E);
	init_gate(46,int_addr(hw_interrupt14),0x8E);
	init_gate(47,int_addr(hw_interrupt15),0x8E);

	/* fill in the table up to 255	*/	
	for(i = 48; i < NR_INT_VECS; ++i) {
		if(i != SYS_CALL_INT_NUM) {
			init_gate(i,int_addr(unused), 0x8E);
		}
	}
	
	/* system call gate we set up separately	*/
	init_gate(SYS_CALL_INT_NUM, int_addr(syscall),0xEF);	
	
	/* Fill high level interrupt handler table with NULL function ptr	*/
	ZERO_HANDLERS;
	
	/* Setup the TSS entry							*/
	setup_tss(0x10, 1024*1024*32);
}

void setup_tss(u16_t ss0, u32_t esp0) {
	tss_descriptor_t *tdp;

	u32_t base, limit;
	memset((void*)&tss, 0, sizeof(tss_t));
	
	tss.ss0 = ss0;
	tss.esp0 = esp0;
	tss.cs = 0x8 + 3;
	tss.ss = tss.es = tss.fs = tss.gs = tss.ds = 0x10 + 3;	
}

inline void switch_tss(u32_t esp0) {
	tss.ss0 = 0x10;
	tss.esp0 = esp0;
	tss.cs = 0x8 + 3;
	tss.ss = tss.es = tss.fs = tss.gs = tss.ds = 0x10 + 3;
}

void getpid_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx) {
	do_getpid();
}

void io_request_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx) {
	struct io_request* iorp = (struct io_request*)ebx;	
	if(multitasking == 2) {
		iorp->reqor_pid = sched_get_curr_proc()->pid;
	}
		
	serve_ior(iorp);
}

void read_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx) {
	do_read((int)ebx, (void*)ecx, (size_t)edx);
}

void open_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx) {
	do_open((char*)ebx, (mode_t)ecx);
}

void close_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx) {
	do_close((int)ebx);
}

void lseek_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx) {
	do_lseek((int)ebx, (off_t)ecx);
}

void exit_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx) {
	do_exit((int)ebx);
}

void write_cons_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx) {
	do_write_cons((char*)edx);
}

void sys_call(intr_stackframe_t stackframe) {	
	if((stackframe.eax >= 0) && (stackframe.eax < NR_SYS_CALLS)) {
		if(sys_call_handler[stackframe.eax] == NULL_SYSCALL_HANDLER) {
			panic("Not implemented system call service number!");
		}
		
		/* SYS_IOREQ is an in kernel system call. From user-mode it is not allowed! */
		if(stackframe.eax == SYS_IOREQ && get_intr_priv_level(stackframe.cs) != PRIV_LEV_KERN) {
			panic("Restricted system call service!");
		}
	
		(void)sys_call_handler[stackframe.eax](stackframe.ebx, 
				      	stackframe.ecx, stackframe.edx);
				   
		complete_syscall();
			   
	} else {
		panic("Illegal system call request!");
	}
}

static void stackframe_dump(intr_stackframe_t sf) {
	printf("INT_NO = %u\nEAX = %u\nEBX = %u\nECX = %u\nEDX = %u\nESI = %u\nEDI = %u\nESP = %u\nEBP = %u\nEIP = %u\nCS = %u\nDS = %u\nERROR_CODE = %u\nES = %u\nFS = %u\nrest = %u\nold_esp = %u\nss = %u\n",
		 sf.int_no, sf.eax, sf.ebx, sf.ecx, sf.edx, sf.esi, sf.edi, sf.esp,
		 sf.ebp, sf.eip, sf.cs, sf.ds, sf.error_code, sf.es, sf.fs,
		 sf.restore_kstack_esp, sf.old_esp, sf.ss);
	printf("CPL = 0%d\n", sf.cs & 0x3);
}

void panic(char* msg)
{
	lock();
	printf("\nKernel panic : %s\n", msg);	
	/* a real uninterruptible inifinite loop :-) */
	while(1)  asm volatile("hlt");
}

static void ack_int(u32_t int_no) {
	if(int_no >= 40) {
		outportb(0xA0, 0x20);
	}
	
	outportb(0x20,0x20);
}

static inline int user_mode(u32_t cs) {
	return (cs & 0x3);
}

/* When multitasking is 0, we restore the current
 * on-the-stack context, because we are in kernel
 * mode.
 * When multitasking is 1, it means we are ready 
 * to start multitasking, and when we receive 
 * the first timer interrupt, we load the context
 * of the scheduled process. And then we change
 * the state to 2 implicitly.
 * If multitasking is 2, we are in multitasking mode 
 * so in every interrupt, we save the context of the
 * running process, and at the end of the handler
 * we restore one of the processes context. 
 * NOTE: states are in connection with handling
 * of save and restore of contexts. If we
 * are in 0, in kernel mode, it is possible we
 * havent initialized the scheduler, the process
 * handler, etc, but want to receive an interrupt for
 * some reasons! So space where we can store the context
 * may be undefined. In state 1, the space is surely 
 * defined, but save would be a bad choice, because we
 * rewrite the prepared user context with the kernel context
 * of the last instruction which want to start the multitasking :-)
 * So in state 1 no save is performed, just the load of the new
 * context!
 * In state 2 we perform save and restore too. 
 */ 
void set_multitasking_mode(u32_t mode) {
	lock();
	multitasking = mode;
	unlock();
}

static int get_intr_priv_level(u32_t cs) {
	if((cs & PRIV_MASK) == PRIV_LEV_USER) {
		return PRIV_LEV_USER;
	} else if((cs & PRIV_MASK) == PRIV_LEV_KERN) {
		return PRIV_LEV_KERN;		
	}	else {
		printf("invalid cs value :: %d\n", cs);
		panic("STOP!\n");
	}
}

void global_intr_handler(intr_stackframe_t stackframe)
{
	/* Acknowledge the interrupt to the PIC */
	ack_int(stackframe.int_no);	

	if(multitasking == 2) {
		sched_get_curr_proc()->ec.intr_level += 1;
	}
	
	if(!multitasking) goto handle;
	if(multitasking == 1 && stackframe.int_no == TIMER_INT_NUM) {		
		multitasking = 2;
		goto reload;
	} else if(multitasking == 1 && stackframe.int_no != TIMER_INT_NUM) {
		goto handle;
	}		
	
	/* Setup the pointers to the saved context on the stack
	 * in order to access them correctly later.
	 */
	if(get_intr_priv_level(stackframe.cs) == PRIV_LEV_USER) {
		sched_get_curr_proc()->ec.user_context = 
		(intr_stackframe_t*)stackframe.restore_kstack_esp;
	} else {
		sched_get_curr_proc()->ec.kernel_context =
		(intr_stackframe_t*)stackframe.restore_kstack_esp;
	}
	
handle: 
	if(intr_handlers[stackframe.int_no] != NULL_HANDLER) 
		intr_handlers[stackframe.int_no](stackframe);
	else {
		/* Exception */
		if(stackframe.int_no <= 16 && stackframe.int_no != PAGE_FAULT_NUM) {
			if(multitasking == 2) {
				sched_hold();
				//kill process
			} else {							
				panic((char*)exceptions[stackframe.int_no]);
			}
		} else if(stackframe.int_no == PAGE_FAULT_NUM) {									
			u32_t faddr = read_cr2();
			if(handle_page_fault(&(sched_get_curr_proc()->vm), read_cr2()) == ILLEGAL_ACCESS) {								     /* kill the process */
				sched_hold();
			}
		}		
	
		/* System Call */
		if(stackframe.int_no == SYS_CALL_INT_NUM) sys_call(stackframe);		
	}

	if(multitasking == 2) {
		if(sched_get_curr_proc()->ec.intr_level != 0)
			sched_get_curr_proc()->ec.intr_level -= 1;
	}
	
	reload :														
		if(multitasking == 0 || multitasking == 1) {
			/* restart execution context which was interrupted */
			context_handler(stackframe.restore_kstack_esp, 0, 0);
		}

	/* finished with handling the system call or the interrupt */
	if(sched_get_curr_proc()->ec.intr_level == 0) {
		switch_tss(sched_get_curr_proc()->ec.kernel_tss_esp);
		context_handler(sched_get_curr_proc()->ec.user_context->restore_kstack_esp, 1, (u32_t)sched_get_curr_proc()->vm.pd);	
	/* continue the job in kernel mode */
	} else {	
		context_handler(sched_get_curr_proc()->ec.kernel_context->restore_kstack_esp, 0, 0);
	}	
}

static void init_gate(u32_t vec_nr, u32_t offset, u8_t dpl_type)
{
	gatedesc_t* gp = &gate_tab[vec_nr];
	gp->offset_low = offset & 0xFFFF;
	gp->selector = 0x08;
	gp->p_dpl_type = dpl_type ;   
	gp->offset_high = (offset >> 16) & 0xFFFF;
	gp->pad = 0;
}

static void init_desc(u32_t nr, u32_t base, u32_t limit, u8_t access, u8_t gran) {
	segdesc_t * sp = &gdt_tab[nr];
	sp->limit_low = limit & 0xffff;
	sp->base_low = base & 0xffff;
	sp->base_middle = (base >> 16) & 0xff;
	sp->access = access;
	sp->base_high = (base >> 24) & 0xff;
	sp->granularity = ((gran & 0xf) << 4) | ((limit >> 16) & 0xf);
}

void remap_irq( int pic1, int pic2)
{	
	lock();
	/* send ICW1 */
	outportb(PIC1, ICW1); 
	outportb(PIC2, ICW1); 

	/* send ICW2 */
	outportb(PIC1 + 1, pic1);	/* remap */
	outportb(PIC2 + 1, pic2);	/*  pics */

	/* send ICW3 */
	outportb(PIC1 + 1, 4);	/* IRQ2 -> connection to slave */
	outportb(PIC2 + 1, 2);

	/* send ICW4 */
	outportb(PIC1 + 1, ICW4);
	outportb(PIC2 + 1, ICW4);
	
	outportb(0x21, 0);
	outportb(0xA1, 0);

	/* disable all IRQs */
	outportb(PIC1_MASK_REG, 0xFF);
	outportb(PIC2_MASK_REG, 0xFF);
	
	unlock();
}

void register_intr_handler(u8_t int_no, intr_handler_t func) {
	lock();
	intr_handlers[int_no] = func;
	unlock();
}

void unregister_intr_handler(u8_t int_no) {
	lock();
	intr_handlers[int_no] = NULL_HANDLER;
	unlock();
}

inline void lock() {
	asm volatile("cli");
}

inline void unlock() {
	asm volatile("sti");
}

void enable_irq(u8_t irq) {
	if(irq <= 15) {
		lock();
		if(irq < 8) {
			outportb(PIC1_MASK_REG, inportb(PIC1_MASK_REG) & ~(1 << irq));
		} else {
			outportb(PIC2_MASK_REG, inportb(PIC2_MASK_REG) & ~(1 << (irq-8)));
		}
		unlock();
	}
}

void disable_irq(u8_t irq) {
	if(irq <= 15) {
		lock();
		if(irq < 8) {
			outportb(PIC1_MASK_REG, inportb(PIC1_MASK_REG) | (1 << irq));
		} else {
			outportb(PIC2_MASK_REG, inportb(PIC2_MASK_REG) | (1 << (irq-8)));
		}
		unlock();
	}	
}

void register_dump() {
	reg_t regs;
	
	register_state(&regs);
	printf("\neax = %p\nebx = %p\necx = %p\nedx = %p\nesi = %p\nedi = %p\nesp = %p\nebp = %p\n",
		regs.eax, regs.ebx, regs.ecx, regs.edx, regs.esi, regs.edi, regs.esp, regs.ebp);
}

