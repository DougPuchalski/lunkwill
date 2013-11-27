#include "base64.h"

char* b64_encode( char *data, int data_len, enum B64_CHARSET cs)
{
	char *b64_string, *c_set;
	int i, byte, buf;

	b64_string=calloc(1, (4*(data_len+2))/3);
	
	switch(cs)
	{
		case B64_URL:
			c_set=B64_URL_CHARS;
		break;

		case B64_DEFAULT: //Use default if unknown
		default:
			c_set=B64_DEFAULT_CHARS;
		break;
	}
	
	for(i=0, byte=0;i<data_len;i+=3)
	{
		buf=data[i]<<16;

		if(i+1<data_len)
		{
			buf|=data[i+1]<<8;
		}
		else
		{
			b64_string[byte+2]='=';			
		}

		if(i+2<data_len)
		{
			buf|=data[i+2];
		}
		else
		{
			b64_string[byte+3]='=';
		}
		
		if(!b64_string[byte])
			b64_string[byte++]=c_set[(uint8_t)(buf >> 18) & 63];
		if(!b64_string[byte])
			b64_string[byte++]=c_set[(uint8_t)(buf >> 12) & 63];
		if(!b64_string[byte])
			b64_string[byte++]=c_set[(uint8_t)(buf >> 6) & 63];
		if(!b64_string[byte])
			b64_string[byte++]=c_set[(uint8_t)(buf >> 0) & 63];
	}
	
	return b64_string;
}


char* b64_decode( char *b64_string, enum B64_CHARSET cs )
{
	static char *lookup_url=NULL;
	static char *lookup_def=NULL;
	char *data, *lookup;
	int i,len, buf, byte;

	if(lookup_url==NULL) //First function call
	{
		lookup_url=calloc(1,256);
		lookup_def=calloc(1,256);
		
		for(i=0;i<64;i++)
		{
			data=B64_URL_CHARS;
			lookup_url[(int)data[i]]=(char)i;
			data=B64_DEFAULT_CHARS;
			lookup_def[(int)data[i]]=(char)i;
		}	
	}
	
	len= strlen(b64_string);
	data=calloc(1, len);

	switch(cs)
	{
		case B64_URL:
			lookup=lookup_url;
		break;

		case B64_DEFAULT: //Use default if unknown
		default:
			lookup=lookup_def;
		break;
	}
	

	for(i=0, byte=0;i<len;i+=4)
	{
		buf = (lookup[(uint8_t)(b64_string[i+0])])<< 18;
		buf|= (lookup[(uint8_t)(b64_string[i+1])])<< 12;
		buf|= (lookup[(uint8_t)(b64_string[i+2])])<<  6;
		buf|= (lookup[(uint8_t)(b64_string[i+3])])<<  0;		

		data[byte++]=(buf>>16)&0xff;
 		data[byte++]=(buf>> 8)&0xff;
		data[byte++]=(buf>> 0)&0xff;
	}

	return data;
}
