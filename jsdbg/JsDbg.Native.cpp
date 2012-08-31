//#include <node.h>
//#include <v8.h>

//#include <Windows.h>

#include <list>
//#include <algorithm>
#include <vector>

#include "JsThread.h"
#include "JsDbg.h"

// TODO: put this in public file
void* malloczeroes(int size){
	printf("allocing %d\n", size);

	void* ret = malloc(size);
	memset(ret, 0x0, size);
	return ret;
}


DWORD JsDbg::Load (const char* path_to_exe){
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
		this->p_active = true;
		this->StartDebugThread();

		assert(this->p_pid > 0);

		return 0;
	} else {
		return GetLastError();
	}	
}

DWORD JsDbg::Attach(const int64_t pid){
	this->p_hnd = OpenProcess(PROCESS_ALL_ACCESS, false, pid);

	assert(this->p_hnd > 0);

	printf("pid: %d\n", pid);
	if(DebugActiveProcess((DWORD)pid)){
		this->p_pid = pid;
		this->p_active = true;
		this->StartDebugThread();

		return 0;
	}else{
		return GetLastError();
	}
}

DWORD JsDbg::Detach(){	
	if(DebugActiveProcessStop(this->p_pid))
	{
		return 0;
	}

	return GetLastError();
}

void JsDbg::StartDebugThread(){
	/*printf("%x\n", this);
	LPDEBUG_EVENT dbg = (LPDEBUG_EVENT)malloc(sizeof(DEBUG_EVENT));
	printf("Start debug thead ...\n");
	while(this->p_active){	
		//printf("Inside while\n");

		memset(dbg, 0x0, sizeof(DEBUG_EVENT));

		if(WaitForDebugEvent(dbg, INFINITE)){

			//printf("Handling debug event\n");
			this->p_active = false;
			//printf("Debug Event here: %d, %d\n", dbg->dwProcessId, dbg->dwThreadId);			
			this->p_active = true;
			ContinueDebugEvent(dbg->dwProcessId, dbg->dwThreadId, DBG_CONTINUE);
		}
	}
	printf("the other side ...\n");
	free(dbg);*/
}

// List thread

typedef struct tagTHREADENTRY32 {
  DWORD dwSize;
  DWORD cntUsage;
  DWORD th32ThreadID;
  DWORD th32OwnerProcessID;
  LONG  tpBasePri;
  LONG  tpDeltaPri;
  DWORD dwFlags;
} THREADENTRY32, *PTHREADENTRY32;

typedef HANDLE (WINAPI *SIGN_CreateToolhelp32Snapshot)(DWORD, DWORD);
typedef BOOL (WINAPI *SIGN_Thread32First)(HANDLE, PTHREADENTRY32);

// FIXME: Unsafe!
SIGN_CreateToolhelp32Snapshot CreateToolhelp32Snapshot = NULL; 
SIGN_Thread32First Thread32First = NULL;
SIGN_Thread32First Thread32Next = NULL;

typedef enum _tagThreadSnapEnum : DWORD {
	TH32CS_INHERIT  = 0x80000000,
	TH32CS_SNAPHEAPLIST = 0x00000001,
	TH32CS_SNAPMODULE = 0x00000008,
	TH32CS_SNAPMODULE32 = 0x00000010,
	TH32CS_SNAPPROCESS = 0x00000002,
	TH32CS_SNAPTHREAD = 0x00000004,
	TH32CS_SNAPALL = (0x00000001 /*TH32CS_SNAPHEAPLIST*/ | 0x00000008/*TH32CS_SNAPMODULE*/ | 0x00000002 /*TH32CS_SNAPPROCESS*/ | 0x00000004 /*TH32CS_SNAPTHREAD*/ )
} ThreadSnapEnum;

void JsDbg::ListThreads(JsThread *** ptr_array, int* length){
	//printf("1) %x\n", this);
	std::list<JsThread*> ls;
	
	//printf("2) %x\n", this);
	// list threads
	// to list threads one needs a function that is hidden in kernel32 ...
		
	printf("C++: get functions ...\n");
	//{
		// no need to close this one
		HMODULE kernel32 = GetModuleHandle(TEXT("kernel32"));
		FARPROC n; 

		//if(CreateToolhelp32Snapshot == NULL) {
			n = GetProcAddress(kernel32, "CreateToolhelp32Snapshot");
			CreateToolhelp32Snapshot = (SIGN_CreateToolhelp32Snapshot)n;
			if(NULL == CreateToolhelp32Snapshot){
				printf("C++: Err getting CreateToolhelp32Snapshot! %d", GetLastError());
				return;
			}
		//}

		//if(Thread32First == NULL){
			n = GetProcAddress(kernel32, "Thread32First");
			Thread32First = (SIGN_Thread32First)n;
			if(NULL == Thread32First){
				printf("C++: Err getting Thread32First! %d", GetLastError());
				return;
			}
		//}
		
		//if(Thread32Next == NULL){
			n = GetProcAddress(kernel32, "Thread32Next");
			Thread32Next = (SIGN_Thread32First)n;
			if(NULL == Thread32Next){
				printf("C++: Err getting Thread32Next! %d", GetLastError());
				return;
			}
		//}
		
	//}
	//printf("3) %x\n", this);

	//ls.push_back(new JsThread(1, 1));
	//ls.push_back(new JsThread(2, 2));
	//ls.push_back(new JsThread(3, 3));

	printf("C++: start listing...\n");
	
	//printf("4) %x\n", this);
	assert(CreateToolhelp32Snapshot != NULL);

	HANDLE snap = CreateToolhelp32Snapshot((DWORD)TH32CS_SNAPTHREAD, (DWORD)(this->p_pid));

	THREADENTRY32 thread;
					
	// thread init		
	memset(&thread, 0, sizeof(thread));
	// this should be done manually, or the call will fail
	thread.dwSize = sizeof(thread);
	// ---

	BOOL has = Thread32First(snap, &thread);
	while(has){

		if(thread.th32OwnerProcessID == this->p_pid){
			ls.push_back(new JsThread(thread.th32OwnerProcessID, thread.th32ThreadID));
		}

		memset(&thread, 0, sizeof(thread)); thread.dwSize = sizeof(thread);

		has = Thread32Next(snap, &thread);
	}


	CloseHandle(snap);
	
	printf("C++: end listing!\n");
		
	if(ls.size() > 0){
		printf("C++: convert to array\n");

		*ptr_array = (JsThread**)malloc(sizeof(JsThread*) * ls.size());
		*length = ls.size();

		int i = 0;
		for(std::list<JsThread*>::iterator it = ls.begin(); it != ls.end(); it++, i++){
			(*ptr_array)[i] = *it;
		}

		printf("C++: end convert to array\n");
	}
	
}