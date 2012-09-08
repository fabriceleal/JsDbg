#ifndef __UTILV8_H
#define __UTILV8_H


#ifndef SET_ACCESSOR
#define SET_ACCESSOR(FIELD)		\
	inst->SetAccessor(String::New( #FIELD ), Get ## FIELD ##)
#endif SET_ACCESSOR



#ifndef DEFINE_ACCESSOR
#define DEFINE_ACCESSOR(FIELD, SOURCE_INST_TYPE, SOURCE_TYPE, TARGET_TYPE)								\
	Handle<Value> SOURCE_INST_TYPE ## ::Get ## FIELD ## (Local<String> name, const AccessorInfo& info){	\
		Local<Object> self = info.Holder();																\
		Local<Value> v = self->GetInternalField(0);														\
		Local<External> e = Local<External>::Cast(v);													\
		SOURCE_INST_TYPE ## * t = reinterpret_cast< ## SOURCE_INST_TYPE ## *>(e->Value());				\
		return TARGET_TYPE ## ::New((SOURCE_TYPE) t->m_ ## FIELD ##);									\
	}

#endif DEFINE_ACCESSOR

#ifndef DEFINE_ACCESSOR_CS
#define DEFINE_ACCESSOR_CS(FIELD, SOURCE_INST_TYPE, LAMBDA, TARGET_TYPE)								\
	Handle<Value> SOURCE_INST_TYPE ## ::Get ## FIELD ## (Local<String> name, const AccessorInfo& info){	\
		Local<Object> self = info.Holder();																\
		Local<Value> v = self->GetInternalField(0);														\
		Local<External> e = Local<External>::Cast(v);													\
		SOURCE_INST_TYPE ## * t = reinterpret_cast< ## SOURCE_INST_TYPE ## *>(e->Value());				\
		return TARGET_TYPE ## ::New( LAMBDA ## (t->m_ ## FIELD ##));									\
	}

#endif DEFINE_ACCESSOR_CS

#ifndef DEFINE_ACCESSOR_V8OBJ
#define DEFINE_ACCESSOR_V8OBJ(FIELD, SOURCE_INST_TYPE)													\
	Handle<Value> SOURCE_INST_TYPE ## ::Get ## FIELD ## (Local<String> name, const AccessorInfo& info){	\
		Local<Object> self = info.Holder();																\
		Local<Value> v = self->GetInternalField(0);														\
		Local<External> e = Local<External>::Cast(v);													\
		SOURCE_INST_TYPE ## * t = reinterpret_cast< ## SOURCE_INST_TYPE ## *>(e->Value());				\
		return t->m_ ## FIELD ## ->GetV8Obj();															\
	}

#endif DEFINE_ACCESSOR_V8OBJ

#ifndef IMPLEMENT_GETV8OBJ
#define IMPLEMENT_GETV8OBJ(CLASS, INIT_CODE)						\
	Local<Object> CLASS ##::GetV8Obj() { 							\
		Handle<FunctionTemplate> temp = FunctionTemplate::New();	\
		temp->SetClassName(String::New(#CLASS));					\
																	\
		Handle<ObjectTemplate> inst = temp->InstanceTemplate();		\
		inst->SetInternalFieldCount(1);								\
																	\
		INIT_CODE													\
																	\
		Handle<Function> ctor = temp->GetFunction();				\
		Local<Object> o = ctor->NewInstance();						\
		o->SetInternalField(0, External::New(this));				\
																	\
		return o;													\
	}
#endif

#endif