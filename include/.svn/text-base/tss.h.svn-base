#ifndef TSS_H
#define TSS_H
#include <sys/types.h>

struct tss {
	/* Fields with prefix "reserved' are reserved for CPU use (later)	*/
	u16_t previous_task_link;
	u16_t reserved0;
	u32_t esp0;
	u16_t ss0;
	u16_t reserved1;
	u32_t esp1;
	u16_t ss1;
	u16_t reserved2;
	u32_t esp2;
	u16_t ss2;
	u16_t reserved3;
	u32_t cr3;
	u32_t eip;
	u32_t eflags;
	u32_t eax;
	u32_t ecx;
	u32_t edx;
	u32_t ebx;
	u32_t esp;
	u32_t ebp;
	u32_t esi;
	u32_t edi;
	u16_t es;
	u16_t reserved4;
	u16_t cs;
	u16_t reserved5;
	u16_t ss;
	u16_t reserved6;
	u16_t ds;
	u16_t reserved7;
	u16_t fs;
	u16_t reserved8;
	u16_t gs;
	u16_t reserved9;
	u16_t ldt_selector;
	u16_t reserved10;
	u16_t io_map_base_address;
} __attribute__((packed));
typedef struct tss tss_t;

/* TSS Segment Descriptor layout(GDT)	*/
struct tss_descriptor {
	/* Segment limit Bits 0..15	*/
	u16_t segment_limit_low;
	
	/* Base address Bits 0..15	*/
	u16_t base_address_low;
	
	/* Base address Bits 16..23	*/
	u8_t base_address_middle;
	
	/* Type, Descriptor Privileg Level, Present	*/
	u8_t segment_flags;
	
	/* Limit Bits 16..19, unused, 0, 0, Granularity	*/
	u8_t limit_gran;
	
	/* Base address Bits 24..31	*/
	u8_t base_address_high;
}__attribute__((packed));

typedef struct tss_descriptor tss_descriptor_t;

#endif
