
#ifndef __JSDBG_H
#define __JSDBG_H

#include <node.h>
#include <v8.h>
#include <Windows.h>

#include "Hashtable.h"
#include "JsThread.h"

using namespace node;
using namespace v8;

class JsDbg : public /* So handle_ is visible for the world */ ObjectWrap
{
	LPSTARTUPINFOA p_start;
	LPPROCESS_INFORMATION p_procinfo;
	HANDLE p_hnd;
	DWORD p_pid;
	Hashtable<long>* p_breakpoints;

public:
	bool m_active;

	// We need this to raise events from C++...
	Handle<Object> m_jsObj;

	static Persistent<FunctionTemplate> s_ct;
	static void NODE_EXTERN Init(Handle<Object> target);
	static Handle<Value> New(const Arguments& args);

	JsDbg(){ 
		this->p_start = NULL;
		this->p_procinfo = NULL;
		this->p_hnd = NULL;
		this->p_pid = NULL;
		this->m_active = false;
		this->p_breakpoints = new Hashtable<long>(1027);

		//this->Init();
	}

	~JsDbg(){ 
		// TODO Close handles, free ptrs ...
		
	}
	
	static Handle<Value> load(const Arguments& args);
	static Handle<Value> attach(const Arguments& args);
	static Handle<Value> detach(const Arguments& args);
	static Handle<Value> listThreads(const Arguments& args);
	static Handle<Value> run(const Arguments& args);
	static Handle<Value> funcResolve(const Arguments& args);
	static Handle<Value> setBp(const Arguments& args);

	void Init();
	DWORD Load(const char* path_to_exe);
	DWORD Attach(const int64_t pid);
	DWORD Detach();
	void ListThreads(JsThread *** ptr_array, int* length);
	long FuncResolve(char* dll, char* function);

	void StartDebugThread();
	void SetBp(long address);


};

#endif
