#include "JsDbg.h"
#include "WinUtil.h"

JsBuffer* JsDbg::BufReadProcessMemory(BYTE* address, SIZE_T length)
{	
	BYTE* buffer = NULL;
	DWORD old = 0;

	try 
	{
		UnprotectMemory uc(p_hnd, address, length, PAGE_EXECUTE_READWRITE);
		
		buffer = (BYTE*) malloc(sizeof(BYTE) * length);
		assert(buffer != NULL);
		printf("C++: memory alloc'ed\n");

		{
			SIZE_T current_length = length, count = 0;		
			BYTE* ptr_buffer = buffer;

			while(current_length > 0){
				if(!ReadProcessMemory(p_hnd, address, ptr_buffer, current_length, &count)){
					if(count == 0){
						throw "Error reading process'es memory!";						
					} // else: all data ok!
				}

				current_length -= count;
				ptr_buffer += count;
				address += count;
			}
		}
				
	} catch (char* s) {
		printf("C++: Caught exception %s!\n", s);

		if(buffer != NULL)
		{
			free(buffer);	
		}
	}

	if(buffer != NULL && length > 0) {
		return new JsBuffer(buffer, length);
	} else {
		return NULL;
	}
}