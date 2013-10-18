#include "log.h"

int log_write(char *message, int error_level){
	char *Error_level[] = {"[ DEBUG ]", "[ WARN  ]", "[  ERR  ]", "[ EMERG ]"};
	FILE *logfile;
	
	if((logfile = fopen("lunkwill.log", "a")) == NULL){
		fprintf(stderr, "%s\tCould not open logfile. Error reporting to stderr only\n", Error_level[2]);
		return 1;
	}

	struct tm *ti;
	time_t time_s;
	char t_buf[128];

	time(&time_s);
	ti = localtime(&time_s);

	strftime(t_buf, 128, "%d. %b %Y %H:%M:%S", ti);

	fprintf(stderr, "%s\t%s\t%s\n", Error_level[error_level], t_buf, message);
	fprintf(logfile, "%s\t%s\t%s\n", Error_level[error_level], t_buf, message);
	
	fclose(logfile); /** \todo Try to manage log writes in main thread to not always have open and close file */
	
	return 0;
}

