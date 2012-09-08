#include "JsSystemDll.h"
#include "UtilV8.h"

DEFINE_ACCESSOR(Filename, JsSystemDll, char*, String)
DEFINE_ACCESSOR(Name, JsSystemDll, char*, String)
DEFINE_ACCESSOR(Base, JsSystemDll, int32_t, Integer)
DEFINE_ACCESSOR(Hnd, JsSystemDll, int32_t, Integer)
DEFINE_ACCESSOR_CS(Size, JsSystemDll, [](LARGE_INTEGER i){ return (double) i.QuadPart; } , Number)

IMPLEMENT_GETV8OBJ(JsSystemDll, {
	assert(sizeof(int32_t) >= sizeof(void*));
	assert(sizeof(int32_t) >= sizeof(HANDLE));
	assert(sizeof(double) >= sizeof(LARGE_INTEGER));

	SET_ACCESSOR(Filename);
	SET_ACCESSOR(Name);
	SET_ACCESSOR(Base);
	SET_ACCESSOR(Hnd);
	SET_ACCESSOR(Size);
})