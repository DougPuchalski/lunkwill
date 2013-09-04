#ifndef __SERVER_H__
#define __SERVER_H__

#include "fifo.h" //For _fifo

extern pthread_mutex_t lock_send;
extern int send_fd;

extern pthread_mutex_t lock_count;
extern int thread_count;

extern struct _fifo *jobs;


extern int strnmatch(char *a, char *b, int n);

/** \brief Server which accept clients and pass them to the worker fork */
extern int start_server(int port, int listen_queue, int timeout, int fd_ro, int fd_wr);

/** \brief Thread spawner */
extern int start_worker(int max_num_threads, int fd_ro, int fd_wr);

#endif
