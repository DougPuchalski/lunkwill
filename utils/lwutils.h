#ifndef __LWUTILS_H__
#define __LWUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <limits.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <pthread.h>
#include <stdarg.h>
#include <getopt.h>
#include <libconfig.h>
#include <dlfcn.h>
#include <git2.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>


#define nfree(willy) if(willy!=NULL){free((void *)willy); willy=NULL;}

#define strbegin(a,b) strncmp(a,b,strlen(b))
#define strend(a,b) strcmp(a,(strlen(b)>=strlen(a))?((b)+strlen(b)-strlen(a)):b)

#define BUF_SIZE 4096

#define CONSOLE_LOG

#define HTTP_451 "HTTP/1.0 451 Unavailable For Legal Reasons\r\nContent-Type:text/html\r\nPragma: no-cache\r\nCache-Control: no-store\r\n\r\n" \
"<html><canvas id=c></canvas><script type='text/javascript'>"\
"with(document.getElementById('c')){height=Math.max(document.body.clientHeight-20,window.innerHeight-20);width=Math.max(document.body.clientWidth-20,window.innerWidth-20); h=9; c=getContext('2d'); c.globalAlpha=.5; a=setInterval(\"c.font='bold 25px sans-serif',c.fillText('You shall not pass!',h,h),c.rotate(h++)\",15);setTimeout(function(){clearInterval(a);},10000);}" \
"window.setCookie('login','')"\
"</script><body bgcolor=#FF1111></body></html>"

#define HTTP_500 "HTTP/1.0 500 Internal Server Error \r\nContent-Type:text/html\r\nPragma: no-cache\r\nCache-Control: no-store\r\n\r\n" \
"<html><body>"\
"<h1>Server Error</h1><br>"\
"<h2>This problem will <a href='http://en.wikipedia.org/wiki/Infinite_monkey_theorem'>almost surely</a> be fixed!</h2>"\
"</body></html>"

#define URL_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
				"abcdefghijklmnopqrstuvwxyz" \
				"1234567890.,"


#include "log.h"
#include "fifo.h"
#include "base64.h"
#include "html.h"
#include "sighandler.h"
#include "tools.h"
#include "database.h"
#include "structures.h"
#include "stringsearchtree.h"


#endif

