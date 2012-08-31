// No need to include Windows.h 

#include "JsThread.h"

JsThreadContext* JsThread::get_context(){
	HANDLE hnd = OpenThread(THREAD_ALL_ACCESS, false, this->p_threadid);
	CONTEXT* ctx = (CONTEXT*)malloc(sizeof(CONTEXT));

	JsThreadContext* jsThreadCtx = NULL;
	ctx->ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;

	if(GetThreadContext(hnd, ctx)){
		jsThreadCtx = new JsThreadContext(ctx);
	} else {
		printf("C++: Err getting thread context for thread %d, last error: %d\n", this->p_threadid, GetLastError());
	}

	free(ctx);
	CloseHandle(hnd);
		
	return jsThreadCtx;
}


DWORD JsThread::set_context(JsThreadContext* ctx){


	return 0;
}