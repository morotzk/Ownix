#ifndef KHEAP_H
#define KHEAP_H
#include <sys/types.h>
#include <sys/sys_const.h>
#include <mem_util.h>

struct memory_fragment {
	unsigned long base;
	unsigned long size;
	struct memory_fragment* next;
	struct memory_fragment* prev;
	BIT_T(free);	
};

#define NULL_MEMORY_FRAGMENT			(struct memory_fragment*)0

#define ENOPAGEALIGN				-1
#define ENOINIT					-2

/* Initialize the heap						*/
void init_heap(char* memory_base, size_t size);
/* Allocate a fragment in the heap bytes size			*/
void* alloc_memory_fragment(size_t size);
/* Free a fragment from the memory points by ptr		*/
void free_memory_fragment(void* ptr);
/* Add a free memory fragment to the heap 			*/
int add_to_heap(char* memory_base, size_t size);

#define KMALLOC(size)				alloc_memory_fragment(size)
#define KFREE(ptr)				free_memory_fragment(ptr)

#endif
