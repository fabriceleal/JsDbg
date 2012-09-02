#pragma comment(lib, "node")

#include <node.h>
#include <v8.h>
#include <string>
//#include <gcroot.h>
//#include <iostream>
#include <uv.h>

#include "JsDebugEvents.h"

using namespace node;
using namespace v8;


void JsDebugEvent::Dispatch(JsDbg* dbg){
	// Call the emit method of JsDbg. It will inherit from EventEmitter
	HandleScope hd;
	JsDebugEventInfo* info = (JsDebugEventInfo*)malloc(sizeof(JsDebugEventInfo));

	this->FillEventInfo(info);
	
	Handle<Value> argv[2] = 
	{
		String::New(info->name, strlen(info->name)),
		this->GetV8Obj()
	};
		
	MakeCallback(dbg->handle_, "emit", 2, argv);
		
	free(info);
	
}

