// Implement here Base event Handlers

#include <vector>
#include <list>

#include "JsDbg.h"
#include "JsDebugEvents.h"

#define DUMMY_IMPLEMENTATION(CLASS)					\
	void CLASS ## ::Dispatch(JsDbg* d){	\
		JsDebugEvent::Dispatch(d);					\
	}

void JsDebugLoadDllEvent::Dispatch(JsDbg* d){
	// Push system dll into list of system dlls	
	d->m_systemdlls->push_back(m_SystemDll);

	//printf("C++: Load dll %s at %d\n", m_SystemDll->m_Filename, m_SystemDll->m_Base); //, i);

	JsDebugEvent::Dispatch(d);
}

void JsDebugUnloadDllEvent::Dispatch(JsDbg* d){
	JsSystemDll* system_dll = NULL;
	//int idx = -1;
		
	// Locate the system_dll
	//int i = 0;
	for(std::list<JsSystemDll*>::iterator it = d->m_systemdlls->begin(); 
        it != d->m_systemdlls->end(); 
		it++/*, i++*/)
	{
		if((*it)->m_Base == m_Base){
			system_dll = *it;
			//idx = i;
			break;
		}
	}

	if(NULL != system_dll){
		printf("C++: Unload dll %s found at %d\n", system_dll->m_Filename, system_dll->m_Base); //, i);
	} else {
		printf("C++: Unload dll wtf, %d\n", m_Base);
	}

	JsDebugEvent::Dispatch(d);

	// Take off system dll from list of system dlls
	if(NULL != system_dll){
		d->m_systemdlls->remove(system_dll);
		delete system_dll;
	}
}

DUMMY_IMPLEMENTATION(JsDebugExceptionEvent)
DUMMY_IMPLEMENTATION(JsExceptionAccessViolationEvent)
DUMMY_IMPLEMENTATION(JsExceptionBreakpointEvent)
DUMMY_IMPLEMENTATION(JsExceptionGuardPageEvent)
DUMMY_IMPLEMENTATION(JsExceptionSingleStepEvent)
DUMMY_IMPLEMENTATION(JsDebugCreateThreadEvent)
DUMMY_IMPLEMENTATION(JsDebugCreateProcessEvent)
DUMMY_IMPLEMENTATION(JsDebugExitThreadEvent)
DUMMY_IMPLEMENTATION(JsDebugExitProcessEvent)
DUMMY_IMPLEMENTATION(JsDebugOutputStringEvent)
DUMMY_IMPLEMENTATION(JsDebugRipEvent)

#undef DUMMY_IMPLEMENTATION