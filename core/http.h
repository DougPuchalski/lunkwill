#ifndef __HTTP_H__
#define __HTTP_H__

/** \brief Supported MIME types */
enum mime_types{
	MIME_UNKNOWN=0,
	MIME_PLAINTEXT,
	MIME_HTML,
	MIME_JAVASCRIPT,
	MIME_CSS,
	MIME_PNG
};


/** \brief Content-Type for supported MIME types */
const static char *content_types[] = {
	"application/octet-stream",
	"text/plain",
	"text/html",
	"text/javascript",
	"text/css",
	"image/png"
};

#endif
