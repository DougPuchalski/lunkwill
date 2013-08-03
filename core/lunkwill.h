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

#include "../modules/modules.h"

#define nfree(a) free(a);a=NULL;
#ifndef NO_DBG
	#define dbgprintf(a) realdbgprintf ("%s:%s:\t%s\n", __FILE__, __LINE__, a)
#else
	#define dbgprintf(a)
#endif


#define BUF_SIZE 2048
#define HTTP_404 "HTTP/1.0 404 Not Found\r\nContent-Type:text/html\r\nPragma: no-cache\r\nCache-Control: no-store\r\n\r\n"


/** \brief "html_ui" is required for module output */
struct html_ui{
	void *base;
	void *header;
	void *sidebar;
	void *main;
};

/** \brief Supported MIME types */
enum mime_types{
	MIME_UNKNOWN=0,
	MIME_PLAINTEXT,
	MIME_HTML,
	MIME_JAVASCRIPT,
	MIME_CSS,
	MIME_PNG
};

/** \brief Struct for signalhandling */
typedef struct{
	void *(*func)(void *);
	void *param;
	void *next;
} sighndlr_list;

/** \brief Struct for pipe rx tx */
struct pipe_rxtx{
	int fd;
	int size;
	char data[BUF_SIZE];	
};

/** \brief Struct for linked list */
struct _fifo {
        void *data;
        struct _fifo *next;
        struct _fifo *last;
};


//FIRST IN > FIRST OUT LIST
extern int fifo_push(struct _fifo **fifo, void *data);
extern void *fifo_pop(struct _fifo **fifo);

//SIGNAL HANDLER
extern sighndlr_list *sighandler;
extern void init_sighndlr();
extern void sighndlr_add(void *(*func)(void *), void *param);
extern void sighndlr_remove(void *(*func)(void *), void *param);
extern void sighndlr_safe_exit(int param);

//LISTENER
int start_server(int port, int listen_queue, int timeout, int fd_ro, int fd_wr);

//WORKER
int start_worker(int fd_ro, int fd_wr);

//HTTP HEADER
extern int send_file(char **buffer, char *file_path);
extern int get_mime(char *file_path);
extern int send_string(char **buffer, char *string);


//HTML BUILDER
struct html_ui *new_html();
extern char *html_flush(void **html, int follow);
extern void *html_add_tag(void **parent, char *tag_open, char* content_string, char *tag_close );


#endif
