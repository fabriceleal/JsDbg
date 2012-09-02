
#include "JsDbg.h"
#include "JsDebugEvents.h"

typedef struct {
	JsDbg* dbg;
} DebugThreadData, * PDebugThreadData;


DWORD WINAPI DebugThread(LPVOID data){
	LPDEBUG_EVENT dbg = (LPDEBUG_EVENT)malloc(sizeof(DEBUG_EVENT));
	PDebugThreadData wrkData = (PDebugThreadData) data;

	printf("C++, DebugThread: Start debug loop ...\n");
	while(wrkData->dbg->m_active){	
		//printf("Inside while\n");

		memset(dbg, 0x0, sizeof(DEBUG_EVENT));

		//printf("C++, DebugThread: Waiting for debug event ...\n");
		if(WaitForDebugEvent(dbg, INFINITE)){

			//printf("Handling debug event\n");
			wrkData->dbg->m_active = false;
			//printf("C++, DebugThread: Debug Event here: %d, %d\n", dbg->dwProcessId, dbg->dwThreadId);
			JsDebugEvent* ev = GetInstance(dbg);
			
			JsDebugEventInfo* info = (JsDebugEventInfo*) malloc(sizeof(JsDebugEventInfo));
			ev->FillEventInfo(info);
			//printf("C++: %s\n", info->name);
			ev->Dispatch(wrkData->dbg);
			free(info);

			if(EXIT_PROCESS_DEBUG_EVENT != dbg->dwDebugEventCode){
				wrkData->dbg->m_active = true;
				ContinueDebugEvent(dbg->dwProcessId, dbg->dwThreadId, DBG_CONTINUE);
			} 
		}
	}
	printf("C++, DebugThread: the other side of debug loop ...\n");
	
	free(dbg);
	HeapFree(GetProcessHeap(), 0, data);
	return 0;
}

void JsDbg::StartDebugThread(){	
	DWORD thread_id = 0;
	HANDLE thread_hnd = NULL;

	PDebugThreadData data = (PDebugThreadData) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(DebugThreadData));
	data->dbg = this;

	/*
	thread_hnd = CreateThread(NULL, 0, DebugThread, data, 0, &thread_id);
	printf("Handle: %d, Id: %d\n", thread_hnd, thread_id);
	if(NULL == thread_hnd){
		printf("Get last error: %d\n", GetLastError());
	}*/

	DebugThread(data);

}

