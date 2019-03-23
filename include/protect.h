#ifndef PROTECT_H
#define PROTECT_H
#include <sys/types.h>
#include <port_io.h>
#include <sys/system.h>
#include <ph/contexts.h>

#define KERNEL_CS		0x8
#define KERNEL_DS		0x10
#define USER_CS			0x18
#define USER_DS			0x20

#define GDT_START 0
#define IDT_START 5
#define NR_DESC	  5
#define NR_GATE   50  

#define PIC1 0x20 /* Master PIC */
#define PIC2 0xA0 /* Slave PIC */

#define PIC1_MASK_REG	PIC1+1
#define PIC2_MASK_REG	PIC2+1

#define ICW1 0x11
#define ICW4 0x01

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8 and 70, as configured by default */

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define TMR_IRQ		0x0
#define KBD_IRQ		0x1
#define XTW_IRQ		0x5
#define FLPY_IRQ	0x6
#define RTC_IRQ		0x8
#define ATW_IRQ		0xD

#define SYS_CALL_INT_NUM 	0x32
#define TIMER_INT_NUM		0x20
#define KEYBRD_INT_NUM 		0x21

/* After remap interrupts we find corresponding int vector using this macro	*/
#define IRQ_TO_INTNO(irq) (irq+0x20)
/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void remap_irq(int pic1, int pic2);

/* Szegmens deszkriptor struktúra */
typedef struct segdesc
{
	u16_t limit_low;
	u16_t base_low;
	u8_t base_middle;
	u8_t access;
	u8_t granularity;
	u8_t base_high;
} segdesc_t;

/* Null, Kernel Code, Kernel Data, User Code, User Data, TSS	*/
#define NR_GDT_ENTRIES		6

/* Index of TSS descriptor in GDT	*/
#define TSS_INDEX		5

/* Kapu deszkriptor struktúra */
typedef volatile struct gatedesc
{
	u16_t offset_low; 
	u16_t selector;   
	u8_t pad;		  
	u8_t p_dpl_type;  
	u16_t offset_high;					  	
} gatedesc_t;

/* Általános célú regiszterek */
typedef volatile struct reg
{
	u32_t edi;	
	u32_t esi;	
	u32_t ebp;	
	u32_t esp;	
	u32_t ebx;	
	u32_t edx;	
	u32_t ecx;	
	u32_t eax;
} reg_t;

typedef void (*syscall_handler_t)(u32_t, u32_t, u32_t);
typedef void (*intr_handler_t)(intr_stackframe_t stackframe);

#define NULL_SYSCALL_HANDLER			(syscall_handler_t)0

void register_intr_handler(u8_t int_no, intr_handler_t func);
void unregister_intr_handler(u8_t int_no);

/* currently available system call dispatcher functions */
void write_cons_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx);
void getpid_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx);
void io_request_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx);
void read_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx);
void open_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx);
void close_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx);
void lseek_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx);
void exit_syscall_handler(u32_t ebx, u32_t ecx, u32_t edx);
/********************************************************/
void sys_call(intr_stackframe_t stackframe);

static syscall_handler_t sys_call_handler[] = {
	NULL_SYSCALL_HANDLER,					/* 0 */
	exit_syscall_handler,					/* 1 */
	NULL_SYSCALL_HANDLER,					/* 2 */	
	NULL_SYSCALL_HANDLER,					/* 3 */
	write_cons_syscall_handler,			/* 4 */	
	getpid_syscall_handler,					/* 5 */
	io_request_syscall_handler,			/* 6 */
	read_syscall_handler,					/* 7 */
	open_syscall_handler,					/* 8 */
	close_syscall_handler,					/* 9 */
	lseek_syscall_handler,					/* 10 */
	exit_syscall_handler,					/* 11 */
	NULL_SYSCALL_HANDLER,					/* 12 */
	NULL_SYSCALL_HANDLER,					/* 13 */
	NULL_SYSCALL_HANDLER						/* 14 */
};

#define NR_SYS_CALLS (sizeof(sys_call_handler)/sizeof(syscall_handler_t))

#define PAGE_FAULT_NUM 14

/* protect.c */
void set_prot_env();
void global_intr_handler(intr_stackframe_t stackframe);
void panic( char* msg );

/* Set the interrupts enabled flag in EFLAGS to 0 which means every interrupts ignored	*/
void lock();

/* Enable interrupts	*/
void unlock();

/* if IE flag is set, then irq IRQ-line is enabled	*/
void enable_irq(u8_t irq);

/* Disable irq IRQ-line					*/
void disable_irq(u8_t irq);

/* Get the values of 8 general registers		*/
extern void register_state(reg_t *regs);

/* Dump the state of registers				*/
void register_dump();
extern void load_tss();
extern volatile void context_handler(u32_t restore_esp, u32_t paging, u32_t pdaddr);
void set_multitasking_mode(u32_t mode);
/* Setup tss						*/
void setup_tss(u16_t ss0, u32_t esp0);

/* külső függvények */
extern void divide_exception();
extern void debugger_exception();
extern void reserved_exception();
extern void breakpoint_exception();
extern void into_exception();
extern void bounds_exception();
extern void invalid_op_exception();
extern void coproc_not_exception();
extern void double_fault_exception();
extern void coproc_seg_exception();
extern void invalid_tss_exception();
extern void segnot_present_exception();
extern void stack_fault_exception();
extern void general_protection_exception();
extern void page_fault_exception();
extern void num_coproc_exception();
extern void hw_interrupt0();
extern void hw_interrupt1();
extern void hw_interrupt2();
extern void hw_interrupt3();
extern void hw_interrupt4();
extern void hw_interrupt5();
extern void hw_interrupt6();
extern void hw_interrupt7();
extern void hw_interrupt8();
extern void hw_interrupt9();
extern void hw_interrupt10();
extern void hw_interrupt11();
extern void hw_interrupt12();
extern void hw_interrupt13();
extern void hw_interrupt14();
extern void hw_interrupt15();
extern void syscall();
extern void unused();

extern u32_t read_cr2();

#endif
