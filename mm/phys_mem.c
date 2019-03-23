#include <mm/phys_mem.h>
#include <printf.h>

/* Usable (Above 1MB )	physical memory size in Kb */
u32_t physical_memory_size;
struct memory_region physical_memory_map[NR_MEM_REG];
u32_t* free_frame_bmp;
u32_t  nr_user_frames;

void fill_phys_mem_map(u32_t phys_mem_sz) {
	physical_memory_size = phys_mem_sz;
	
	physical_memory_map[PMM_IDX_KSTATIC].base = KERNEL_STATIC_BASE;
	physical_memory_map[PMM_IDX_KSTATIC].size = KERNEL_STATIC_SIZE;
	physical_memory_map[PMM_IDX_KHEAP].base = KERNEL_HEAP_BASE;
	physical_memory_map[PMM_IDX_KHEAP].size = KERNEL_HEAP_SIZE;
	physical_memory_map[PMM_IDX_FREE].base = physical_memory_map[PMM_IDX_KHEAP].base +
		physical_memory_map[PMM_IDX_KHEAP].size; 		
	physical_memory_map[PMM_IDX_FREE].size = physical_memory_size * 1024 - 
		physical_memory_map[PMM_IDX_FREE].base;	
}

u32_t get_phys_mem_size() {
	return physical_memory_size;
}

struct memory_region get_mem_region(u32_t index) {
	if(index < NR_MEM_REG) {
		return physical_memory_map[index];
	}
}

s32_t set_mem_region_base(u32_t index, u32_t base) {
	if(index < NR_MEM_REG) {
		if(physical_memory_map[index].base == DYN_CONF && 
		   index != PMM_IDX_KSTATIC) {
		   	physical_memory_map[index].base = base;
		   	return 0;
		}
	}
	
	return ECONF;
}

s32_t set_mem_region_size(u32_t index, u32_t size) {
	if(index < NR_MEM_REG) {
		if(physical_memory_map[index].size == DYN_CONF && 
		   index != PMM_IDX_KSTATIC) {
		   	physical_memory_map[index].size = size;
		   	return 0;
		}
	}
	
	return ECONF;
}

///////////////////////////////////////////////////////////////////////////////
/*				Page frame operations			     */
///////////////////////////////////////////////////////////////////////////////
void count_free_page_frames() {
	u32_t nr_frame;
	u32_t bmp_sz;
	u32_t i;
	
	nr_frame = (physical_memory_map[PMM_IDX_FREE].size) / PAGE_SIZE;
	bmp_sz = nr_frame / 8;
	free_frame_bmp = (u32_t*)KMALLOC(bmp_sz);
	/* Initially every page frame is free 
	 * (it is the space prepare to use by
	 * user programs. *)
	 */
	nr_user_frames = bmp_sz / 4;
	for(i = 0; i < nr_user_frames; ++i) free_frame_bmp[i] = 0;
}

/* Return the index of the first free page frame. */
u32_t search_free_page_frame() {
	u32_t pfb, pfi;
	/* Iterate over the dwords("page frame block" = pfb) */
	for(pfb = 0; pfb < nr_user_frames; ++pfb) {
		if(free_frame_bmp[pfb] == 0xffffffff) continue;
		
		/* Iterate over the bits in the dword */
		for(pfi = 0; pfi < 32; ++pfi) {
			if(!(free_frame_bmp[pfb] & (0x00000001 << pfi)))
				return  ((pfb * 32 + pfi) + 
					(physical_memory_map[PMM_IDX_FREE].base / PAGE_SIZE));
		}
	}
	
	/* return this when there is no free page frame */
	return NO_FREE_FRAME;
}

void reserve_page_frame(u32_t page_frame_index) {
	u32_t pfb, pfi, corr_pf_index;
	
	corr_pf_index = page_frame_index - (physical_memory_map[PMM_IDX_FREE].base / PAGE_SIZE);	
	pfb = corr_pf_index / 32;
	pfi = corr_pf_index % 32;	
	free_frame_bmp[pfb] |= 0x00000001 << pfi; 
}

void free_page_frame(u32_t page_frame_index) {
	u32_t pfb, pfi, corr_pf_index;
	
	corr_pf_index = page_frame_index - (physical_memory_map[PMM_IDX_FREE].base / PAGE_SIZE);	
	pfb = corr_pf_index / 32;
	pfi = corr_pf_index % 32;	
	free_frame_bmp[pfb] &= ~(0x00000001 << pfi);
}

void zero_page_frame(u32_t page_frame_index) {
	u32_t corr_pf_index;
	
	corr_pf_index = page_frame_index - (physical_memory_map[PMM_IDX_FREE].base / PAGE_SIZE);	
	memset((void*)(corr_pf_index * PAGE_SIZE), 0, PAGE_SIZE);	
}

