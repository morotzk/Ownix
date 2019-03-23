#include <kheap.h>
#include <printf.h>

struct memory_fragment* kheap_first;
static int initialized = 0;

void init_heap(char* memory_base, size_t size) {
	if(initialized == 1)
		return;
		
	if((void*)memory_base == NULL)
		return;
		
	kheap_first = (struct memory_fragment*)memory_base;
	kheap_first->base = (unsigned long)memory_base + sizeof(struct memory_fragment);
	kheap_first->prev = NULL_MEMORY_FRAGMENT;
	kheap_first->next = NULL_MEMORY_FRAGMENT;
	kheap_first->size = size-sizeof(struct memory_fragment);
	kheap_first->free = 1;
	
	initialized = 1;
}

void* alloc_memory_fragment(size_t size) {
	if(initialized != 1)
		return NULL;
		
	/* We need memory for the memory fragment struct + size of payload data	*/
	size_t total_size = size + sizeof(struct memory_fragment);
	
	struct memory_fragment* actual_fragment = kheap_first;
	struct memory_fragment* new_fragment;
	struct memory_fragment prev_fragment;
	
	if(actual_fragment == NULL_MEMORY_FRAGMENT) {
		return NULL;
	}

	while(actual_fragment != NULL_MEMORY_FRAGMENT) {
		if(actual_fragment->size >= total_size && actual_fragment->free == 1) {	
			/* Found a memory fragment with enough size for us */
			if(actual_fragment->size > total_size) {
				prev_fragment = *actual_fragment;
			
				/* Create the new fragment with the given size	*/
				actual_fragment->size = size;
				actual_fragment->free = 0;
			
				/* Decrease the previous big fragment's size	*/
				new_fragment = (struct memory_fragment*)(actual_fragment->base + actual_fragment->size);
				new_fragment->base = (actual_fragment->base + actual_fragment->size) + sizeof(struct memory_fragment);
				new_fragment->size = prev_fragment.size - size - sizeof(struct memory_fragment);
				new_fragment->free = 1;
				
				actual_fragment->next = new_fragment;
				new_fragment->prev = actual_fragment;
				new_fragment->next = prev_fragment.next;

				return (void*)(actual_fragment->base);	
			 } else {			 	
			 	actual_fragment->free = 0;
			 	return (void*)(actual_fragment->base);			 	
			 }
		}	
	
		actual_fragment = actual_fragment->next;
	}

	return NULL;
}

void free_memory_fragment(void* ptr) {
	if(initialized != 1)
		return;
		
	struct memory_fragment* actual_fragment;
	struct memory_fragment* new_fragment;
	
	actual_fragment = kheap_first;
	
	if(actual_fragment == NULL_MEMORY_FRAGMENT)
		return;
		
	while(actual_fragment != NULL_MEMORY_FRAGMENT) {
		if(actual_fragment->base ==(unsigned long)ptr) {
			/* Can we coalesce this fragment with the previous
			 * and/or with the next memory fragment?
			 */
			 if(actual_fragment->prev != NULL_MEMORY_FRAGMENT && actual_fragment->prev->free == 1) {			 	
			 	actual_fragment->prev->size += actual_fragment->size + sizeof(struct memory_fragment);
			 	actual_fragment->prev->next = actual_fragment->next;
			 	new_fragment = actual_fragment->prev;
			 	new_fragment->free = 1;
			 	memset((void*)actual_fragment, 0, sizeof(struct memory_fragment) + actual_fragment->size);
			 	
			 	if(new_fragment->next != NULL_MEMORY_FRAGMENT && new_fragment->next->free == 1) {
			 		new_fragment->size += new_fragment->next->size + sizeof(struct memory_fragment);
			 		new_fragment->next = new_fragment->next->next;
			 		new_fragment->free = 1;
			 	} 	
			 } else if(actual_fragment->next != NULL_MEMORY_FRAGMENT && actual_fragment->next->free == 1) {		 	
			 	actual_fragment->size += actual_fragment->next->size + sizeof(struct memory_fragment);		
			 	actual_fragment->next = actual_fragment->next->next;
		
			 	if(actual_fragment->next != NULL_MEMORY_FRAGMENT)
			 		actual_fragment->next->prev = actual_fragment;			 	
		
			 	actual_fragment->free = 1;		
			 	memset((void*)actual_fragment->base, 0, actual_fragment->size);		
			 } else {			 	
			 	actual_fragment->free = 1;
			 	memset((void*)actual_fragment->base, 0, actual_fragment->size);	
			 }
			 
			 break;
		}
	
		actual_fragment = actual_fragment->next;
	}
}

/* We allow only page-aligned base addresses!	*/
int add_to_heap(char* memory_base, size_t size) {
	if(initialized != 1)
		return ENOINIT;

	if((unsigned long)memory_base % 0x1000 != 0)
		return ENOPAGEALIGN;
		
	if((unsigned long)size % 0x1000 != 0)
		return ENOPAGEALIGN;
	
	struct memory_fragment* new_fragment;
	struct memory_fragment* act = kheap_first;
	while(act->next != NULL_MEMORY_FRAGMENT)
		act = act->next;
		
	new_fragment = (struct memory_fragment*)memory_base;
	new_fragment->base = (unsigned long)memory_base + sizeof(struct memory_fragment);
	new_fragment->prev = act;
	new_fragment->next = NULL_MEMORY_FRAGMENT;
	new_fragment->size = size - sizeof(struct memory_fragment);
	new_fragment->free = 1;	
	
	act->next = new_fragment;
	
	return 0;	
}

