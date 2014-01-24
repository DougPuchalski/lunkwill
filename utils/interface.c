#include "interface.h"


char *ui_show(page_t *p)
{
	// Build website and then pass it to html_add_tag
	static char website[4096];
	int i, n;
	
	char *head=
		"<script type='text/javascript'>"
		"ui = new window.UserInterface;"
		"ui.raw={";
//	char *body;
	char *tail=
		"};ui.build()"
		"</script>";
		
	for(i=0,n=0;n<4095;i++,n++)
	{
		if(head[i]==0) break;
		website[n]=head[i];
	}

//	for(i=0;n<4095;i++,n++)
//	{
//		if(body[i]==0) break;
//		website[n]=body[i];		
//	}

	for(i=0;n<4095;i++,n++)
	{
		if(tail[i]==0) break;
		website[n]=tail[i];		
	}

	website[n]=0;

	return website;
}
