#ifndef __JSDEBUGEVENTS_H
#define __JSDEBUGEVENTS_H

#include <Windows.h>

class JsDebugEvent {

public:

	void Dispatch(JsDbg* d);

	static JsDebugEvent GetInstance(DEBUG_EVENT* dbgEvent){
		switch(dbgEvent->dwDebugEventCode){
		case 0x1:
			return new JsDebugExceptionEvent(&dbgEvent->u);
		case 0x2:
			return new JsDebugCreateThreadEvent(&dbgEvent->u);
		case 0x3:
			return new JsDebugCreateProcessEvent(&dbgEvent->u);
		case 0x4:
			return new JsDebugExitThreadEvent(&dbgEvent->u);
		case 0x5:
			return new JsDebugExitProcessEvent(&dbgEvent->u);
		case 0x6:
			return new JsDebugLoadDllEvent(&dbgEvent->u);
		case 0x7:
			return new JsDebugUnloadDllEvent(&dbgEvent->u);
		case 0x8:
			return new JsDebugOutputStringEvent(&dbgEvent->u);
		case 0x9:
			return new JsDebugRipEvent(&dbgEvent->u);
		}
	}

};

class JsDebugExceptionEvent : JsDebugEvent {
public:
	JsDebugExceptionEvent(EXCEPTION_DEBUG_INFO* info);
};

class JsDebugCreateThreadEvent : JsDebugEvent {
public:
	JsDebugCreateThreadEvent(CREATE_THREAD_DEBUG_INFO* info);
};

class JsDebugCreateProcessEvent : JsDebugEvent {
public:
	JsDebugCreateProcessEvent(CREATE_PROCESS_DEBUG_INFO* info);
};

class JsDebugExitThreadEvent : JsDebugEvent {
public:
	JsDebugExitThreadEvent(EXIT_THREAD_DEBUG_INFO* info);
}

class JsDebugExitProcessEvent : JsDebugEvent {
public:
	JsDebugExitProcessEvent(EXIT_PROCESS_DEBUG_INFO* info);
};

class JsDebugLoadDllEvent : JsDebugEvent {
public:
	JsDebugLoadDllEvent(LOAD_DLL_DEBUG_INFO* info);
};

class JsDebugUnloadDllEvent : JsDebugEvent {
public:
	JsDebugUnloadDllEvent(UNLOAD_DLL_DEBUG_INFO* info);
};

class JsDebugOutputStringEvent : JsDebugEvent {
public:
	JsDebugOutputStringEvent(OUTPUT_DEBUG_STRING_INFO* info);
};

class JsDebugRipEvent : JsDebugEvent {
public:
	JsDebugRipEvent(RIP_INFO* info);
};

#endif