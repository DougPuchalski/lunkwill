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

#include "server.h"
#include "../modules/modules.h"

#define nfree(a) if(a!=NULL){free(a); a=NULL;}

#define strbegin(a,b) strncmp(a,b,strlen(b))

#ifndef NO_DBG
	extern FILE *stddebug;
	#define dbgprintf(a,...) fprintf (stddebug, "\033[%d;%dm\nPID:%d THRD:%d %s:%d: " a, \
				getpid()%8+30, 47-getpid()%8, getpid(), (int)(pthread_self()&0xFF), \
				__FILE__, __LINE__, __VA_ARGS__); fflush(stddebug);
#else
	#define dbgprintf(a, ...)
#endif


#define BUF_SIZE 2048

#define HTTP_451 "HTTP/1.0 451 Unavailable For Legal Reasons\r\nContent-Type:text/html\r\nPragma: no-cache\r\nCache-Control: no-store\r\n\r\n" \
"<html><canvas id=c></canvas><script type='text/javascript'>"\
"with(document.getElementById('c')){height=Math.max(document.body.clientHeight-20,window.innerHeight-20);width=Math.max(document.body.clientWidth-20,window.innerWidth-20); h=9; c=getContext('2d'); c.globalAlpha=.5; a=setInterval(\"c.font='bold 25px sans-serif',c.fillText('We do not forgive. We do not forget. Expect us.',h,h),c.rotate(h++)\",15);setTimeout(function(){clearInterval(a);},10000);}" \
"</script><body bgcolor=#FF1111></body></html>"

#define HTTP_500 "HTTP/1.0 500 Internal Server Error \r\nContent-Type:text/html\r\nPragma: no-cache\r\nCache-Control: no-store\r\n\r\n" \
"<html><body>"\
"<h1>Server Error</h1><br>"\
"<h2>This problem will <a href='http://en.wikipedia.org/wiki/Infinite_monkey_theorem'>almost surely</a> be fixed!</h2>"\
"</body></html>"

#define url_chars "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
				"abcdefghijklmnopqrstuvwxyz" \
				"1234567890.,"

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
	char *data;	
};

/** \brief Struct for linked list */
struct _fifo {
        void *data;
        struct _fifo *next;
        struct _fifo *last;
};

extern int exit_server;

//FIRST IN > FIRST OUT LIST
extern int fifo_push(struct _fifo **fifo, void *data);
extern void *fifo_pop(struct _fifo **fifo);

//SIGNAL HANDLER
extern void init_sighndlr();
extern void sighndlr_add(void *(*func)(void *), void *param);
extern void sighndlr_safe_exit(int param);

//LISTENER
int start_server(int port, int listen_queue, int timeout, int fd_ro, int fd_wr);

//WORKER
int start_worker(int max_num_threads, int fd_ro, int fd_wr);

//HTTP HEADER
extern int send_file(char **buffer, char *file_path);
extern int get_mime(char *file_path);
extern int send_string(char **buffer, char *string);


//HTML BUILDER
struct html_ui *new_html();
extern char *html_flush(void **html, int follow);
extern void *html_add_tag(void **parent, char *tag_open, char* content_string, char *tag_close );


#endif
