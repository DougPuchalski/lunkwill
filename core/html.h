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

#define HEADER_OPEN "<div id='HEAD' class='header' style='white-space:nowrap;'>"
#define HEADER_CLOSE "</div><br>"

#define SIDEBAR_OPEN "<div style='display:inline-block;width:200px;vertical-align:top;" \
	"white-space:normal;' id='SIDEBAR' class='sidebar'>"
#define SIDEBAR_CLOSE "</div>"

#define MAIN_OPEN "<div style='display:inline-block;white-space: normal;vertical-align:top;'"\
	"id='MAIN' class='main'>"
#define MAIN_CLOSE "</div>"

#endif
