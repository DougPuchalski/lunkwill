#ifndef __LWUTILS_H__
#define __LWUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <netinet/in.h>

#include <pthread.h>   
#include <getopt.h>
#include <libconfig.h>
#include <dlfcn.h>
#include <sqlite3.h> 

#define nfree(willy) if(willy!=NULL){free(willy); willy=NULL;}

#define strbegin(a,b) strncmp(a,b,strlen(b))

#define NO_DBG
#ifndef NO_DBG
	extern FILE *stddebug;
	#define DBGPRINTF(a,...) fprintf (stddebug, "\033[%d;%dm\nPID:%d THRD:%d %s:%d: " a, \
				getpid()%4+30, 43-getpid()%4, getpid(), (int)(pthread_self()), \
				__FILE__, __LINE__, __VA_ARGS__); fflush(stddebug);
#else
	#define DBGPRINTF(a, ...)
#endif

#define BUF_SIZE 4096


#include "fifo.h"
#include "html.h"
#include "http.h"
#include "sighandler.h"
#include "tools.h"
#include "database.h"
#include "structures.h"


#endif

