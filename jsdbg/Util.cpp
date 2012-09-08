#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "Util.h"

void* malloczeroes(int size){
	//printf("allocing %d\n", size);

	void* ret = malloc(size);
	memset(ret, 0x0, size);
	return ret;
}