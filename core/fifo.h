#ifndef __FIFO_H__
#define __FIFO_H__

/** \brief Struct for linked list */
struct _fifo {
        void *data;
        struct _fifo *next;
        struct _fifo *last;
};

//FIRST IN > FIRST OUT LIST
/** \brief Adds an element to a linked list
 *	(*fifo) must point to NULL on first call for proper initialization */
extern int fifo_push(struct _fifo **fifo, void *data);

/** \brief Returns the first datapointer. It won't be freed the user */
extern void *fifo_pop(struct _fifo **fifo);

/** \brief Helperfunction for fifo_push. It creates and initializes the struct */
extern struct _fifo *fifo_init(void *data);

#endif
