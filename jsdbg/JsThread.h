#ifndef __JSTHREAD_H
#define __JSTHREAD_H

#include <node.h>
#include <v8.h>
#include <Windows.h>

#include "JsThreadContext.h"

using namespace v8;
using namespace node;

class JsThread 
{
	DWORD p_threadid;
	DWORD p_procid;

	static Persistent<FunctionTemplate> s_ct;

public:
		
	JsThread(DWORD procid, DWORD threadid) : p_threadid(threadid), p_procid(procid) { }
	
	DWORD thread_id(){
		return this->p_threadid;
	}

	DWORD process_id(){
		return this->p_procid;
	}

	JsThreadContext* get_context();
	DWORD set_context(JsThreadContext* ctx);

	static Local<Object> GetV8Obj(JsThread* t);

	static Handle<Value> GetThreadId(Local<String> name, const AccessorInfo& info);
	static Handle<Value> GetProcessId(Local<String> name, const AccessorInfo& info);
	
	static Handle<Value> GetContext(const Arguments& args);

};

#endif