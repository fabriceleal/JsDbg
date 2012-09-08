#ifndef __WINUTIL_H
#define __WINUTIL_H

#include <Windows.h>

class UnprotectMemory 
{
	DWORD p_old;
	HANDLE p_hnd;
	void* p_address;
	long p_length;
	bool p_needsUnprotect;

public:

	UnprotectMemory(HANDLE hnd, void* address, long length, DWORD force);
	~UnprotectMemory();

};

#endif