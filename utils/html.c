#include "html.h"

char *html_flush(void **html, int follow)
{
	char *ret=NULL, *r;
	char *children=NULL, *following=NULL;
	int length, children_len, following_len;

	if(html==NULL || (*html)==NULL)
	{
		return NULL;
	}

	//Turn subordinated elements into a string
	children=html_flush(&((*((HTML_TAG**)html))->tag_embedded_tags), 1);
	if(children==NULL)
	{
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

	if(following==NULL)
	{
		following=calloc(1,1);
	}


	length= (*((HTML_TAG**)html))->tag_open_len;
	length+=(*((HTML_TAG**)html))->tag_content_string_len;
	length+=(*((HTML_TAG**)html))->tag_close_len;

	length+=(children_len=strlen(children));
	length+=(following_len=strlen(following));

	r=ret=malloc(length+BUF_SIZE);

	memcpy(r,(*((HTML_TAG**)html))->tag_open,(*((HTML_TAG**)html))->tag_open_len);
	r+=(*((HTML_TAG**)html))->tag_open_len;

	memcpy(r,(*((HTML_TAG**)html))->tag_content_string, (*((HTML_TAG**)html))->tag_content_string_len);
	r+=(*((HTML_TAG**)html))->tag_content_string_len;

	memcpy(r,children,children_len);
	r+=children_len;

	memcpy(r,(*((HTML_TAG**)html))->tag_close, (*((HTML_TAG**)html))->tag_close_len);
	r+=(*((HTML_TAG**)html))->tag_close_len;

	memcpy(r,following,following_len);

	*(r+following_len)=0;

	//Free everything
	nfree((*((HTML_TAG**)html))->tag_open);
	nfree((*((HTML_TAG**)html))->tag_close);
	nfree((*((HTML_TAG**)html))->tag_content_string);
	nfree(children);
	nfree(following);
	nfree((*html));

	return ret;
}

void *html_a_tag(void **parent, char *tag_open, int tag_open_len, char* content_string, int content_string_len, char *tag_close, int tag_close_len)
{
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
		leeko=tag_open=malloc(sizeof(char));
		tag_open[0]=0;
	}
	if(tag_close==NULL)
	{
		leekc=tag_close=malloc(sizeof(char));
		tag_close[0]=0;
	}
	if(content_string==NULL)
	{
		leekcs=content_string=malloc(sizeof(char));
		content_string[0]=0;
	}

	(*tag)=malloc(sizeof(HTML_TAG));

	for(; tag_open_len!=BUF_SIZE && tag_open_len>=sizeof(void*) && tag_open[tag_open_len-1]==0 && tag_open[tag_open_len-2]!=0; tag_open_len--);
	for(; content_string_len!=BUF_SIZE && content_string_len>=sizeof(void*) && content_string[content_string_len-1]==0 && content_string[content_string_len-2]!=0; content_string_len--);
	for(; tag_close_len!=BUF_SIZE && tag_close_len>=sizeof(void*) && tag_close[tag_close_len-1]==0 && tag_close[tag_close_len-2]!=0; tag_close_len--);

	(*tag)->tag_open_len=(tag_open_len>sizeof(void*)&&tag_open_len!=BUF_SIZE)?
	                     tag_open_len:strlen(tag_open);

	(*tag)->tag_open=malloc((*tag)->tag_open_len+8);

	(*tag)->tag_content_string_len=(content_string_len>sizeof(void*)&&content_string_len!=BUF_SIZE)?
	                               content_string_len:strlen(content_string);

	(*tag)->tag_content_string=malloc((*tag)->tag_content_string_len+8);

	(*tag)->tag_close_len=(tag_close_len>sizeof(void*)&&tag_close_len!=BUF_SIZE)?
	                      tag_close_len:strlen(tag_close);

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
	                      html_add_tag(&user_iface->base, SIDEBAR_OPEN, NULL, SIDEBAR_CLOSE);

	user_iface->main = \
	                   html_add_tag(&user_iface->base, MAIN_OPEN, NULL, MAIN_CLOSE);

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

	for(i=0; i<string_len; i++)
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
