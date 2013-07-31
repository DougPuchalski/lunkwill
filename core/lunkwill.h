#ifndef __LUNKWILL_H__
#define __LUNKWILL_H__

#ifndef __MAIN__
#define EXTERN extern
#else
#define EXTERN
#endif

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


#define BUF_SIZE 1024
#define HTTP_404 "HTTP/1.0 404 Not Found\nContent-Type:text/html\n\n"
#define HTTP_404_MESS "<html><body><h1>FILE NOT FOUND</h1></body></html>"


struct html_ui{
	void *base;
	void *header;
	void *sidebar;
	void *main;
};

EXTERN struct{
	int (*module[63])(void *, struct html_ui*);
    config_t config;
	void *sighndlr;
} session;

/** \brief Supported MIME types */
enum mime_types{
	MIME_UNKNOWN=0,
	MIME_PLAINTEXT,
	MIME_HTML,
	MIME_JAVASCRIPT,
	MIME_CSS,
	MIME_PNG
};


extern int start_server();

extern void init_sighndlr();
extern void sighndlr_add(void *(*func)(void *), void *param);
extern void sighndlr_remove(void *(*func)(void *), void *param);

extern int load_config(char *config_file_name);

extern int send_file(char **buffer, char *file_path);
extern int get_mime(char *file_path);
extern int send_string(char **buffer, char *string);

struct html_ui *new_html();
extern char *html_flush(void **html, int follow);
extern void *html_add_tag(void **parent, char *tag_open, char* content_string, char *tag_close );


#endif
