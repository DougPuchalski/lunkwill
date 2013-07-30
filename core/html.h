#ifndef __HTML_H__
#define __HTML_H__

struct html_tag
{
	char *tag_open;
	char *tag_content_string;
	void *tag_embedded_tags;
	char *tag_close;
	void *next_tag;
};

typedef struct html_tag HTML_TAG;

#define HEADER_OPEN "<div id='HEAD' class='header'>"
#define HEADER_CLOSE "</div>"

#define SIDEBAR_OPEN "<div id='SIDEBAR' class='sidebar'>"
#define SIDEBAR_CLOSE "</div>"

#define MAIN_OPEN "<div id='MAIN' class='main'>"
#define MAIN_CLOSE "</div>"

#endif
