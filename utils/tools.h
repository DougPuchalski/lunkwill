#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "lwutils.h"

extern int strnmatch(char *a, char *b, int n);

extern int join_to_int(char *data, char *encoding, int bits, int n);

extern char *split_to_xstring(int data, char *encoding, int bits, int n);

#endif
