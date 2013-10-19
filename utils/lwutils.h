#ifndef __LWUTILS_H__
#define __LWUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
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
#include <git2.h>


#define nfree(willy) if(willy!=NULL){free(willy); willy=NULL;}

#define strbegin(a,b) strncmp(a,b,strlen(b))

#define BUF_SIZE 4096

#define CONSOLE_LOG

#include "log.h"
#include "fifo.h"
#include "html.h"
#include "http.h"
#include "sighandler.h"
#include "tools.h"
#include "database.h"
#include "structures.h"


#endif

