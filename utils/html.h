#ifndef __HTML_H__
#define __HTML_H__

#include "lwutils.h"

#define HEADER_OPEN "<div id='HEAD' class='header' style='white-space:nowrap;'>"
#define HEADER_CLOSE "</div><br>"

#define SIDEBAR_OPEN "<div style='display:inline-block;width:200px;vertical-align:top;" \
	"white-space:normal;' id='SIDEBAR' class='sidebar'>"
#define SIDEBAR_CLOSE "</div>"

#define MAIN_OPEN "<div style='display:inline-block;white-space: normal;vertical-align:top;' "\
	"id='MAIN' class='main'>"
#define MAIN_CLOSE "</div>"


/** \brief "html_tag" contains a basic structure to represent xml-like files*/
struct html_tag
{
	char *tag_open;
	char *tag_content_string;
	void *tag_embedded_tags;
	char *tag_close;
	void *next_tag;
};

typedef struct html_tag HTML_TAG;

/** \brief "html_ui" is required for module output */
struct html_ui{
	void *base;
	void *header;
	void *sidebar;
	void *main;
};

/** \brief Initializes a new html_ui */
extern struct html_ui *new_html();

/** \brief Turns "HTML_TAG" into an string and free's all elements */
extern char *html_flush(void **html, int follow);

/** \brief Adds a "HTML_TAG" into an existing tag */
extern void *html_add_tag(void **parent, char *tag_open, char* content_string, char *tag_close );

/** \brief Escapes every char. String has to be freed afterwards */
extern char *html_escape(char *string_ptr);

#endif
