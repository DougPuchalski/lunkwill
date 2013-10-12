#include "worker.h"
#include "server.h"
#include "request_parser.h"

int send_login(char **buffer)
{
	struct html_ui *h;
	void *x;
	int s=0;

	h=new_html();
	void *form=html_add_tag(&h->main, \
	"<form action='javascript:get_url("\
	"(document.getElementById(\"email\").value+document.getElementById(\"password\").value)"\
	"),get_url(\"\")'>","","</form>");
	html_add_tag(&form, "<strong>E-Mail:</strong><br><input id=email type=email>","","</input><p>");
	html_add_tag(&form, "<strong>Password:</strong><br><input id=password type=password>","","</input><p>");
	html_add_tag(&form, "<input type=submit value=Login>",NULL,"</input>");
	s=send_string(buffer,x=html_flush(&h->base,1));
	nfree(h);
	nfree(x);
	return s;
}

void *workerthread()
{
	DBGPRINTF("New pthread startet%s\n","");
	char buf[BUF_SIZE];

	while(1)
	{
		struct pipe_rxtx *buffer=NULL;

		pthread_mutex_lock( &Lock_Count );
			buffer=fifo_pop(&Jobs);
		pthread_mutex_unlock( &Lock_Count );

		if(buffer==NULL)
		{
			DBGPRINTF("No more work to do%s\n","");
			goto IQUITTODAY;
		}
		request parsed_request=parse_request(buffer->data);
		nfree(buffer->data);

		switch(parsed_request.special_file)
		{
			case NON_SPECIAL:
				switch(Modules[0].func(Modules[0].data,&parsed_request))
				{
					case 0: //uid ok
					{
						parsed_request.answer=new_html();
						void *x,*html=((struct html_ui*)parsed_request.answer)->header;
						int i;
						
						for(i=0;i<256;i++)
						{
							if(Modules[i].name!=NULL)
							{
								html_add_tag(&html, \
									"<a href='javascript:"\
									"createCookie(\"module\",\"", \
									x=split_to_xstring(i,URL_CHARS,6,2) \
									,"\",\"7\"),get_url(\"\")' " \
									"style='background:#aa2211;color:#FFF;margin-left:5px;'"\
									"><div style='margin:1px 10px;display: inline-block'>");
									
								nfree(x);

								html_add_tag(&html, \
									Modules[i].name, \
									NULL, "</div></a>" );
							}
						}

						html_add_tag(&((struct html_ui*)parsed_request.answer)->header, \
							"<a href='javascript:"\
							"eraseCookie(\"login\"),get_url(\"\")' " \
							"style='background:#aa2211;color:#FFF;"\
							"position:absolute;right:0px;'" \
							">","<div style='margin:1px 10px;'>Logout</div>","</a>");
						
						if(Modules[parsed_request.module].func!=NULL)
						{
							Modules[parsed_request.module].func( \
								Modules[parsed_request.module].data, \
								&parsed_request);
						}

						buffer->size=send_string(&buffer->data,\
							x=html_flush(&((struct html_ui*)parsed_request.answer)->base,1));
						nfree(parsed_request.answer);
						nfree(x);
					}
					break;
					case 1: //ask for login
						buffer->size=send_login(&buffer->data);
						break;
					case 2: //server error
						goto ERROR_500;
					case 3: //new session
						sprintf(buf, "<script>"\
							"createCookie('login','%s',7);"\
							"createCookie('module','BA',7);"\
							"</script>",\
							parsed_request.module_request);
						buffer->size=send_string(&buffer->data, buf);
					break;
					default:
						goto ERROR_451;
					break;
				}
				goto PIPE;
				break;
			case INDEX_HTML:
				DBGPRINTF("Send %s\n", "index.html");
				buffer->size=send_file(&buffer->data, "www/index.html");
				goto PIPE;
				break;
			case LOGO_PNG:
				DBGPRINTF("Send %s\n", "logo.png");
				buffer->size=send_file(&buffer->data, "www/logo.png");
				goto PIPE;
				break;
			case FAVICON_ICO:
				DBGPRINTF("Send %s\n", "favicon.ico");
				buffer->size=send_file(&buffer->data, "www/favicon.ico");
				goto PIPE;
				break;
			default:
				DBGPRINTF("Send unknown:%d\n", parsed_request.special_file);
				break;
		}

		ERROR_451:
			DBGPRINTF("Send %s\n", "HTTP 451");
			buffer->size=strlen(HTTP_451);
			buffer->data=malloc(buffer->size);
			strncpy(buffer->data, HTTP_451, buffer->size);
			goto PIPE;
		
		ERROR_500:
			DBGPRINTF("Send %s\n", "HTTP 500");
			buffer->size=strlen(HTTP_500);
			buffer->data=malloc(buffer->size);
			strncpy(buffer->data, HTTP_500, buffer->size);
			goto PIPE;

		PIPE:		
			pthread_mutex_lock( &Lock_Send );
				DBGPRINTF("Send %d bytes through pipe %d\n", buffer->size, Send_FD);
				if(write(Send_FD, buffer, sizeof(struct pipe_rxtx))==-1)
				{
					nfree(buffer->data);
					nfree(buffer);
					DBGPRINTF("Pipe %d is broken\n", Send_FD);
					pthread_mutex_unlock( &Lock_Send );
					goto IQUITTODAY;
				}
				if(write(Send_FD, buffer->data, buffer->size)==-1)
				{
					nfree(buffer->data);
					nfree(buffer);
					DBGPRINTF("Pipe %d is broken\n", Send_FD);
					pthread_mutex_unlock( &Lock_Send );
					goto IQUITTODAY;
				}
			pthread_mutex_unlock( &Lock_Send );
		
		nfree(buffer->data);
		nfree(buffer);

	}

	IQUITTODAY:
	pthread_mutex_lock( &Lock_Count );
		Thread_Count--;
	pthread_mutex_unlock( &Lock_Count );
	pthread_exit(NULL);
}
