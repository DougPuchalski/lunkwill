#include "html.h"
#include "lunkwill.h"

char *html_flush(void **html, int follow)
{
		char *ret=NULL;
		char *children, *following;
		int length;


		if(html==NULL || (*html)==NULL){
			return NULL;
		}
		
		children=html_flush(&((*((HTML_TAG**)html))->tag_embedded_tags), 1);
		if(children==NULL){
			children=calloc(1,1);
		}
		
		if(follow)
		{
			following=html_flush(&((*((HTML_TAG**)html))->next_tag), 1);
		}
		else
		{
			following=NULL;
		}
		
		if(following==NULL){
			following=calloc(1,1);
		}
				
		length=strlen((*((HTML_TAG**)html))->tag_open);		
		length+=strlen((*((HTML_TAG**)html))->tag_content_string);
		length+=strlen(children);
		length+=strlen((*((HTML_TAG**)html))->tag_close);
		length+=strlen(following);
		
		ret=malloc(length+BUF_SIZE);
		strcpy(ret,(*((HTML_TAG**)html))->tag_open);
		strcat(ret,(*((HTML_TAG**)html))->tag_content_string);
		strcat(ret,children);
		strcat(ret,(*((HTML_TAG**)html))->tag_close);
		strcat(ret,following);

		nfree((*html));
		return ret;
}

void *html_add_tag(void **parent, char *tag_open, char* content_string, char *tag_close ){
	HTML_TAG **tag;

	if((*((HTML_TAG **)parent)==NULL))
	{
		tag=(HTML_TAG **)parent;
	}
	else
	{
		tag=((HTML_TAG **)&(*((HTML_TAG **)parent))->tag_embedded_tags);
	}
	

	while((tag!=NULL)&&((*tag)!=NULL))
	{
		tag=((HTML_TAG **)&(*((HTML_TAG **)tag))->next_tag);
	}
	
	if(tag_open==NULL)
	{
		tag_open=calloc(1,sizeof(char *));
	}
	if(tag_close==NULL)
	{
		tag_close=calloc(1,sizeof(char *));
	}
	if(content_string==NULL)
	{
		content_string=calloc(1,sizeof(char *));
	}

	(*tag)=malloc(sizeof(HTML_TAG));
	(*tag)->tag_open=malloc(strlen(tag_open)+8);
	(*tag)->tag_content_string=malloc(strlen(content_string)+8);
	(*tag)->tag_close=malloc(strlen(tag_close)+8);
	(*tag)->tag_embedded_tags=NULL;	
	(*tag)->next_tag=NULL;	
	
	strcpy((*tag)->tag_open, tag_open);
	strcpy((*tag)->tag_content_string, content_string);
	strcpy((*tag)->tag_close, tag_close);

	return (*tag);
}

struct html_ui * new_html()
{
	struct html_ui *user_iface;
	
	user_iface=calloc(1, sizeof(struct html_ui));

	user_iface->base=html_add_tag(&user_iface->base, NULL, NULL, NULL);

	user_iface->header = \
		html_add_tag(&user_iface->base, HEADER_OPEN, NULL, HEADER_CLOSE);

	user_iface->sidebar = \
		html_add_tag(&user_iface->base, SIDEBAR_OPEN, NULL, SIDEBAR_CLOSE );

	user_iface->main = \
		html_add_tag(&user_iface->base, MAIN_OPEN, NULL, MAIN_CLOSE );
		
	return user_iface;
}
