#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "lwutils.h"

struct page
{
	char title[64];
	char sidebar_title[64];
	char sidebar[128];
	char site[2048];
};

typedef struct page page;

/* \brief Generate HTML Code out of the page struct */
char *build_page(page *p);

#endif
