#pragma comment(lib, "node")

#include <node.h>
#include <v8.h>
#include <string>
//#include <gcroot.h>
//#include <iostream>
#include <uv.h>

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

	// Register classes in module
	target->Set(String::NewSymbol("JsDbg"), s_ct->GetFunction());
}

Handle<Value> JsDbg::New(const Arguments& args){
	HandleScope scope;
				
	// Create object
	JsDbg* o = new JsDbg();

	// Wrap and return
	o->Wrap(args.This());
	return args.This();
}

Handle<Value> JsDbg::load(const Arguments& args){
	// Unwrap instance
	JsDbg* xthis = ObjectWrap::Unwrap<JsDbg>(args.This());
	
	String::AsciiValue s(args[0]);

	printf("You want to load %s\n", *s);

	DWORD res = xthis->Load(*s);
	if(res != 0){
		// Some error!
		printf("ERR: %d\n", res);
	} 
		
	return Undefined();
}

Handle<Value> JsDbg::attach(const Arguments& args){
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
	// Unwrap instance
	JsDbg* xthis = ObjectWrap::Unwrap<JsDbg>(args.This());

	DWORD res  = xthis->Detach();
	if(res != 0){
		printf("ERR: %d\n", res);
	}

	return Undefined();
}

// list threads

struct ListThreadsData {
	uv_work_t request;
	Persistent<Function> callback;
	bool error;
	std::string error_message;
	
	JsDbg* _this;

	int res_len;
	JsThread** res_arr;
};

static void listThreadsAsyncWork(uv_work_t* req){
	ListThreadsData* data = static_cast<ListThreadsData*>(req->data);

	data->_this->ListThreads(&data->res_arr, &data->res_len);
}

static void listThreadsAsyncAfter(uv_work_t* req){
	HandleScope hnd;
	ListThreadsData* data = static_cast<ListThreadsData*>(req->data);

	if(data->error){
		// TODO: ...
	} else {
		Local<Array> a = Array::New(data->res_len);
		for(int i = 0; i < data->res_len; ++i){
			a->Set(i, JsThread::GetV8Obj(data->res_arr[i]));		
		}

		Local<Value> argv[1] = {
			//Local<Value>::New(Null()), No... No ...
			a
		};

		TryCatch try_catch;
		data->callback->Call(Context::GetCurrent()->Global(), 1, argv);
		if(try_catch.HasCaught()){
			FatalException(try_catch);
		}
	}
	// Do not free the data->res_*
	delete data;
}

Handle<Value> JsDbg::listThreads(const Arguments& args){
	// Get callback function
	Local<Function> cb = Local<Function>::Cast(args[0]);

	ListThreadsData* data = new ListThreadsData();
	data->error = false;
	data->request.data = data;
	data->callback = Persistent<Function>::New(cb);
	data->res_len = 0;
	data->res_arr = NULL;
	data->_this = ObjectWrap::Unwrap<JsDbg>(args.This());

	int status = uv_queue_work(uv_default_loop(), &data->request, listThreadsAsyncWork, listThreadsAsyncAfter);
	assert(status == 0);

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