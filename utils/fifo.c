#include "fifo.h"

fifo_t *fifo_init(void *data)
{
	fifo_t *fifo;
	fifo=malloc(sizeof(fifo_t));

	fifo->next=NULL;
	fifo->last=fifo;
	fifo->data=data;

	return fifo;
}

int fifo_push(fifo_t **fifo, void *data)
{
	fifo_t *new_fifo;
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

void *fifo_pop(fifo_t **fifo)
{
	void *ret;
	void *v;

	if((*fifo)==NULL)
	{
		return NULL;
	}

	ret=(*fifo)->data;

	if((*fifo)->next!=NULL)(*fifo)->next->last=(*fifo)->last;
	v=(*fifo)->next;
	nfree((*fifo));
	(*fifo)=v;

	return ret;
}
