#ifndef __JS_SYSTEMDLL_H
#define __JS_SYSTEMDLL_H

#include <node.h>
#include <v8.h>

#include <Windows.h>

using namespace node;
using namespace v8;

class JsSystemDll {
public:
	char* m_Filename;
	char* m_Name;
	void* m_Base;
	HANDLE m_Hnd;
	LARGE_INTEGER m_Size;

	JsSystemDll(HANDLE hnd, void* base);
	~JsSystemDll();

	Local<Object> GetV8Obj();

	static Handle<Value> GetFilename(Local<String> name, const AccessorInfo& info);
	static Handle<Value> GetName(Local<String> name, const AccessorInfo& info);
	static Handle<Value> GetBase(Local<String> name, const AccessorInfo& info);
	static Handle<Value> GetHnd(Local<String> name, const AccessorInfo& info);
	static Handle<Value> GetSize(Local<String> name, const AccessorInfo& info);
};

#endif