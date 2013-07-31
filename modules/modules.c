#include "modules.h"

/** \brief Parses get requests */
int border(void *request, struct html_ui* user_iface){
	char buf[BUF_SIZE];
	void *html_ptr;
	int i;
	
	new_html();

	html_ptr=html_add_tag( \
		&user_iface->header, \
		"<table>", NULL, "</table>");
	
	html_ptr=html_add_tag(&html_ptr, "<tr>", NULL, "</tr>");
	
	for(i=0;i<63;i++){
		if(modules[i].name!=NULL){
			sprintf(buf, \
				"<td class='menu' onclick=\"javascript:get_url('/%d/')\">",\
				modules[i].id-1);
			html_add_tag(&html_ptr, buf, modules[i].name, "</td>");
		}
	}		
	return 0;
}

int init_modules()
{	
	memset(modules,0,sizeof(modules));
	session.module[__COUNTER__]=border;
	dummy_init_module(__COUNTER__);
	projects_init_module(__COUNTER__);
	git_init_module(__COUNTER__);
	
	return 0;
}
