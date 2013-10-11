#include "tools.h"

int strnmatch(char *a, char *b, int n)
{
	int i=0,j=0;

	while(!i&&j<n)
	{
		i=1;

		int x;
		for(x=0;x<strlen(b)&&i;x++)
		{
			i=!!(a[j]^b[x]);
		}
		j++;
	}
	return i;
}

int join_to_int(char *data, char *encoding, int bits, int n)
{
	int i=0,x;
	for(x=0;x<n;x++)
	{
		int y=0;

		while(1) 
		{
			if(encoding[y]==0)return -1;

			if(data[x]==encoding[y])
			{
				break;
			}
			y++;
		}

		i&=~(((int)(0xFF))<<(bits*x));
		i|=((int)(y))<<(bits*x);
	}
	i&=~(((int)(0))<<(bits*n));
	return i;
}


char *split_to_xstring(int data, char *encoding, int bits, int n)
{
	int x,filter=0;
	char *ret;
	ret=calloc(1,n+1);
	
	for(x=0;x<bits;filter<<=1,filter|=1,x++);

	for(x=0;x<n;x++)
	{
		ret[x]=encoding[(data>>(x*bits))&filter];
	}

	return ret;
}
