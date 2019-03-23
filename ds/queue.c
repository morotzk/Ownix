#include <ds/queue.h>

struct q_node* q_creat() {
	struct q_node* q_head;
	
	q_head = (struct q_node*)KMALLOC(sizeof(struct q_node));
	return q_head;
}

u32_t q_size(struct q_node* head) {
	struct q_node* q_n;
	u32_t sz = 0;
	
	q_n = head;
	while(q_n != NULL_Q) {
		++sz;
		q_n = q_n->next;
	}
	
	return sz;
}

void q_enq(struct q_node* head, void* value) {
	struct q_node* q_n;

	if(head == NULL_Q) return;
	
	q_n = head;
	while(q_n->next != NULL_Q) q_n = q_n->next;
	q_n->next = (struct q_node*)KMALLOC(sizeof(struct q_node));
	q_n = q_n->next;
	q_n->value = value;
	q_n->next = NULL_Q;
}

struct q_node* q_rem(struct q_node* head) {
	struct q_node* next_head;

	if(head == NULL_Q) return NULL_Q;
	
	next_head = head->next;
	KFREE((void*)head);
	
	return next_head;
}

