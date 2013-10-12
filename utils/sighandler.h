#ifndef __SIGHANDLER_H__
#define __SIGHANDLER_H__

#include "lwutils.h"

/** \brief Struct for signalhandling */
typedef struct{
	void *(*func)(void *);
	void *param;
	void *next;
} sighndlr_list;

extern int Exit_Server;
extern struct _fifo *Sighandler;

/** \brief Set up signalhandlers */
extern void init_sighndlr();

/** \brief Adds a new functionn to the queue.
 * The function will be executed on SIGINT or SIGTERM */
extern void sighndlr_add(void *(*func)(void *), void *param);

/** \brief Sets Exit_Server. */
extern void sighndlr_safe_exit(int param);

/** \brief Works throu queue. */
extern void atexit_safe_exit();

#endif
