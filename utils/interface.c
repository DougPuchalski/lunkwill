#include "interface.h"


char *build_page(page *p)
{
	// Build website and then pass it to html_add_tag
	static char website[4096];
	memset(website, 0, 4096);

	strcpy(website, "<script type=\"text/javascript\">inf = new window.main_interface; inf.setTitle(\"");
	strcpy(website+80, p->title);
	strcat(website+80, "\");</script>");

	return website;
}
