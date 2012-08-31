#pragma comment(lib, "node")

#include <node.h>
#include <v8.h>
#include <string>
//#include <gcroot.h>
#include <iostream>
#include <uv.h>

#include "JsThread.h"

using namespace node;
using namespace v8;

Local<Object> JsThread::GetV8Obj(JsThread* t) {
	
	Handle<FunctionTemplate> temp = FunctionTemplate::New();
	temp->SetClassName(String::New("JsThread"));

	Handle<ObjectTemplate> inst = temp->InstanceTemplate();
	inst->SetInternalFieldCount(1);

	inst->SetAccessor(String::New("threadId"), GetThreadId);
	inst->SetAccessor(String::New("processId"), GetProcessId);
	
	Handle<ObjectTemplate> proto = temp->PrototypeTemplate();
	proto->Set("getContext", FunctionTemplate::New(GetContext));
	
	Handle<Function> ctor = temp->GetFunction();
	Local<Object> o = ctor->NewInstance();
	o->SetInternalField(0, External::New(t));
		
	return o;
}

Persistent<FunctionTemplate> JsThread::s_ct;


Handle<Value> JsThread::GetThreadId(Local<String> name, const AccessorInfo& info){
	Local<Object> self = info.Holder();
	Local<Value> v = self->GetInternalField(0);
	Local<External> e = Local<External>::Cast(v);
	JsThread* t = reinterpret_cast<JsThread*>(e->Value());

	return Integer::New(t->thread_id());
}

Handle<Value> JsThread::GetProcessId(Local<String> name, const AccessorInfo& info){
	Local<Object> self = info.Holder();
	Local<Value> v = self->GetInternalField(0);
	Local<External> e = Local<External>::Cast(v);
	JsThread* t = reinterpret_cast<JsThread*>(e->Value());

	return Integer::New(t->process_id());
}

Handle<Value> JsThread::GetContext(const Arguments& args){
	JsThread* xthis = ObjectWrap::Unwrap<JsThread>(args.This());

	JsThreadContext* ctx = xthis->get_context();

	return JsThreadContext::GetV8Obj(ctx);
}