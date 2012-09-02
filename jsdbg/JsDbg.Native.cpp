//#include <node.h>
//#include <v8.h>

//#include <Windows.h>

#include "JsDbg.h"


// TODO: put this in public file
void* malloczeroes(int size){
	printf("allocing %d\n", size);

	void* ret = malloc(size);
	memset(ret, 0x0, size);
	return ret;
}


DWORD JsDbg::Load (const char* path_to_exe){
	this->Init();

	printf("%x\n", this);
	this->p_start = (LPSTARTUPINFOA)malloczeroes(sizeof(STARTUPINFOA));
	assert(this->p_start != NULL);

	this->p_procinfo = (LPPROCESS_INFORMATION)malloczeroes(sizeof(PROCESS_INFORMATION));
	assert(this->p_procinfo != NULL);

	this->p_start->dwFlags = 0x1;
	this->p_start->wShowWindow = 0x1;
	this->p_start->cb = sizeof(STARTUPINFOA);
		
	/*char* cmd = NULL;
	{
		int len = strlen(path_to_exe) + 1;
		cmd = (char*)malloc(len);
		strcpy(cmd, path_to_exe);
	}*/
	
	if(CreateProcessA(path_to_exe, NULL, NULL, NULL, false, DEBUG_PROCESS | DEBUG_ONLY_THIS_PROCESS, NULL, NULL, this->p_start, this->p_procinfo)){
		this->p_pid = this->p_procinfo->dwProcessId;
		this->m_active = true;
		
		assert(this->p_pid > 0);

		//printf("C++: All Access: %x\n", PROCESS_ALL_ACCESS);
		this->p_hnd = OpenProcess(
			//PROCESS_ALL_ACCESS | 0xf000, 
			PROCESS_ALL_ACCESS | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
			false, this->p_pid);

		assert(this->p_hnd > 0);

		return 0;
	} else {
		return GetLastError();
	}	
}

DWORD JsDbg::Attach(const int64_t pid){
	this->p_hnd = OpenProcess(PROCESS_ALL_ACCESS | 0xf000, false, pid);

	assert(this->p_hnd > 0);

	printf("pid: %d\n", pid);
	if(DebugActiveProcess((DWORD)pid)){
		this->p_pid = pid;
		this->m_active = true;
		
		return 0;
	}else{
		return GetLastError();
	}
}

DWORD JsDbg::Detach(){
	printf("C++: Detaching...\n");
	if(DebugActiveProcessStop(this->p_pid))
	{
		printf("C++: Detached...\n");
		return 0;
	}

	printf("C++: ERR detaching...\n");
	return GetLastError();
}


long JsDbg::FuncResolve(char* dll, char* function){
	// FIXME Of course this cant be like this!

	printf("C++: FuncResolve(%s, %s)\n", dll, function);

	HMODULE hnd = GetModuleHandleA(dll);
	if(NULL == hnd){
		printf("C++: Err on GetModuleA, %d\n", GetLastError());
		return NULL;
	}

	long address = (long)GetProcAddress(hnd, function);
	printf("Sizeof Farproc: %d, sizeof long: %d\n", sizeof(FARPROC), sizeof(long));
	if(NULL == address){
		printf("C++: Err on FuncResolve, %d\n", GetLastError());
		return NULL;
	}

	printf("C++: Result: %d\n", address);
	
	return address;
}

void JsDbg::SetBp(long address){
	printf("C++: SetBp (%d)\n", address);

	char* buff = (char*)malloczeroes(sizeof(char) * 1);
	SIZE_T bytes = -1;

	printf("C++: Thread ID: %d\n", GetCurrentThreadId());

	DWORD old = -1;
	MEMORY_BASIC_INFORMATION info;
	long res;
	if(0 == (res = VirtualQueryEx(this->p_hnd, (LPVOID)address, &info, sizeof(MEMORY_BASIC_INFORMATION)))){
		printf("C++: (-1) ERR:%d\n ", GetLastError());
	}
	printf("C++: Res = %d, allocprotect: %x, state: %x, protect: %x, type: %x\n", 
			res, info.AllocationProtect, info.State, info.Protect, info.Type);

	if(MEM_FREE == info.State){
		printf("C++: ERR!!!! This memory is free!!!!\n");
		return;
	}

	printf("C++: PAGE_READWRITE: %x\n", (info.AllocationProtect & PAGE_READWRITE));
	printf("C++: PAGE_EXECUTE_READWRITE: %x\n", (info.AllocationProtect & PAGE_EXECUTE_READWRITE));

	if(!VirtualProtectEx(this->p_hnd, (LPVOID)address, 1, PAGE_READWRITE, &old)){
		printf("C++: (0) ERR: %d\n", GetLastError());
	}

	if(!ReadProcessMemory(this->p_hnd, (LPCVOID)address, buff, 1, &bytes)){
		printf("C++: (1) ERR: %d, read: %d\n", GetLastError(), bytes);
	}

	printf("C++: Original: %d\n", buff[0]);

	long l = (long)buff[0];
	this->p_breakpoints->Insert(address, l);
	
	buff[0] = 0xCC;
	printf("C++: Size of LPVOID = %d, size of long = %d\n", sizeof(LPVOID), sizeof(long));

	bytes = -1;
	if(!WriteProcessMemory(this->p_hnd, (LPVOID) address, (LPVOID)buff, 1, &bytes)){
		printf("C++: (2) ERR: %d, wrote: %d\n", GetLastError(), bytes);
	}
}