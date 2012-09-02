//#include <Windows.h>

#include "JsDebugEvents.h"

JsDebugEvent* GetInstance(const DEBUG_EVENT* dbgEvent){
	switch(dbgEvent->dwDebugEventCode){
	case 0x1:
		//return new JsDebugExceptionEvent((EXCEPTION_DEBUG_INFO*) &dbgEvent->u);
		return JsDebugExceptionEvent::GetInstance((EXCEPTION_DEBUG_INFO*) &dbgEvent->u);
	case 0x2:
		return new JsDebugCreateThreadEvent((CREATE_THREAD_DEBUG_INFO*) &dbgEvent->u);
	case 0x3:
		return new JsDebugCreateProcessEvent((CREATE_PROCESS_DEBUG_INFO*) &dbgEvent->u);
	case 0x4:
		return new JsDebugExitThreadEvent((EXIT_THREAD_DEBUG_INFO*) &dbgEvent->u);
	case 0x5:
		return new JsDebugExitProcessEvent((EXIT_PROCESS_DEBUG_INFO*) &dbgEvent->u);
	case 0x6:
		return new JsDebugLoadDllEvent((LOAD_DLL_DEBUG_INFO*) &dbgEvent->u);
	case 0x7:
		return new JsDebugUnloadDllEvent((UNLOAD_DLL_DEBUG_INFO*) &dbgEvent->u);
	case 0x8:
		return new JsDebugOutputStringEvent((OUTPUT_DEBUG_STRING_INFO*) &dbgEvent->u);
	case 0x9:
		return new JsDebugRipEvent((RIP_INFO*) &dbgEvent->u);
	}
}

JsDebugExceptionEvent* JsDebugExceptionEvent::GetInstance(const EXCEPTION_DEBUG_INFO* info){
	return JsDebugExceptionEvent::GetInstance((const EXCEPTION_RECORD*)&info->ExceptionRecord);
}

JsDebugExceptionEvent* JsDebugExceptionEvent::GetInstance(const EXCEPTION_RECORD* rec){
	switch(rec->ExceptionCode){
	case EXCEPTION_ACCESS_VIOLATION:
		return new JsExceptionAccessViolationEvent(rec);
	case EXCEPTION_BREAKPOINT:
		return new JsExceptionBreakpointEvent(rec);
	case EXCEPTION_SINGLE_STEP:
		return new JsExceptionSingleStepEvent(rec);
	case EXCEPTION_GUARD_PAGE:
		return new JsExceptionGuardPageEvent(rec);
	}
	return new JsDebugExceptionEvent(rec);
}