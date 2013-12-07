#include "html.h"

char *html_flush(void **html, int follow)
{
		char *ret=NULL, *r;
		char *children=NULL, *following=NULL;
		int length, children_len;

		if(html==NULL || (*html)==NULL){
			return NULL;
		}
		
		//Turn subordinated elements into a string  
		children=html_flush(&((*((HTML_TAG**)html))->tag_embedded_tags), 1);
		if(children==NULL){
			children=calloc(1,1);
		}
		
		if(follow)
		{
			//Proceed with the next element at the same depth  
			following=html_flush(&((*((HTML_TAG**)html))->next_tag), 1);
		}
		else
		{
			following=NULL;
		}
		
		if(following==NULL){
			following=calloc(1,1);
		}
		
		
		length= (*((HTML_TAG**)html))->tag_open_len;
		length+=(*((HTML_TAG**)html))->tag_content_string_len;
		length+=(*((HTML_TAG**)html))->tag_close_len;

		length+=(children_len=strlen(children));
		length+=strlen(following);
		
		r=ret=malloc(length+BUF_SIZE);
		strcpy(r,(*((HTML_TAG**)html))->tag_open);
		strcpy(r+=(*((HTML_TAG**)html))->tag_open_len,(*((HTML_TAG**)html))->tag_content_string);
		strcpy(r+=(*((HTML_TAG**)html))->tag_content_string_len,children);
		strcpy(r+=children_len,(*((HTML_TAG**)html))->tag_close);
		strcpy(r+=(*((HTML_TAG**)html))->tag_close_len,following);

		//Free everything
		nfree((*((HTML_TAG**)html))->tag_open);
		nfree((*((HTML_TAG**)html))->tag_close);
		nfree((*((HTML_TAG**)html))->tag_content_string);
		nfree(children);
		nfree(following);
		nfree((*html));

		return ret;
}

void *html_add_tag(void **parent, char *tag_open, char* content_string, char *tag_close ){
	HTML_TAG **tag;
	void *leeko=NULL,*leekc=NULL, *leekcs=NULL;

	if((*((HTML_TAG **)parent))==NULL)
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
		leeko=tag_open=calloc(1,sizeof(char));
	}
	if(tag_close==NULL)
	{
		leekc=tag_close=calloc(1,sizeof(char));
	}
	if(content_string==NULL)
	{
		leekcs=content_string=calloc(1,sizeof(char));
	}

	(*tag)=malloc(sizeof(HTML_TAG));

	(*tag)->tag_open_len=strlen(tag_open);
	(*tag)->tag_open=malloc((*tag)->tag_open_len+8);

	(*tag)->tag_content_string_len=strlen(content_string);
	(*tag)->tag_content_string=malloc((*tag)->tag_content_string_len+8);

	(*tag)->tag_close_len=strlen(tag_close);
	(*tag)->tag_close=malloc((*tag)->tag_close_len+8);

	(*tag)->tag_embedded_tags=NULL;
	(*tag)->next_tag=NULL;	
	
	strcpy((*tag)->tag_open, tag_open);
	strcpy((*tag)->tag_content_string, content_string);
	strcpy((*tag)->tag_close, tag_close);
	
	nfree(leeko);
	nfree(leekc);
	nfree(leekcs);

	return (*tag);
}

struct html_ui *new_html()
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

char *html_escape(char *string_ptr)
{
	return html_nescape(string_ptr, strlen(string_ptr));
}

char *html_nescape(char *string_ptr, int string_len)
{
		int i;
		char *a;

		a=malloc(6*string_len+2);
		
		for(i=0;i<string_len;i++)
		{
				if((int)string_ptr[i]==10)
				{
						strcpy(a+6*i, "  <br>");
				}
				else
				{
						sprintf(a+6*i, "&#%03d;", (int)string_ptr[i]);
				}
		}
		
	return a;
}
