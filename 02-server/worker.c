#include "worker.h"
#include "server.h"
#include "request_parser.h"

void *workerthread(struct serverwork *sw)
{

	while(1)
	{
		struct pipe_rxtx *buffer=NULL;

		pthread_mutex_lock(&sw->lock_count);
		buffer=fifo_pop(&sw->jobs);
		pthread_mutex_unlock(&sw->lock_count);

		if(buffer==NULL)
		{
			goto IQUITTODAY;
		}
		request parsed_request=parse_request(buffer->data);
		nfree(buffer->data);

		switch(parsed_request.special_file)
		{
		case NON_SPECIAL:
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
			buffer->size=send_file(&buffer->data, "www/index.html");
			goto PIPE;
			break;
		case LOGO_PNG:
			buffer->size=send_file(&buffer->data, "www/logo.png");
			goto PIPE;
			break;
		case FAVICON_ICO:
			buffer->size=send_file(&buffer->data, "www/favicon.ico");
			goto PIPE;
			break;
		case LINK_RESOLVER:
			buffer->size=send_file(&buffer->data, parsed_request.module_request);
			goto PIPE;
			break;
		case ERROR_451:
			buffer->size=send_raw(&buffer->data,HTTP_451);
			goto PIPE;
			break;
		default:
			buffer->size=send_raw(&buffer->data,HTTP_500);
			goto PIPE;
			break;
		}

PIPE:
		if(buffer->size>0)
		{
			pthread_mutex_lock(&sw->lock_send);

			if(write(sw->send_fd, buffer, sizeof(struct pipe_rxtx))==-1)
			{
				nfree(buffer->data);
				nfree(buffer);
				pthread_mutex_unlock(&sw->lock_send);
				goto IQUITTODAY;
			}
			if(write(sw->send_fd, buffer->data, buffer->size)==-1)
			{
				nfree(buffer->data);
				nfree(buffer);
				pthread_mutex_unlock(&sw->lock_send);
				goto IQUITTODAY;
			}
			pthread_mutex_unlock(&sw->lock_send);
		}
		nfree(buffer->data);
		nfree(buffer);

	}

IQUITTODAY:
	pthread_mutex_lock(&sw->lock_count);
	sw->thread_count--;
	pthread_mutex_unlock(&sw->lock_count);
	pthread_exit(NULL);
}
