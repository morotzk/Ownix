#ifndef VIRT_MEM_H
#define VIRT_MEM_H
#include <ds/queue.h>
#include <fs/fs.h>

/* How many page entries per paging table(page table, page directory) */
#define ENTRY_PER_TABLE		1024
#define NO_MAP					0
/* Flags for a page entry */
#define FLAG_PRES				0x1
#define FLAG_RW				0x2
#define FLAG_US				0x4
/* Memory mapping type constants */
#define MAP_TYPE_SEG			0
#define MAP_TYPE_STACK		1
#define MAP_TYPE_HEAP		2
#define MAP_TYPE_FILE		3
/* return value constants for handle_page_fault */
#define ILLEGAL_ACCESS		1
#define LEGAL_ACCESS			0

/* macros */
#define PG_PRES(pe)			(pe & FLAG_PRES)	
#define PG_RW(pe)				(pe & FLAG_RW)
#define PG_US(pe)				(pe & FLAG_US)
#define PG_IND(pe)			(pe >> 12)
#define PG_FA(pe)				(pe & 0xFFFFF000)
#define PE(pa,flags)			(pa | flags)

typedef u32_t		pde_t;			/* Type for page directory entry */
typedef u32_t		pte_t;			/* Type for page table entry */
typedef u32_t map_type_t;

/* memory mapping descriptor structure */
struct mmapd {
	map_type_t map_type;
	u32_t va_base;
	u32_t mapped_sz;
	//char* file_path;	
	struct file mmap_file;
	/* base position in the mmapped file from where the mapping start */
	u32_t f_base;
	/* Important when we map CODE pages from
	 * an executable to map it as READ_ONLY!
	 */
	u32_t map_flags;
};

/* virtual memory descriptor structure */
struct vmd {
	pde_t* pd;
	/* list of mmap */
	struct q_node* mmap_list;
};

/* Create a new virtual address space */
pde_t* create_vas();
/* Delete a virtual address space */
void delete_vas(pde_t* pd);
/* Map the vaddr VA to the paddr PA in the VAS described by pd */
void map_to_phys(pde_t* pd, u32_t vaddr, u32_t paddr, u32_t flags);
/* Delete the mapping from vaddr to a page frame and free the frame */
void unmap(pde_t* pd, u32_t vaddr);
/* Get the PA mapped to vaddr in the VAS described by pd */
u32_t virt_to_phys(pde_t* pd, u32_t vaddr);
/* Handle a page fault in VM described by vm */
u32_t handle_page_fault(struct vmd* vm, u32_t fault_addr);
/* Check vaddr is mapped to phys mem if not try to map it */
u32_t vm_mapping(struct vmd* vm, u32_t vaddr);
/* create a new file mapping in vm */
void create_file_map(struct vmd* vm, struct file file, u32_t va_base, u32_t fpos, u32_t msize, u32_t flags);
/* create a new memory mapping in vm with type map_type */
void create_mem_map(struct vmd* vm, map_type_t map_type, u32_t va_base, u32_t msize, u32_t flags);

#endif
