#ifndef __HTML_H__
#define __HTML_H__

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

#define HEADER_OPEN "<div id='HEAD' class='header'>"
#define HEADER_CLOSE "</div><br>"

#define SIDEBAR_OPEN "<div style='float:left;width:200px' id='SIDEBAR' class='sidebar'><br><br>"
#define SIDEBAR_CLOSE "</div>"

#define MAIN_OPEN "<div style='float:left' id='MAIN' class='main'>"
#define MAIN_CLOSE "</div>"

#endif
