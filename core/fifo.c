#include "lunkwill.h"

struct _fifo *fifo_init(void *data) {
	struct _fifo *fifo;
	fifo=malloc(sizeof(struct _fifo));
	
	fifo->next=NULL;
	fifo->last=fifo;
	fifo->data=data;
	
	return fifo;
};

/** \brief Adds an element to a linked list
 *	(*fifo) must point to NULL on first call for proper initialization */
int fifo_push(struct _fifo **fifo, void *data) {
	struct _fifo *new_fifo;
	new_fifo=fifo_init(data);

	if((*fifo)==NULL)
	{
		(*fifo)=new_fifo;
	}
	else
	{
		(*fifo)->last->next=new_fifo;
		(*fifo)->last=new_fifo;
	}
	return 0;
}

/** \brief Returns the first datapointer. It won't be freed the user */
void *fifo_pop(struct _fifo **fifo) {
	void *ret;
	void *v;
	
	if((*fifo)==NULL) {
		return NULL;
	}

	ret=(*fifo)->data;

	if((*fifo)->next!=NULL)(*fifo)->next->last=(*fifo)->last;
	v=(*fifo)->next;
	nfree((*fifo));
	(*fifo)=v;

	return ret;
}
