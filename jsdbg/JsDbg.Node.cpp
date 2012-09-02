#pragma comment(lib, "node")

#include <node.h>
#include <v8.h>
#include <string>
//#include <gcroot.h>
//#include <iostream>
//#include <uv.h>

#include "JsDbg.h"
#include "JsThread.h"

using namespace node;
using namespace v8;

void NODE_EXTERN JsDbg::Init(Handle<Object> target){
	HandleScope scope;

	// set ctor function
	Local<FunctionTemplate> t = FunctionTemplate::New(New);

	// set the node.js class name
	s_ct = Persistent<FunctionTemplate>::New(t);
	s_ct->InstanceTemplate()->SetInternalFieldCount(1);
	s_ct->SetClassName(String::NewSymbol("JsDbg"));

	// Register class member functions
	NODE_SET_PROTOTYPE_METHOD(s_ct, "load", load);
	NODE_SET_PROTOTYPE_METHOD(s_ct, "attach", attach);
	NODE_SET_PROTOTYPE_METHOD(s_ct, "detach", detach);
	NODE_SET_PROTOTYPE_METHOD(s_ct, "listThreads", listThreads);
	NODE_SET_PROTOTYPE_METHOD(s_ct, "run", run);
	NODE_SET_PROTOTYPE_METHOD(s_ct, "funcResolve", funcResolve);
	NODE_SET_PROTOTYPE_METHOD(s_ct, "setBp", setBp);

	// Register classes in module
	target->Set(String::NewSymbol("JsDbg"), s_ct->GetFunction());
}

Handle<Value> JsDbg::New(const Arguments& args){
	HandleScope scope;
	printf("C++: JsDbg.new: %x\n", args.This());

	// Create object
	JsDbg* o = new JsDbg();
		
	o->Wrap(args.This());
	o->Ref();
	//o->m_jsObj = args.This();	

	printf("C++: Handle: %x\n", o->handle_);

	printf("C++: JsDbg.new: %x\n", args.This());

	return args.This();
}

Handle<Value> JsDbg::load(const Arguments& args){
	HandleScope scope;
	printf("C++: JsDbg.load: %x\n", args.This());

	// TODO Make this async

	// Unwrap instance
	JsDbg* xthis = ObjectWrap::Unwrap<JsDbg>(args.This());
	
	String::AsciiValue s(args[0]);

	printf("You want to load %s\n", *s);

	DWORD res = xthis->Load(*s);
	if(res != 0){
		// Some error!
		printf("ERR: %d\n", res);
	} 

	// Raise event, a little pointless I guess...

	Handle<Value> argv[1] = 
	{
		String::New("Loaded", 6)		
	};
	
	printf("C++, load: Raising callback!\n");
	MakeCallback(xthis->handle_, "emit", 1, argv);

	return Undefined();
}

Handle<Value> JsDbg::attach(const Arguments& args){
	//HandleScope scope;

	printf("C++ JsDbg.attach: %x\n", args.This());

	// Unwrap instance
	JsDbg* xthis = ObjectWrap::Unwrap<JsDbg>(args.This());

	Local<Integer> i = Integer::New(0);
	if(args.Length() > 0){
		i = args[0]->ToInteger();
	}
	DWORD res = xthis->Attach(i->IntegerValue());
	if(res != 0){
		printf("ERR: %d\n", res);
		DWORD d = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM;		
		LPTSTR buffer = NULL;		
		DWORD w = ::FormatMessage(d, NULL, res, 0, (LPTSTR)&buffer, 0, NULL);		
		printf("ERR: %ls\n", buffer);
		if(w == 0){
			printf("%d\n", GetLastError());
		}		
		LocalFree(buffer);
	}

	return Undefined();
}

Handle<Value> JsDbg::detach(const Arguments& args){
	//HandleScope scope;

	printf("C++ JsDbg.detach: %x\n", args.This());

	// Unwrap instance
	JsDbg* xthis = ObjectWrap::Unwrap<JsDbg>(args.This());

	DWORD res  = xthis->Detach();
	if(res != 0){
		printf("ERR: %d\n", res);
	}

	return Undefined();
}

Handle<Value> JsDbg::run(const Arguments& args){
	//HandleScope scope;

	printf("C++ JsDbg.run: %x\n", args.This());

	// Unwrap instance
	JsDbg* xthis = ObjectWrap::Unwrap<JsDbg>(args.This());

	xthis->StartDebugThread();
	
	return Undefined();
}

Handle<Value> JsDbg::funcResolve(const Arguments& args){
	//HandleScope scope;

	printf("C++ JsDbg.funcResolve: %x\n", args.This());

	// Unwrap instance
	JsDbg* xThis = ObjectWrap::Unwrap<JsDbg>(args.This());
	String::AsciiValue s1(args[0]);
	String::AsciiValue s2(args[1]);
	
	return Integer::New(xThis->FuncResolve(*s1, *s2));
}

Handle<Value> JsDbg::setBp(const Arguments& args){
	//HandleScope scope;

	printf("C++ JsDbg.run: %x\n", args.This());

	// Unwrap instance
	JsDbg* xThis = ObjectWrap::Unwrap<JsDbg>(args.This());
	long address = args[0]->ToInteger()->IntegerValue();

	xThis->SetBp(address);
	
	return Undefined();
}

Persistent<FunctionTemplate> JsDbg::s_ct;

extern "C" {
	void NODE_EXTERN init(Handle<Object> target)
	{
		JsDbg::Init(target);
	}
	NODE_MODULE(sharp, init);
}
