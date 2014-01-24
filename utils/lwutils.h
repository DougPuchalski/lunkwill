#ifndef __LWUTILS_H__
#define __LWUTILS_H__

#include <stdlib.h>

#define nfree(willy) if(willy!=NULL){free((void *)willy); willy=NULL;}

#define strbegin(a,b) strncmp(a,b,strlen(b))
#define strend(a,b) strcmp(a,(strlen(b)>=strlen(a))?((b)+strlen(b)-strlen(a)):b)

#define BUF_SIZE 4096

#define CONSOLE_LOG

#define HTTP_500 "HTTP/1.0 500 Internal Server Error \r\nContent-Type:text/html\r\nPragma: no-cache\r\nCache-Control: no-store\r\n\r\n" \
"<html><body>"\
"<h1>Server Error</h1><br>"\
"<h2>This problem will <a href='http://en.wikipedia.org/wiki/Infinite_monkey_theorem'>almost surely</a> be fixed!</h2>"\
"</body></html>"

#define URL_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
				"abcdefghijklmnopqrstuvwxyz" \
				"1234567890.,"


extern void *html_add_tag(void*,char*,char*,char*);
extern void *html_flush(void **,int);

/** \brief This struct contains the parsed request */
typedef struct
{

	char session_id[21];
	char project_id[5];
	char module_id[3];
	char module_request[BUF_SIZE];

	int special_file;
	int user;
	int group;
	int session1;
	int session2;
	int project;
	int module;

	void *answer;

} request;

/** \brief Contains all important module infos*/
struct module_info
{
	int id;
	char *name;
	char *description;
	int ((*func)(void *module_data, request *client_request));
	void *data;
};

/** \brief List of all Modules*/
extern struct module_info Modules[256];

#endif

