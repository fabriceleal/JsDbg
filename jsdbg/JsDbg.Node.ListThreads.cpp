#pragma comment(lib, "node")

#include <node.h>
#include <v8.h>
//#include <uv.h>

#include "JsDbg.h"

// list threads
// No use in using async to get the list of threads. 

/*
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
	HandleScope scope;

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
*/

Handle<Value> JsDbg::listThreads(const Arguments& args){
	HandleScope scope;

	printf("C++ JsDbg.listThreads: %x\n", args.This());

	// Get callback function
	Local<Function> cb = Local<Function>::Cast(args[0]);
	/*
	ListThreadsData* data = new ListThreadsData();
	data->error = false;
	data->request.data = data;
	data->callback = Persistent<Function>::New(cb);
	data->res_len = 0;
	data->res_arr = NULL;
	data->_this = ObjectWrap::Unwrap<JsDbg>(args.This());

	int status = uv_queue_work(uv_default_loop(), &data->request, listThreadsAsyncWork, listThreadsAsyncAfter);
	assert(status == 0);
	*/

	JsDbg* _this = ObjectWrap::Unwrap<JsDbg>(args.This());
	JsThread** arr = NULL;
	int len = 0;
	
	_this->ListThreads(&arr, &len);

	Local<Array> a = Array::New(len);

	for(int i = 0; i < len; ++i){
		a->Set(i, JsThread::GetV8Obj(arr[i]));		
	}

	Local<Value> argv[1] = { a };
	TryCatch try_catch;
	cb->Call(Context::GetCurrent()->Global(), 1, argv);
	if(try_catch.HasCaught()){
		FatalException(try_catch);
	}

	return Undefined();
}
