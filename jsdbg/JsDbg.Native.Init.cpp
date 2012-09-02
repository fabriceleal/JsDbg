#pragma comment(lib, "Advapi32")

//#include <Windows.h>
//#include <WinBase.h>
#include "JsDbg.h"

BOOL SetPrivilege( 
	HANDLE hToken,  // token handle 
	LPCTSTR Privilege,  // Privilege to enable/disable 
	BOOL bEnablePrivilege  // TRUE to enable. FALSE to disable 
) 
{ 
	TOKEN_PRIVILEGES tp = { 0 }; 
	// Initialize everything to zero 
	LUID luid; 
	DWORD cb=sizeof(TOKEN_PRIVILEGES); 
	if(!LookupPrivilegeValue( NULL, Privilege, &luid )){
		printf("C++: Err 5\n");
		return FALSE; 
	}
	tp.PrivilegeCount = 1; 
	tp.Privileges[0].Luid = luid; 
	if(bEnablePrivilege) { 
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	} else { 
		tp.Privileges[0].Attributes = 0; 
	} 
	AdjustTokenPrivileges( hToken, FALSE, &tp, cb, NULL, NULL ); 
	if (GetLastError() != ERROR_SUCCESS) {
		printf("C++: Err 6\n");
		return FALSE; 
	}
	return TRUE;
}

void JsDbg::Init(){
	printf("Start init...\n");

	HANDLE hToken;                        
    TOKEN_PRIVILEGES token_privileges;                  
    DWORD dwSize;                        
    ZeroMemory (&token_privileges, sizeof (token_privileges));
    token_privileges.PrivilegeCount = 1;
    if ( !OpenProcessToken (GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken)){
        //return FALSE;
		printf("C++ Err 1 \n");
		return;
	}
    if (!LookupPrivilegeValue ( NULL, SE_DEBUG_NAME, &token_privileges.Privileges[0].Luid))
    { 
        CloseHandle (hToken);
        printf("C++ Err 2 \n");
		return;
    }
 
    token_privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges ( hToken, FALSE, &token_privileges, 0, NULL, &dwSize))
    { 
        CloseHandle (hToken);
        printf("C++ Err 3 \n");
		return;
    }
    CloseHandle (hToken);
    //return TRUE;

	return;
	/*
	HANDLE hToken;

	printf("C++: Init, starting\n");
	printf("C++: Thread ID: %d\n", GetCurrentThreadId());

	if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken))
    {
        if (GetLastError() == ERROR_NO_TOKEN)
        {
            if (!ImpersonateSelf(SecurityImpersonation))
			{
				printf("C++: Err 1\n");
				return; // ERR
			}

            if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken)){
				printf("C++: Err 2\n");
                return; // ERR
            }
         }
        else
		{
			printf("C++: Err 3\n");
			return; // ERR
		}
     }

    // enable SeDebugPrivilege
    if(!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
    {
		printf("C++: Err 4\n");
        CloseHandle(hToken);

        // indicate failure
        return; // ERR
    }
	*/
	//CloseHandle(hToken);
}