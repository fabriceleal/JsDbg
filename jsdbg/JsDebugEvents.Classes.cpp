
#include "JsDebugEvents.h"

// printf("C++: (%d) %s\n", strlen( #UNQUOTED_NAME ), #UNQUOTED_NAME);		

#define IMPLEMENT_FILLEVENTINFO(CLASS, UNQUOTED_NAME)								\
	void CLASS ## ::FillEventInfo(JsDebugEventInfo* evInfo) {						\
		evInfo->name = (char*)malloc(1 + sizeof(char) * strlen( #UNQUOTED_NAME ));	\
		strcpy(evInfo->name, #UNQUOTED_NAME);										\
	}


// "Exception" is very generic
IMPLEMENT_FILLEVENTINFO(JsDebugExceptionEvent, Exception)
// These are more specific exceptions ...
IMPLEMENT_FILLEVENTINFO(JsExceptionAccessViolationEvent, AccessViolation)
IMPLEMENT_FILLEVENTINFO(JsExceptionBreakpointEvent, Breakpoint)
IMPLEMENT_FILLEVENTINFO(JsExceptionSingleStepEvent, SingleStep)
IMPLEMENT_FILLEVENTINFO(JsExceptionGuardPageEvent, GuardPage)

IMPLEMENT_FILLEVENTINFO(JsDebugCreateThreadEvent, CreateThread)
IMPLEMENT_FILLEVENTINFO(JsDebugCreateProcessEvent, CreateProcess)
IMPLEMENT_FILLEVENTINFO(JsDebugExitThreadEvent, ExitThread)
IMPLEMENT_FILLEVENTINFO(JsDebugExitProcessEvent, ExitProcess)
IMPLEMENT_FILLEVENTINFO(JsDebugLoadDllEvent, LoadDll)
IMPLEMENT_FILLEVENTINFO(JsDebugUnloadDllEvent, UnloadDll)
IMPLEMENT_FILLEVENTINFO(JsDebugOutputStringEvent, OutputString)
IMPLEMENT_FILLEVENTINFO(JsDebugRipEvent, Rip)

#undef IMPLEMENT_FILLEVENTINFO

#define IMPLEMENT_GETV8OBJ(CLASS, INIT_CODE)						\
	Local<Object> CLASS ##::GetV8Obj() { 							\
		Handle<FunctionTemplate> temp = FunctionTemplate::New();	\
		temp->SetClassName(String::New(#CLASS));					\
																	\
		Handle<ObjectTemplate> inst = temp->InstanceTemplate();		\
		inst->SetInternalFieldCount(1);								\
																	\
		INIT_CODE													\
																	\
		Handle<Function> ctor = temp->GetFunction();				\
		Local<Object> o = ctor->NewInstance();						\
		o->SetInternalField(0, External::New(this));				\
																	\
		return o;													\
	}

#define SET_ACCESSOR(FIELD)		\
	inst->SetAccessor(String::New( #FIELD ), Get ## FIELD ##)

#define DEFINE_ACESSORS_INTEGER(FIELD, SOURCE_INST_TYPE, TARGET_TYPE)													\
	Handle<Value> SOURCE_INST_TYPE ## ::Get ## FIELD ## (Local<String> name, const AccessorInfo& info){	\
		Local<Object> self = info.Holder();											\
		Local<Value> v = self->GetInternalField(0);									\
		Local<External> e = Local<External>::Cast(v);								\
		SOURCE_INST_TYPE ## * t = reinterpret_cast< ## SOURCE_INST_TYPE ## *>(e->Value());		\
		return TARGET_TYPE ## ::New((int32_t) t->m_ ## FIELD ##);										\
	}	

DEFINE_ACESSORS_INTEGER(Address, JsDebugExceptionEvent, Integer)

// Generic...
IMPLEMENT_GETV8OBJ(JsDebugExceptionEvent, {
	SET_ACCESSOR(Address);
})

//Specific ...
IMPLEMENT_GETV8OBJ(JsExceptionAccessViolationEvent, {
	SET_ACCESSOR(Address);
})
IMPLEMENT_GETV8OBJ(JsExceptionBreakpointEvent, {
	SET_ACCESSOR(Address);
})
IMPLEMENT_GETV8OBJ(JsExceptionSingleStepEvent, {
	SET_ACCESSOR(Address);
})
IMPLEMENT_GETV8OBJ(JsExceptionGuardPageEvent, {
	SET_ACCESSOR(Address);
})

IMPLEMENT_GETV8OBJ(JsDebugCreateThreadEvent, {})
IMPLEMENT_GETV8OBJ(JsDebugCreateProcessEvent, {})
IMPLEMENT_GETV8OBJ(JsDebugExitThreadEvent, {})
IMPLEMENT_GETV8OBJ(JsDebugExitProcessEvent, {})
IMPLEMENT_GETV8OBJ(JsDebugLoadDllEvent, {})
IMPLEMENT_GETV8OBJ(JsDebugUnloadDllEvent, {})
IMPLEMENT_GETV8OBJ(JsDebugOutputStringEvent, {})
IMPLEMENT_GETV8OBJ(JsDebugRipEvent, {})

#undef IMPLEMENT_GETV8OBJ

#define DECLARE_CONSTR(CLASS, STRUCT)		CLASS ## :: ## CLASS ## (const STRUCT ## * dbgInfo)

DECLARE_CONSTR(JsDebugExceptionEvent, EXCEPTION_DEBUG_INFO)  : m_Address (dbgInfo->ExceptionRecord.ExceptionAddress), m_Code (dbgInfo->ExceptionRecord.ExceptionCode) {
	printf("C++: Address should be: %d\n", m_Address);
	if(NULL != dbgInfo->ExceptionRecord.ExceptionRecord){
		m_Inner = JsDebugExceptionEvent::GetInstance((const EXCEPTION_RECORD*) &dbgInfo->ExceptionRecord.ExceptionRecord);
	} else {
		m_Inner = NULL;
	}
}

DECLARE_CONSTR(JsDebugExceptionEvent, EXCEPTION_RECORD) : m_Address (dbgInfo->ExceptionAddress), m_Code (dbgInfo->ExceptionCode){ 
	printf("C++: Address should be: %d\n", m_Address);
	if(NULL != dbgInfo->ExceptionRecord){
		m_Inner = JsDebugExceptionEvent::GetInstance((const EXCEPTION_RECORD*) &dbgInfo->ExceptionRecord);
	} else {
		m_Inner = NULL;
	}
}

DECLARE_CONSTR(JsDebugCreateThreadEvent, CREATE_THREAD_DEBUG_INFO){ 

}

DECLARE_CONSTR(JsDebugCreateProcessEvent, CREATE_PROCESS_DEBUG_INFO){ 

}

DECLARE_CONSTR(JsDebugExitThreadEvent, EXIT_THREAD_DEBUG_INFO){ 

}

DECLARE_CONSTR(JsDebugExitProcessEvent, EXIT_PROCESS_DEBUG_INFO){ 

}

DECLARE_CONSTR(JsDebugLoadDllEvent, LOAD_DLL_DEBUG_INFO){ 
	printf("C++: HANDLE: %d\n", dbgInfo->hFile);
	void** imgName = (void**)dbgInfo->lpImageName;
	if(NULL != imgName && 
			NULL != *imgName)
	{
		if(0 == dbgInfo->fUnicode){
			printf("C++: NAME: %s\n", *imgName);
		} else {
			printf("C++: NAME: %ls\n", *imgName);
		}
	}

	CloseHandle(dbgInfo->hFile);
}

DECLARE_CONSTR(JsDebugUnloadDllEvent, UNLOAD_DLL_DEBUG_INFO){ 

}

DECLARE_CONSTR(JsDebugOutputStringEvent, OUTPUT_DEBUG_STRING_INFO){ 

}

DECLARE_CONSTR(JsDebugRipEvent, RIP_INFO){ 

}

#undef DECLARE_CONSTR
