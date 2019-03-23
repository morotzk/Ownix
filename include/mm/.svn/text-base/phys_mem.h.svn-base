#ifndef PHYS_MEM_H
#define PHYS_MEM_H
#include <sys/types.h>
#include <kheap.h>
#include <mem_util.h>

/* We have three predefined memory region */
#define NR_MEM_REG		3

#define KERNEL_STATIC_BASE	(0)
#define KERNEL_STATIC_SIZE	(2*1024*1024)

#define KERNEL_HEAP_BASE	KERNEL_STATIC_SIZE
#define KERNEL_HEAP_SIZE	KERNEL_STATIC_SIZE

/* Memory region will be configured dynamically. */
#define DYN_CONF		((u32_t)0)

#define PMM_IDX_KSTATIC		0
#define PMM_IDX_KHEAP		1
#define PMM_IDX_FREE		2

/* When set_mem_region_*() returns with this
 * const it means the region's field already
 * configured. 
 */
#define ECONF			(-1)

struct memory_region {
	u32_t base;
	u32_t size;
};

#define PAGE_SIZE		0x1000
#define NO_FREE_FRAME		0xffffffff

void fill_phys_mem_map(u32_t phys_mem_sz);
u32_t get_phys_mem_size();
struct memory_region get_mem_region(u32_t index);
s32_t set_mem_region_base(u32_t index, u32_t base);
s32_t set_mem_region_size(u32_t index, u32_t size);

void count_free_page_frames();
u32_t search_free_page_frame();
void reserve_page_frame(u32_t page_frame_index);
void free_page_frame(u32_t page_frame_index);
void zero_page_frame(u32_t page_frame_index);

#endif
