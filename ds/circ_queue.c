#include <ds/circ_queue.h>

/* Add value to the tail of the queue	*/
void circ_q_enq(struct circ_q_node* q_head, void* value) {
	struct circ_q_node* a_qn = q_head;
	struct circ_q_node* n_qn;
	
	if(a_qn == NULL_C_Q) return;
	
	while(a_qn->next != q_head) a_qn = a_qn->next;	
	
	/* Create the new node	*/
	n_qn = (struct circ_q_node*)KMALLOC(sizeof(struct circ_q_node));
	if((void*)n_qn == NULL) return;
	
	n_qn->value = value;
	n_qn->next = q_head;
	
	/* Link the old tail and the new tail */
	a_qn->next = n_qn;
}

/* Remove the head of the queue	*/
struct circ_q_node* circ_q_rem(struct circ_q_node* q_head) {
	struct circ_q_node* a_qn = q_head;
	struct circ_q_node* n_qn;

	if((void*)a_qn == NULL) return;
	
	/* If queue's length is 1 then just free
	 * allocated space and return.
	 */
	if(a_qn->next == a_qn) {
		KFREE(a_qn);
		return NULL_C_Q;
	}
	
	/* New head of the queue is the current head's next.
	 * Sure it is not null, because the size of the
	 * queue is >= 2, because otherwise control cannot
	 * be passed to here.
	 */
	n_qn = a_qn->next;
	
	/* Find the tail of the queue */
	while(a_qn->next != q_head) a_qn = a_qn->next;
	
	/* Link the new head to the tail of the queue	*/
	a_qn->next = n_qn;
	
	/* Free the old head */
	KFREE((void*)q_head);
	
	return n_qn;
}

/* Create an empty circular queue */
struct circ_q_node* circ_q_creat() {
	struct circ_q_node* n_qhn = (struct circ_q_node*)KMALLOC(sizeof(struct circ_q_node));
	if((void*)n_qhn == NULL) return NULL_C_Q;
	
	n_qhn->next = n_qhn;	
	return n_qhn;
}

