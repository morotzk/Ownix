#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <kheap.h>
#include <sys/types.h>

struct circ_q_node {
	void* value;
	struct circ_q_node* next;	
};

#define NULL_C_Q			(struct circ_q_node*)0

void circ_q_enq(struct circ_q_node* q_head, void* value);
struct circ_q_node* circ_q_rem(struct circ_q_node* q_head);
struct circ_q_node* circ_q_creat();

#endif
