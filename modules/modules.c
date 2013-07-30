#include "modules.h"

/** \brief Parses get requests */
void *border(void *request){
	char *path,p[BUF_SIZE];
	char *http_answer = malloc(BUF_SIZE);

	if(((char *)request)[0]==' '){
		if (!config_lookup_string(&session.config, "SERVER_ROOT", (const char **)&path))
		{
			strcpy(p,"www");
		}
		else
		{
			strcpy(p, path);
		}

		strcat(p, "/index.html");

		if(send_file(&http_answer, p, MIME_HTML)==-1)
		{
			return NULL;
		}
	}
	else
	{
		int i;
		char *tag_open, *tag_close;
		void *html_ptr;
		
		new_html();
		
		tag_open=malloc(BUF_SIZE);
		tag_close=malloc(BUF_SIZE);

		strcpy(tag_open,"<table>");
		strcpy(tag_close,"</table>");
		html_ptr=html_add_tag(&session.user_iface->header, tag_open, NULL, tag_close);
		
		strcpy(tag_open,"<tr>");
		strcpy(tag_close,"</tr>");
		http_answer[0]=0;
		html_ptr=html_add_tag(&html_ptr, tag_open, NULL, tag_close);
		
		for(i=0;i<63;i++){
			if(modules[i].name!=NULL){
				sprintf(tag_open, \
					"<td class='menu' onclick=\"javascript:get_url('/%d/')\">",\
					modules[i].id);
				strcpy(tag_close,"</td>");
				sprintf(http_answer,"%s",modules[i].name);
				html_add_tag(&html_ptr, tag_open, http_answer, tag_close);
			}
		}		
		
		send_string(http_answer, html_flush(&session.user_iface->header, 0));
	}
	return http_answer;	
}

int init_modules()
{	
	memset(modules,0,sizeof(modules));
	session.module[__COUNTER__]=border;
	dummy_init_module(__COUNTER__);
	projects_init_module(__COUNTER__);
	return 0;
}
