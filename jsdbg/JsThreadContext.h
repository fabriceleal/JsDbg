#ifndef __JSTHREADCONTEXT_H
#define __JSTHREADCONTEXT_H

#include <node.h>
#include <v8.h>

#include <Windows.h>

using namespace v8;
using namespace node;

#define DEFINE_NODE_ACCESSORS(FIELD)		\
		static Handle<Value> Get ## FIELD ##(Local<String> name, const AccessorInfo& info); \
		static void Set ## FIELD ##(Local<String> name, Local<Value> value, const AccessorInfo& info)

class JsThreadContext  {
public:

	DWORD p_Dr0;
	DWORD p_Dr1;
	DWORD p_Dr2;
	DWORD p_Dr3;
	DWORD p_Dr6;
	DWORD p_Dr7;

	FLOATING_SAVE_AREA p_FloatSave;

	DWORD p_SegGs;
	DWORD p_SegFs;
	DWORD p_SegEs;
	DWORD p_SegDs;

	DWORD p_Edi;
	DWORD p_Esi;
	DWORD p_Ebx;
	DWORD p_Edx;
	DWORD p_Ecx;
	DWORD p_Eax;
	DWORD p_Ebp;
	DWORD p_Eip;
	
	DWORD p_SegCs;
	DWORD p_EFlags;
	
	DWORD p_Esp;

	DWORD p_SegSs;
	
	BYTE p_ExtendedRegisters[MAXIMUM_SUPPORTED_EXTENSION];
	
	
	JsThreadContext(LPCONTEXT threadCtx){ 
		

#define COPY_FIELD(FIELD)								\
	printf("C++: %s should be %d\n", #FIELD, threadCtx-> ## FIELD ##); \
	this->p_ ## FIELD ## = threadCtx-> ## FIELD


		COPY_FIELD(Dr0);
		COPY_FIELD(Dr1);
		COPY_FIELD(Dr2);
		COPY_FIELD(Dr3);
		COPY_FIELD(Dr6);
		COPY_FIELD(Dr7);

		COPY_FIELD(SegGs);
		COPY_FIELD(SegFs);
		COPY_FIELD(SegEs);
		COPY_FIELD(SegDs);

		COPY_FIELD(Edi);
		COPY_FIELD(Esi);
		COPY_FIELD(Ebx);
		COPY_FIELD(Edx);
		COPY_FIELD(Ecx);
		COPY_FIELD(Eax);
		COPY_FIELD(Ebp);
		COPY_FIELD(Eip);

		COPY_FIELD(SegCs);
		COPY_FIELD(EFlags);

		COPY_FIELD(Esp);
		COPY_FIELD(SegSs);
	}

	static Local<Object> GetV8Obj(JsThreadContext* t);
	
	
	DEFINE_NODE_ACCESSORS(Dr0);
	DEFINE_NODE_ACCESSORS(Dr1);
	DEFINE_NODE_ACCESSORS(Dr2);
	DEFINE_NODE_ACCESSORS(Dr3);
	DEFINE_NODE_ACCESSORS(Dr6);
	DEFINE_NODE_ACCESSORS(Dr7);

	DEFINE_NODE_ACCESSORS(FloatSave);

	DEFINE_NODE_ACCESSORS(SegGs);
	DEFINE_NODE_ACCESSORS(SegFs);
	DEFINE_NODE_ACCESSORS(SegEs);
	DEFINE_NODE_ACCESSORS(SegDs);

	DEFINE_NODE_ACCESSORS(Edi);
	DEFINE_NODE_ACCESSORS(Esi);
	DEFINE_NODE_ACCESSORS(Ebx);
	DEFINE_NODE_ACCESSORS(Edx);
	DEFINE_NODE_ACCESSORS(Ecx);
	DEFINE_NODE_ACCESSORS(Eax);
	DEFINE_NODE_ACCESSORS(Ebp);
	DEFINE_NODE_ACCESSORS(Eip);
	
	DEFINE_NODE_ACCESSORS(SegCs);
	DEFINE_NODE_ACCESSORS(EFlags);
	
	DEFINE_NODE_ACCESSORS(Esp);

	DEFINE_NODE_ACCESSORS(SegSs);
	
	DEFINE_NODE_ACCESSORS(ExtendedRegisters);


} ;



#endif