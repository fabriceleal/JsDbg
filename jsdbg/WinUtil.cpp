#include <stdio.h>

#include "WinUtil.h"


UnprotectMemory::UnprotectMemory(HANDLE hnd, void* address, long length, DWORD force) : 
		p_hnd(hnd), p_address (address), 
		p_length (length), p_needsUnprotect (false)
{
	printf("C++: About to unprotect ...\n");
	if(!VirtualProtectEx(p_hnd, p_address, p_length, force, &p_old)) 
	{
		printf("C++: Error unprotecting ...\n");
		throw "Unable to call VirtualProtectEx for unprotect section.";
	}
	printf("C++: Protected!\n");
	p_needsUnprotect = true;
}

UnprotectMemory::~UnprotectMemory()
{
	if(p_needsUnprotect){
		DWORD dummy;
		if(!VirtualProtectEx(p_hnd, p_address, p_length, p_old, &dummy)) {
			throw "Unable to call VirtualProtectEx for restoring section's protection.";
		}
	}
}
