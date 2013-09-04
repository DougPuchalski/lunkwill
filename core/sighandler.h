#ifndef __SIGHANDLER_H__
#define __SIGHANDLER_H__

#include "fifo.h" //For _fifo

/** \brief Struct for signalhandling */
typedef struct{
	void *(*func)(void *);
	void *param;
	void *next;
} sighndlr_list;

extern int exit_server;
extern struct _fifo *sighandler;

/** \brief Set up signalhandlers */
extern void init_sighndlr();

/** \brief Adds a new functionn to the queue.
 * The function will be executed on SIGINT or SIGTERM */
extern void sighndlr_add(void *(*func)(void *), void *param);

/** \brief Sets exit_server. */
extern void sighndlr_safe_exit(int param);

/** \brief Works throu queue. */
extern void atexit_safe_exit();

#endif
