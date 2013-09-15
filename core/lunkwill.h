#ifndef __LUNKWILL_H__
#define __LUNKWILL_H__

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
#include <sqlite3.h> 

#define nfree(a) if(a!=NULL){free(a); a=NULL;}

#define strbegin(a,b) strncmp(a,b,strlen(b))

#define NO_DBG
#ifndef NO_DBG
	extern FILE *stddebug;
	#define dbgprintf(a,...) fprintf (stddebug, "\033[%d;%dm\nPID:%d THRD:%d %s:%d: " a, \
				getpid()%4+30, 43-getpid()%4, getpid(), (int)(pthread_self()), \
				__FILE__, __LINE__, __VA_ARGS__); fflush(stddebug);
#else
	#define dbgprintf(a, ...)
#endif

#define BUF_SIZE 4096

/** \brief Struct for pipe rx tx */
struct pipe_rxtx{
	int fd;
	int size;
	char *data;	
};


/** \brief This struct contains the parsed request */
typedef struct{
	int special_file;
	char session_id[21];
	int user;
	int group;
	int session1;
	int session2;
	char project_id[5];
	int project;
	char module_id[3];
	int module;
	char module_request[BUF_SIZE];
	void *answer;
} request; 

/** \brief Main database handle */
sqlite3 *db_handle;
int db_status;

#endif
