#ifndef CONTEXTS_H
#define CONTEXTS_H
#include <sys/types.h>
#include <fs/fs.h>
#include <mm/virt_mem.h>

/* Interrupt stack frame */
struct intr_stackframe
{	
	u32_t int_no;	//esp	
	/* General reisters from pushad 		*/
	u32_t edi;  //esp+4	
	u32_t esi;	//esp+8
	u32_t ebp;	//esp+12
	u32_t esp;	//esp+16
	u32_t ebx;	//esp+20
	u32_t edx;	//esp+24
	u32_t ecx;	//esp+28
	u32_t eax; 	//esp+32		
	/* Segment registers before interrupt fired	*/			
	u32_t gs;	//esp+36	
	u32_t fs;	//esp+40	
	u32_t es;	//esp+44	
	u32_t ds; 	//esp+48	
	u32_t restore_kstack_esp; //esp+52	
	/* Pushed error code		*/		
	u32_t error_code;	  //esp+56	
	/* Instruction pointer before interrupt fired	*/
	u32_t eip;		  //esp+60	
	/* Code segment register before interrupt fired	*/	  	
	u32_t cs;		  //esp+64	
	/* Eflags before interrupt fired		*/		
	u32_t eflags;		  //esp+68	
	/* Available only when privilege-level change(i.e from user-mode to supervisor-mode!)	*/	
	u32_t old_esp;		  //esp+72
	u32_t ss;		  //esp+76
};

typedef struct intr_stackframe intr_stackframe_t;

struct sched_info {
	u16_t default_quantum;
	u16_t pending_quantum;
	u8_t sched_state;
	u8_t waitfor;	
};

struct exec_cntxt {
	u32_t kernel_tss_esp;	
	u32_t restore_kstack_esp;	
	u32_t user_stack_top;				/* The initial stack top of the process */		
	struct intr_stackframe* kernel_context;
	struct intr_stackframe* user_context;	
	BIT_T(kernel_mode);		
	s32_t intr_level;						/* Level of interrupt */
};

#define NULL_EX_CNTXT	(struct exec_cntxt*)0

struct proc_context {	
	char proc_name[64];
	pid_t pid;					/* process's own pid */	
	pid_t ppid;					/* parent process's pid */		
	struct exec_cntxt ec;	/* the execution/scheduling context of the process */
	struct sched_info schedinfo;	/* process's scheduler specific info */
	struct file_tab ftab;	/* table of open file descriptors */
	struct vmd vm;	/* the virtual memory descriptor of the process */
};

#endif

