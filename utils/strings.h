#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <string.h>

#include "lwutils.h"

extern int strnmatch(char *string, char *pattern, int len);

extern int join_to_int(char *data, char *encoding, int bits, int n);

extern char *split_to_xstring(int data, char *encoding, int bits, int n);

#endif
