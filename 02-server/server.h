#ifndef __SERVER_H__
#define __SERVER_H__

#include "../utils/lwutils.h"

/** \brief Struct for pipe rx tx */
struct pipe_rxtx
{
	int fd;
	int size;
	char *data;
};


struct serverwork
{
	pthread_mutex_t lock_send;
	int send_fd;

	pthread_mutex_t lock_count;
	int thread_count;

	struct _fifo *jobs;

};

extern int strnmatch(char *a, char *b, int n);

/** \brief Server which accept clients and pass them to the worker fork */
extern int start_server(int port, int listen_queue, int timeout, int fd_ro, int fd_wr);

/** \brief Thread spawner */
extern int start_worker(int max_num_threads, int fd_ro, int fd_wr);

#endif
