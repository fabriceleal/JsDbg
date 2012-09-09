// Implement here Base event Handlers

#include <PeLib.h>
//#include <PeFile.h>

#include <vector>
#include <list>

#include "JsDbg.h"
#include "JsDebugEvents.h"
#include "JsBuffer.h"

#define DUMMY_IMPLEMENTATION(CLASS)					\
	void CLASS ## ::Dispatch(JsDbg* d){				\
		JsDebugEvent::Dispatch(d);					\
	}

void JsDebugLoadDllEvent::Dispatch(JsDbg* d){
	// Push system dll into list of system dlls	
	d->m_systemdlls->push_back(m_SystemDll);

	if(0 == strcmp(m_SystemDll->m_Name, "kernel32.dll"))
	{
		printf("C++: KERNEL32.DLL!\n");
		// Assume 32 bits
		PeLib::PeFile32* pe = new PeLib::PeFile32();
		try 
		{
			JsBuffer* buf = d->BufReadProcessMemory((BYTE*) m_SystemDll->m_Base, 2048); //837632);
			if(buf)
			{
				printf("C++: SUCCESS reading memory...\n");

				if(0 != pe->mzHeader().read((unsigned char*)buf->m_Buffer, (unsigned int) buf->m_Length))
				{
					printf("C++: Error reading mz header\n");
				}

				if(0 != pe->peHeader().read((unsigned char*)buf->m_Buffer, (unsigned int) buf->m_Length, pe->mzHeader().getAddressOfPeHeader()))
				{
					printf("C++: Error reading pe header\n");
				}

				delete buf;
			} 
			else 
			{
				printf("C++: Err reading memory...\n");
			}
		}
		catch(string err) 
		{
			printf("Error: %s", err);
		}
		delete pe;
	}

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