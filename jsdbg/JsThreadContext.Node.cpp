#pragma comment(lib, "node")

#include <node.h>
#include <v8.h>
#include <string>
//#include <gcroot.h>
#include <iostream>
#include <uv.h>

#include "JsThreadContext.h"

using namespace node;
using namespace v8;

#define SET_ACCESSOR(FIELD)		\
	inst->SetAccessor(String::New( #FIELD ), Get ## FIELD ##, Set ## FIELD ## )

#define DEFINE_ACESSORS_DWORD(FIELD)													\
	Handle<Value> JsThreadContext::Get ## FIELD ## (Local<String> name, const AccessorInfo& info){	\
		Local<Object> self = info.Holder();											\
		Local<Value> v = self->GetInternalField(0);									\
		Local<External> e = Local<External>::Cast(v);								\
		JsThreadContext* t = reinterpret_cast<JsThreadContext*>(e->Value());		\
		return Integer::New(t->p_ ## FIELD ##);										\
	}																				\
																					\
	void JsThreadContext::Set ## FIELD ## (Local<String> name, Local<Value> value, const AccessorInfo& info){	\
		Local<Object> self = info.Holder();													\
		Local<Value> v = self->GetInternalField(0);											\
		Local<External> e = Local<External>::Cast(v);										\
		JsThreadContext* t = reinterpret_cast<JsThreadContext*>(e->Value());				\
		t->p_ ## FIELD ## = value->Uint32Value();											\
	}

Local<Object> JsThreadContext::GetV8Obj(JsThreadContext* t){

	Handle<FunctionTemplate> temp = FunctionTemplate::New();
	temp->SetClassName(String::New("JsThreadContext"));

	Handle<ObjectTemplate> inst = temp->InstanceTemplate();
	inst->SetInternalFieldCount(1);
		
	SET_ACCESSOR(Dr0);
	SET_ACCESSOR(Dr1);
	SET_ACCESSOR(Dr2);
	SET_ACCESSOR(Dr3);
	SET_ACCESSOR(Dr6);
	SET_ACCESSOR(Dr7);

	SET_ACCESSOR(SegGs);
	SET_ACCESSOR(SegFs);
	SET_ACCESSOR(SegEs);
	SET_ACCESSOR(SegDs);

	SET_ACCESSOR(Edi);
	SET_ACCESSOR(Esi);
	SET_ACCESSOR(Ebx);
	SET_ACCESSOR(Edx);
	SET_ACCESSOR(Ecx);
	SET_ACCESSOR(Eax);
	SET_ACCESSOR(Ebp);
	SET_ACCESSOR(Eip);

	SET_ACCESSOR(SegCs);
	SET_ACCESSOR(EFlags);

	SET_ACCESSOR(Esp);
	SET_ACCESSOR(SegSs);
		
	Handle<Function> ctor = temp->GetFunction();
	Local<Object> o = ctor->NewInstance();
	o->SetInternalField(0, External::New(t));

	return o;
}

/*Handle<Value> GetDr0(Local<String> name, const AccessorInfo& info){
	Local<Object> self = info.Holder();
	Local<Value> v = self->GetInternalField(0);
	Local<External> e = Local<External>::Cast(v);
	JsThreadContext* t = reinterpret_cast<JsThreadContext*>(e->Value());

	return Integer::New(t->p_Dr0);
}

static void SetDr0(Local<String> name, Local<Value> value, const AccessorInfo& info){
	Local<Object> self = info.Holder();
	Local<Value> v = self->GetInternalField(0);
	Local<External> e = Local<External>::Cast(v);
	JsThreadContext* t = reinterpret_cast<JsThreadContext*>(e->Value());

	t->p_Dr0 = value->Uint32Value();
}

*/
DEFINE_ACESSORS_DWORD(Dr0)
DEFINE_ACESSORS_DWORD(Dr1)
DEFINE_ACESSORS_DWORD(Dr2)
DEFINE_ACESSORS_DWORD(Dr3)
DEFINE_ACESSORS_DWORD(Dr6)
DEFINE_ACESSORS_DWORD(Dr7)

DEFINE_ACESSORS_DWORD(SegGs)
DEFINE_ACESSORS_DWORD(SegFs)
DEFINE_ACESSORS_DWORD(SegEs)
DEFINE_ACESSORS_DWORD(SegDs)

DEFINE_ACESSORS_DWORD(Edi)
DEFINE_ACESSORS_DWORD(Esi)
DEFINE_ACESSORS_DWORD(Ebx)
DEFINE_ACESSORS_DWORD(Edx)
DEFINE_ACESSORS_DWORD(Ecx)
DEFINE_ACESSORS_DWORD(Eax)
DEFINE_ACESSORS_DWORD(Ebp)
DEFINE_ACESSORS_DWORD(Eip)

DEFINE_ACESSORS_DWORD(SegCs)
DEFINE_ACESSORS_DWORD(EFlags)

DEFINE_ACESSORS_DWORD(Esp)
DEFINE_ACESSORS_DWORD(SegSs)

#undef SET_ACCESSOR
#undef DEFINE_ACESSORS_DWORD