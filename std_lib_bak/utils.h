#ifndef __UTILS__
#define __UTILS__

#include <stdlib.h>
#include <stdio.h>

#define error_out(_msg) do{printf("errored: %s\n",_msg); exit(1);}while(0);

static void sanity_check()
{
	puts("linking works");
}

#endif  //__UTILS__
