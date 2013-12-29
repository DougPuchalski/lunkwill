#ifndef __HTML_H__
#define __HTML_H__

#include "lwutils.h"

#define HEADER_OPEN "<div id='HEAD' class='header' style='white-space:nowrap;'>"
#define HEADER_CLOSE "</div><br>"

#define SIDEBAR_OPEN "<div style='display:inline-block;width:20%;vertical-align:top;" \
	"white-space:normal;' id='SIDEBAR' class='sidebar'>"
#define SIDEBAR_CLOSE "</div>"

#define MAIN_OPEN "<div style='display:inline-block;width:79%; white-space: normal;vertical-align:top;' "\
	"id='MAIN' class='main'>"
#define MAIN_CLOSE "</div>"


/** \brief "html_tag" contains a basic structure to represent xml-like files*/
struct html_tag
{
	char *tag_open;
	int tag_open_len;

	char *tag_content_string;
	int tag_content_string_len;

	void *tag_embedded_tags;

	char *tag_close;
	int tag_close_len;

	void *next_tag;
};

typedef struct html_tag HTML_TAG;

/** \brief "html_ui" is required for module output */
struct html_ui
{
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
extern void *html_a_tag(void **parent, char *tag_open, int tag_open_len, char* content_string, int content_string_len, char *tag_close, int tag_close_len);

#define html_add_tag(p, to, cs, tc ) \
 html_a_tag(p, (char *)to, (to!=NULL)?(int)sizeof(to)/sizeof(to[0]):0, \
 (char *)cs, (cs!=NULL)?(int)sizeof(cs)/sizeof(cs[0]):0, \
 (char *)tc, (tc!=NULL)?(int)sizeof(tc)/sizeof(tc[0]):0)

/** \brief Escapes every char. String has to be freed afterwards */
extern char *html_escape(char *string_ptr);

/** \brief Escapes every char. String has to be freed afterwards */
extern char *html_nescape(char *string_ptr, int string_len);

#endif
