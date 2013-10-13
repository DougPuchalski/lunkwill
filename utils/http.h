#ifndef __HTTP_H__
#define __HTTP_H__

#include "lwutils.h"

#define HTTP_451 "HTTP/1.0 451 Unavailable For Legal Reasons\r\nContent-Type:text/html\r\nPragma: no-cache\r\nCache-Control: no-store\r\n\r\n" \
"<html><canvas id=c></canvas><script type='text/javascript'>"\
"with(document.getElementById('c')){height=Math.max(document.body.clientHeight-20,window.innerHeight-20);width=Math.max(document.body.clientWidth-20,window.innerWidth-20); h=9; c=getContext('2d'); c.globalAlpha=.5; a=setInterval(\"c.font='bold 25px sans-serif',c.fillText('We do not forgive. We do not forget. Expect us.',h,h),c.rotate(h++)\",15);setTimeout(function(){clearInterval(a);},10000);}" \
"eraseCookie('login')"\
"</script><body bgcolor=#FF1111></body></html>"

#define HTTP_500 "HTTP/1.0 500 Internal Server Error \r\nContent-Type:text/html\r\nPragma: no-cache\r\nCache-Control: no-store\r\n\r\n" \
"<html><body>"\
"<h1>Server Error</h1><br>"\
"<h2>This problem will <a href='http://en.wikipedia.org/wiki/Infinite_monkey_theorem'>almost surely</a> be fixed!</h2>"\
"</body></html>"

#define URL_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
				"abcdefghijklmnopqrstuvwxyz" \
				"1234567890.,"


/** \brief Content-Type for supported MIME types */
const static char *content_types[] __attribute__ ((unused)) = {
	"application/octet-stream",
	"text/plain",
	"text/html",
	"text/javascript",
	"text/css",
	"image/png",
	"image/x-icon"
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

/** \brief Fills the buffer with an HTTP file request answer 
 *  \returns The size of the answer to send */
extern int send_file(char **buffer, char *file_path);

/** \brief Get the MIME Type
 *  \returns A mime_types value */
extern int get_mime(char *file_path);

/** \brief Fills the buffer with an HTTP answer containing a string
 *  \returns The size of the answer to send */
extern int send_string(char **buffer, char *string);

/** \brief Copy string new allocated string, e.g. for sending error message
 *  \returns The size of the answer to send */ 
extern int send_raw(char **buffer, char *string);

#endif
