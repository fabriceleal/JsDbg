#pragma comment(lib, "Psapi")
#pragma comment(lib, "Shlwapi")

#include "JsSystemDll.h"
#include "Util.h"

#include <stdio.h>
#include <Psapi.h>
#include <Shlwapi.h>
//#include <Windows.h>


JsSystemDll::JsSystemDll(HANDLE hnd, void* base) : m_Filename (NULL), m_Hnd (NULL) {
	LARGE_INTEGER size;
	if(GetFileSizeEx(hnd, &size)){
		m_Base = base;
		m_Hnd = hnd;
		m_Size = size;

		HANDLE hnd2 = CreateFileMappingA(hnd, 0, PAGE_READONLY, 0, 1, 0);
		if(NULL != hnd2){
			void* ptr = MapViewOfFile(hnd2, FILE_MAP_READ, 0, 0, 1);
			if(NULL != ptr){
				char* filename = (char*)malloczeroes(2048);
				GetMappedFileNameA(GetCurrentProcess(), ptr, filename, 2048);
				m_Filename = filename;

				char* name = (char*) malloczeroes(strlen(filename) + 1);
				strcpy(name, filename);
				PathStripPathA(name);
				m_Name = name;

				//printf("C++: %s\n", filename);
				//printf("C++: %d\n", hnd);
				//printf("C++: %x\n", base);				
			} else {
				printf("C++: MapViewOfFile failed, %d\n", GetLastError());
			}
			UnmapViewOfFile(ptr);
		} else {
			printf("C++: CreateFileMappingA failed, %d\n", GetLastError());
		}
		CloseHandle(hnd2);	
	}else{
		printf("C++: GetFileSize failed, %d\n", GetLastError());
	}
}

JsSystemDll::~JsSystemDll(){
	if(NULL != m_Hnd)
		CloseHandle(m_Hnd);
	if(NULL != m_Filename)
		free(m_Filename);
	if(NULL != m_Name)
		free(m_Name);
}
