#include "globals.h"

#include <stdlib.h>


Config* init_config()
{
	struct Config *c = malloc(sizeof(struct Config));
	return c;	
}


