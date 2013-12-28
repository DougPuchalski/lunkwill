#ifndef __BASE64_H__
#define __BASE64_H__

#include "lwutils.h"

#define B64_DEFAULT_CHARS \
"ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
"abcdefghijklmnopqrstuvwxyz" \
"0123456789+/=";

#define B64_URL_CHARS \
"ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
"abcdefghijklmnopqrstuvwxyz" \
"0123456789-_=";

enum B64_CHARSET
{
    B64_DEFAULT,
    B64_URL
};

char* b64_encode(char *data, int data_len, enum B64_CHARSET cs);
char* b64_decode(char *b64_string, enum B64_CHARSET cs);

#endif

