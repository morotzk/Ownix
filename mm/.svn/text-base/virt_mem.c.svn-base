#include <mm/virt_mem.h>
#include <mem_util.h>
#include <mm/phys_mem.h>
#include <protect.h>
#include <sys/sys_const.h>
#include <printf.h>

/* Calculate the page directory and page table indexes to vaddr */
static inline void vaddr_to_ptix(u32_t vaddr, u32_t* pde_ix, u32_t* pte_ix);
/* Allocate a page table to a page directory #pde_ix entry and mark as present */
static inline void add_pt(pde_t* pd, u32_t pde_ix);

pde_t* create_vas() {
	pde_t* pd;
	u32_t va;
	u32_t pf;
	
	pf = search_free_page_frame();
	if(pf == NO_FREE_FRAME) panic("Cannot allocate physical memory page!");
	
	reserve_page_frame(pf);
	pd = (pde_t*)(pf * PAGE_SIZE);
	zero_page_frame(pf);
	memset((void*)pd, 0, PAGE_SIZE);
	
	/* Map in the lower 4 Mb of physical memory */
	for(va = 0; va < 4 * 1024 * 1024; va += PAGE_SIZE) {
		map_to_phys(pd, va, va, FLAG_PRES);
	}
	
	return pd;
}

void delete_vas(pde_t* pd) {
	pde_t pde;
	pte_t pte;
	pte_t* pt;
	
	for(pde = 0; pde < ENTRY_PER_TABLE; ++pde) {
		if(PG_PRES(pd[pde])) {
			pt = (pte_t*)PG_FA(pd[pde]);
			for(pte = 0; pte < ENTRY_PER_TABLE; ++pte) {
				if(PG_PRES(pt[pte])) {
					zero_page_frame(PG_IND(pt[pte]));
					free_page_frame(PG_IND(pt[pte]));					
				}
			}
			zero_page_frame(PG_IND(pd[pde]));
			free_page_frame(PG_IND(pd[pde]));
		}
	}
	
	zero_page_frame(PG_IND((u32_t)pd));
	free_page_frame(PG_IND((u32_t)pd));
}

void map_to_phys(pde_t* pd, u32_t vaddr, u32_t paddr, u32_t flags) {
	u32_t pde, pte;
	pte_t* pt;

	if((void*)pd == NULL) 
		panic("Cannot create memory mapping! Page directory not exist!");
		
	/* Get the appropiate page directory and page table index from vaddr */
	vaddr_to_ptix(vaddr, &pde, &pte);	
	if(!PG_PRES(pd[pde])) {		
		/* First create the new page table and map it */
		add_pt(pd, pde);				
	}
	
	/* Now map vaddr to paddr */
	pt = PG_FA(pd[pde]);	
	
	/* NOTE: FLAG_PRES is compulsory, if we want to map, and prevent us from  
	 * programming mistakes, when we miss this flag.
	 */
	pt[pte] = PE(paddr,FLAG_PRES | flags);	
}

void unmap(pde_t* pd, u32_t vaddr) {
	u32_t pde, pte, paddr;
	pte_t* pt;

	if((void*)pd == NULL)
		panic("Cannot unmap, page directory not exist!");
		
	vaddr_to_ptix(vaddr, &pde, &pte);
	
	if(!PG_PRES(pd[pde]))
		panic("Address was not mapped. Trying to delete nonexisting mapping!");
		
	pt = PG_FA(pd[pde]);
	if(!PG_PRES(pt[pte]))
		panic("Address was not mapped. Trying to delete nonexisting mapping!");	
		
	paddr = PG_FA(pt[pte]);
	pt[pte] = PE(0, 0);
	zero_page_frame(PG_IND(paddr));
	free_page_frame(PG_IND(paddr));	
}

u32_t virt_to_phys(pde_t* pd, u32_t vaddr) {
	u32_t pde, pte;
	pte_t* pt;

	if((void*)pd == NULL)
		panic("Cannot get the address mapping. Page directory not exist!\n");
		
	vaddr_to_ptix(vaddr, &pde, &pte);
	if(!PG_PRES(pd[pde])) return NO_MAP;
	
	pt = PG_FA(pd[pde]);
	if(!PG_PRES(pt[pte])) return NO_MAP;
	return PG_FA(pt[pte]) + vaddr % PAGE_SIZE;	
}

static inline void vaddr_to_ptix(u32_t vaddr, u32_t* pde_ix, u32_t* pte_ix) {
	*pde_ix = vaddr >> 22;
	*pte_ix = (vaddr >> 12) & 0x3FF;
}

static inline void add_pt(pde_t* pd, u32_t pde_ix) {
	u32_t pf;
	pte_t* pta;

	if((void*)pd == NULL) 
		panic("Cannot add page table! Page directory not exist!");
		
	pf = search_free_page_frame();
	if(pf == NO_FREE_FRAME)
		panic("Cannot allocate physical memory!");
		
	reserve_page_frame(pf);
	zero_page_frame(pf);	
	pta = (pte_t*)(pf * PAGE_SIZE);	
	pd[pde_ix] = PE((u32_t)pta, FLAG_PRES | FLAG_US | FLAG_RW);			
}

/* This is the most important function when doing 
 * ON-DEMAND PAGING. 
 */
u32_t handle_page_fault(struct vmd* vm, u32_t fault_addr) {
	u32_t pf, phys_addr, f_seek;
	struct q_node* mmap_list; 
	struct mmapd* mmapdp;		
	
	mmap_list = vm->mmap_list;	
	
	/* Try to find a mapping interval in which fault_addr is inside */
	while(mmap_list != NULL_Q) {
		mmapdp = (struct mmapd*)mmap_list->value;
		switch(mmapdp->map_type) {
			case MAP_TYPE_STACK : {
				if((fault_addr <= mmapdp->va_base) && 
				   (fault_addr >= mmapdp->va_base - mmapdp->mapped_sz)) {
				 	/* Map in a free physical page for the stack */
				 	pf = search_free_page_frame();
				 	if(pf == NO_FREE_FRAME) return ILLEGAL_ACCESS;
				 	reserve_page_frame(pf);
				 	phys_addr = pf * PAGE_SIZE;
				 	map_to_phys(vm->pd, fault_addr, phys_addr, FLAG_RW | FLAG_US);
				 	/* Slide the trap stack page below! */
				 	mmapdp->mapped_sz += PAGE_SIZE; 	
				 	return LEGAL_ACCESS;  
				 }				   
			}
			case MAP_TYPE_FILE : {
				if((fault_addr >= mmapdp->va_base) &&
				   (fault_addr <= (mmapdp->va_base + mmapdp->mapped_sz))) {
				   	f_seek = mmapdp->f_base + 
				   		((fault_addr - mmapdp->va_base) / PAGE_SIZE) * PAGE_SIZE;
				   		
				   	pf = search_free_page_frame();
				   	if(pf == NO_FREE_FRAME) return ILLEGAL_ACCESS;
				   	reserve_page_frame(pf);
				   	phys_addr = pf * PAGE_SIZE;				
					
					int success = lseek(&(mmapdp->mmap_file), f_seek);
					if(success != 0) {
						free_page_frame(pf);
						return ILLEGAL_ACCESS;
					}
					success = read(&(mmapdp->mmap_file), (void*)phys_addr, PAGE_SIZE);
					if(success < 0) {							
						free_page_frame(pf);
						return ILLEGAL_ACCESS;
					}
					/* Map in the page frame */
				   	map_to_phys(vm->pd, fault_addr, phys_addr, mmapdp->map_flags | FLAG_US);				   	
				   			   		   
				   	return LEGAL_ACCESS;
				   }
			}
		}
		mmap_list = mmap_list->next;
	}
	
	return ILLEGAL_ACCESS; 
}

u32_t vm_mapping(struct vmd* vm, u32_t vaddr) {	
	if(virt_to_phys(vm->pd, vaddr) != NO_MAP)
		return LEGAL_ACCESS;		
	
	return handle_page_fault(vm, vaddr);
}

void create_file_map(struct vmd* vm, struct file file, u32_t va_base, u32_t fpos, u32_t msize, u32_t flags) {
	struct mmapd* fmmap; 

	if(vm->mmap_list == NULL_Q) {
		vm->mmap_list = q_creat();
		if(vm->mmap_list == NULL_Q) 
			panic("Out of memory!");			
	}
	
	fmmap = (struct mmapd*)KMALLOC(sizeof(struct mmapd));
	if((void*)fmmap == NULL) panic("Out of memory!");
	
	fmmap->map_type = MAP_TYPE_FILE;
	fmmap->va_base = va_base;
	fmmap->mapped_sz = msize;
	fmmap->mmap_file = file;
	fmmap->f_base = fpos;
	fmmap->map_flags = flags;
	
	q_enq(vm->mmap_list, (void*)fmmap);
}

void create_mem_map(struct vmd* vm, map_type_t map_type, u32_t va_base, u32_t msize, u32_t flags) {
	struct mmapd* pmmap; 

	if(vm->mmap_list == NULL_Q) {
		vm->mmap_list = q_creat();
		if(vm->mmap_list == NULL_Q) 
			panic("Out of memory!");			
	}
	
	pmmap = (struct mmapd*)KMALLOC(sizeof(struct mmapd));
	if((void*)pmmap == NULL) panic("Out of memory!");
	
	pmmap->map_type = map_type;
	pmmap->va_base = va_base;
	pmmap->mapped_sz = msize;
	pmmap->map_flags = flags;
	
	q_enq(vm->mmap_list, (void*)pmmap);		
}

