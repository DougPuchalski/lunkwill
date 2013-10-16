#include "worker.h"
#include "server.h"
#include "request_parser.h"

void *workerthread()
{
	DBGPRINTF("New pthread startet%s\n","");

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
				DBGPRINTF("Send %s\n", "html");
				parsed_request.answer=new_html();
				void *x;

				if(Modules[0].func(Modules[0].data,&parsed_request)==0)
				{
					buffer->size=send_string(&buffer->data,\
						x=html_flush(&((struct html_ui*)parsed_request.answer)->base,1));
				}
				else
				{
					buffer->size=send_raw(&buffer->data,\
						x=html_flush(&((struct html_ui*)parsed_request.answer)->base,1));
				}

				nfree(parsed_request.answer);
				nfree(x);

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
				buffer->size=send_raw(&buffer->data,HTTP_500);
				goto PIPE;
				break;
		}

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
