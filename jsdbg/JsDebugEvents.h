#ifndef __JSDEBUGEVENTS_H
#define __JSDEBUGEVENTS_H

#include <node.h>
#include <v8.h>

#include <Windows.h>

#include "JsSystemDll.h"
#include "JsDbg.h"

using namespace v8;
using namespace node;


struct JsDebugEventInfo {
	char* name;
};

class JsDebugEvent {

protected:

	JsDebugEvent(){ }

public:

	virtual void FillEventInfo(JsDebugEventInfo* evInfo) = 0;
	virtual Local<Object> GetV8Obj() = 0;
	virtual void Dispatch(JsDbg* d);
	
};

#define INHERIT_FROM_JSDEBUGEVENT					\
	void FillEventInfo(JsDebugEventInfo* evInfo);   \
	Local<Object> GetV8Obj();						\
	void Dispatch(JsDbg* d)

class JsDebugExceptionEvent : public JsDebugEvent {

protected:
	
	JsDebugExceptionEvent(const EXCEPTION_DEBUG_INFO* info);
	JsDebugExceptionEvent(const EXCEPTION_RECORD* rec);

public:	

	static JsDebugExceptionEvent* GetInstance(const EXCEPTION_DEBUG_INFO* info);
	static JsDebugExceptionEvent* GetInstance(const EXCEPTION_RECORD* rec);

	PVOID m_Address;
	DWORD m_Code;
	JsDebugExceptionEvent* m_Inner;

	INHERIT_FROM_JSDEBUGEVENT;

	static Handle<Value> GetAddress(Local<String> name, const AccessorInfo& info);
	
};

//// BEGIN JSDEBUGEXCEPTIONEVENT CHILDS

class JsExceptionAccessViolationEvent : public JsDebugExceptionEvent {
public:

	JsExceptionAccessViolationEvent(const EXCEPTION_DEBUG_INFO* info) : JsDebugExceptionEvent(info) { }
	JsExceptionAccessViolationEvent(const EXCEPTION_RECORD* rec) : JsDebugExceptionEvent(rec) { }

	INHERIT_FROM_JSDEBUGEVENT;
};

class JsExceptionBreakpointEvent : public JsDebugExceptionEvent {
public:

	JsExceptionBreakpointEvent(const EXCEPTION_DEBUG_INFO* info) : JsDebugExceptionEvent(info) { }
	JsExceptionBreakpointEvent(const EXCEPTION_RECORD* rec) : JsDebugExceptionEvent(rec) { }

	INHERIT_FROM_JSDEBUGEVENT;
};

class JsExceptionGuardPageEvent : public JsDebugExceptionEvent {
public:

	JsExceptionGuardPageEvent(const EXCEPTION_DEBUG_INFO* info) : JsDebugExceptionEvent(info) { }
	JsExceptionGuardPageEvent(const EXCEPTION_RECORD* rec) : JsDebugExceptionEvent(rec) { }

	INHERIT_FROM_JSDEBUGEVENT;
};

class JsExceptionSingleStepEvent : public JsDebugExceptionEvent {
public:

	JsExceptionSingleStepEvent(const EXCEPTION_DEBUG_INFO* info) : JsDebugExceptionEvent(info) { }
	JsExceptionSingleStepEvent(const EXCEPTION_RECORD* rec) : JsDebugExceptionEvent(rec) { }

	INHERIT_FROM_JSDEBUGEVENT;
};

//// END JSDEBUGEXCEPTIONEVENT CHILDS

class JsDebugCreateThreadEvent : public JsDebugEvent {
public:

	INHERIT_FROM_JSDEBUGEVENT;

	JsDebugCreateThreadEvent(const CREATE_THREAD_DEBUG_INFO* info);
};

class JsDebugCreateProcessEvent : public JsDebugEvent {
public:

	INHERIT_FROM_JSDEBUGEVENT;

	JsDebugCreateProcessEvent(const CREATE_PROCESS_DEBUG_INFO* info);
};

class JsDebugExitThreadEvent : public JsDebugEvent {
public:

	INHERIT_FROM_JSDEBUGEVENT;

	JsDebugExitThreadEvent(const EXIT_THREAD_DEBUG_INFO* info);
};

class JsDebugExitProcessEvent : public JsDebugEvent {
public:

	INHERIT_FROM_JSDEBUGEVENT;

	JsDebugExitProcessEvent(const EXIT_PROCESS_DEBUG_INFO* info);
};

class JsDebugLoadDllEvent : public JsDebugEvent {
public:
	JsSystemDll* m_SystemDll;

	INHERIT_FROM_JSDEBUGEVENT;

	JsDebugLoadDllEvent(const LOAD_DLL_DEBUG_INFO* info);

	static Handle<Value> GetSystemDll(Local<String> name, const AccessorInfo& info);
};

class JsDebugUnloadDllEvent : public JsDebugEvent {
public:
	void* m_Base;

	INHERIT_FROM_JSDEBUGEVENT;

	JsDebugUnloadDllEvent(const UNLOAD_DLL_DEBUG_INFO* info);
};

class JsDebugOutputStringEvent : public JsDebugEvent {
public:

	INHERIT_FROM_JSDEBUGEVENT;
	
	JsDebugOutputStringEvent(const OUTPUT_DEBUG_STRING_INFO* info);
};

class JsDebugRipEvent : public JsDebugEvent {
public:

	INHERIT_FROM_JSDEBUGEVENT;

	JsDebugRipEvent(const RIP_INFO* info);
};

JsDebugEvent* GetInstance(const DEBUG_EVENT* dbgEvent);

#endif