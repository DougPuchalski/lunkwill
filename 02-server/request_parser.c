#include "request_parser.h"

request parse_request(char *get_request)
{
	request req;
	memset(&req, 0, sizeof(request));
	req.module_request[0]=0;

	// Invalid request
	if(strbegin(get_request, "GET /") != 0)
	{
		goto HTTP451;
	}

	get_request+=5;

	// Send index.html
	if(strbegin(get_request," ") == 0)
	{
		req.special_file = INDEX_HTML;
		return req;
	}

	//Shared link
	if(get_request[0]=='?')
	{
		req.special_file = LINK_RESOLVER;

		get_request++;
		while((get_request[0]=='/'||get_request[0]=='\\')&&get_request[0]!=' ')
		{
			get_request++;
		}

		char *gr=get_request;

		while(gr[0]!=' ')
		{
			if(gr[0]=='.'&&gr[1]=='.')
			{
				goto HTTP451;
			}
			gr++;
		}

		goto module_request;
	}

	if(strbegin(get_request, "logo.png ") == 0)
	{
		req.special_file = LOGO_PNG;
		return req;
	}

	if(strbegin(get_request, "favicon.ico ") == 0)
	{
		req.special_file = FAVICON_ICO;
		return req;
	}

	// Set special_file to 0 by default
	req.special_file = NON_SPECIAL;

	if(strnmatch((get_request), URL_CHARS, 20)!=0)
	{
		goto HTTP451;
	}

	// Read session id
	strncpy(req.session_id, get_request, 20);

	req.user=join_to_int(get_request, URL_CHARS, 6, 5);
	get_request+=5;
	req.group=join_to_int(get_request, URL_CHARS, 6, 5);
	get_request+=5;
	req.session1=join_to_int(get_request, URL_CHARS, 6, 5);
	get_request+=5;
	req.session2=join_to_int(get_request, URL_CHARS, 6, 5);
	get_request+=6;

	// Check project id
	if(strnmatch((get_request), URL_CHARS, 4)!=0)
	{
		req.project=0;
		req.module=0;
		return req;
	}

	// Read project id
	strncpy(req.project_id, get_request, 4);
	req.project=join_to_int(get_request, URL_CHARS, 6, 4);
	get_request+=5;


	// Check module id
	if(strnmatch((get_request), URL_CHARS, 2)!=0)
	{
		req.module=0;
		return req;
	}

	strncpy(req.module_id, get_request, 2);
	req.module=join_to_int(get_request, URL_CHARS, 6, 2);
	get_request+=2;
	if(get_request[0]!=' ')get_request++;

	// Find end of module_request
module_request:
	;
	char *ptr = strstr(get_request, " ");
	if(ptr == NULL)	goto HTTP451;

	if(ptr - (get_request) < BUF_SIZE-1)
	{
		strncpy(req.module_request, get_request, ptr - (get_request));
	}
	else
	{
		goto HTTP451;
	}

	return req;

	// Returns NULL
HTTP451:
	req.special_file = ERROR_451;
	return req;
}

