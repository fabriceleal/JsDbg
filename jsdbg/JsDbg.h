
#ifndef __JSDBG_H
#define __JSDBG_H

#include <node.h>
#include <v8.h>
#include <Windows.h>

#include "JsThread.h"

using namespace node;
using namespace v8;

// TODO This guy should raise events
class JsDbg : ObjectWrap
{
	LPSTARTUPINFOA p_start;
	LPPROCESS_INFORMATION p_procinfo;
	HANDLE p_hnd;
	DWORD p_pid;
	bool p_active;

public:

	static Persistent<FunctionTemplate> s_ct;
	static void NODE_EXTERN Init(Handle<Object> target);
	static Handle<Value> New(const Arguments& args);

	JsDbg(){ 
		this->p_start = NULL;
		this->p_procinfo = NULL;
		this->p_hnd = NULL;
		this->p_pid = NULL;
		this->p_active = false;
	}

	~JsDbg(){ 
		// TODO Close handles, free ptrs ...

	}
	
	static Handle<Value> load(const Arguments& args);
	static Handle<Value> attach(const Arguments& args);
	static Handle<Value> detach(const Arguments& args);
	static Handle<Value> listThreads(const Arguments& args);

	DWORD Load(const char* path_to_exe);
	DWORD Attach(const int64_t pid);
	DWORD Detach();
	void ListThreads(JsThread *** ptr_array, int* length);


	void StartDebugThread();

};

#endif
