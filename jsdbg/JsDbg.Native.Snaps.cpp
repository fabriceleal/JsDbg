//#include <Windows.h>

#include <list>
#include <vector>

#include "JsDbg.h"
#include "JsThread.h"

#include <TlHelp32.h>

#include "Util.h"
#include <tchar.h>

// List threads, modules, ...

long JsDbg::FuncResolve(char* dll, char* function){
	// FIXME Of course this cant be like this!

	printf("C++: FuncResolve(%s, %s)\n", dll, function);

	long address = 0;

	HANDLE snap = CreateToolhelp32Snapshot((DWORD)TH32CS_SNAPMODULE, (DWORD)(this->p_pid));
	PMODULEENTRY32 entry = (PMODULEENTRY32) malloczeroes(sizeof(MODULEENTRY32));
	entry->dwSize = sizeof(MODULEENTRY32);

	bool has = Module32First(snap, entry);
	if(has){
		assert(sizeof(char) != sizeof(TCHAR));

		int t_size = sizeof(TCHAR) * strlen(dll) + 2;
		TCHAR* t_dll = (TCHAR*)malloczeroes(t_size);

		::MultiByteToWideChar(CP_ACP, 0, dll, -1, t_dll, t_size);
	
		while(has){
			printf("C++: * %ls\n", entry->szModule);
			if(_tcscmp(t_dll, entry->szModule)){
				printf("C++: Found %ls!!!\n", entry->szModule);
				has = false;
			} else {
				has = Module32Next(snap, entry);
			}
		}
	}
	free(entry);
	CloseHandle(snap);

	return address;
}

void JsDbg::ListThreads(JsThread *** ptr_array, int* length){	
	std::list<JsThread*> ls;
			
	//get_functions();

	printf("C++, ListThreads: start listing...\n");
		
	assert(CreateToolhelp32Snapshot != NULL);
	assert(Thread32First != NULL);
	assert(Thread32Next != NULL);

	HANDLE snap = CreateToolhelp32Snapshot((DWORD)TH32CS_SNAPTHREAD, (DWORD)(this->p_pid));

	printf("C++, ListThreads: have snap...\n");

	THREADENTRY32 thread;
					
	// thread init		
	memset(&thread, 0, sizeof(thread));
	// this should be done manually, or the call will fail
	thread.dwSize = sizeof(thread);
	// ---
	printf("C++, ListThreads: getting first ...\n");
	BOOL has = Thread32First(snap, &thread);
	printf("C++, ListThreads: have first ...\n");
	while(has){

		if(thread.th32OwnerProcessID == this->p_pid){
			ls.push_back(new JsThread(thread.th32OwnerProcessID, thread.th32ThreadID));
		}

		memset(&thread, 0, sizeof(thread)); thread.dwSize = sizeof(thread);

		has = Thread32Next(snap, &thread);
	}
	printf("C++, ListThreads: have all...\n");

	CloseHandle(snap);
			
	if(ls.size() > 0){
		printf("C++, ListThreads: convert to array\n");

		*ptr_array = (JsThread**)malloc(sizeof(JsThread*) * ls.size());
		*length = ls.size();

		int i = 0;
		for(std::list<JsThread*>::iterator it = ls.begin(); it != ls.end(); it++, i++){
			(*ptr_array)[i] = *it;
		}

		printf("C++, ListThreads: end convert to array\n");
	}	
}
