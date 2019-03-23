#ifndef QUEUE_H
#define QUEUE_H
#include <kheap.h>
#include <sys/types.h>

struct q_node {
	void* value;
	struct q_node* next;	
};

#define NULL_Q			(struct q_node*)0

/* 	create a new queue 		*/
struct q_node* q_creat();
/* 	get the size of the queue 	*/
u32_t q_size(struct q_node* head);
/* 	enqueue 			*/
void q_enq(struct q_node* head, void* value);
/* 	remove 				*/
struct q_node* q_rem(struct q_node* head);

#endif
