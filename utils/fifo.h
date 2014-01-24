#ifndef __FIFO_H__
#define __FIFO_H__

#include "lwutils.h"

/** \brief Struct for linked list */
typedef struct fifo_t
{
	void *data;
	struct fifo_t *next;
	struct fifo_t *last;
} fifo_t;

//FIRST IN > FIRST OUT LIST
/** \brief Adds an element to a linked list
 *	(*fifo) must point to NULL on first call for proper initialization */
extern int fifo_push(fifo_t **fifo, void *data);

/** \brief Returns the first datapointer. It won't be freed the user */
extern void *fifo_pop(fifo_t **fifo);

/** \brief Helperfunction for fifo_push. It creates and initializes the struct */
extern fifo_t *fifo_init(void *data);

#endif
