
#include "stdafx.h"
#include "HackPro.h"
#include "Remote.h"

static DWORD  RemoteFunction (REMOTE_INFO* Param)
{
    int rc = -1;
	HINSTANCE hDll;
    pOnLoad_t pOnLoad;

	REMOTE_INFO *pInfo=(REMOTE_INFO*)Param;
    
	hDll = pInfo->pLoadLibrary (pInfo->szDllName);
    if (hDll != NULL)
    {
        pOnLoad = (pOnLoad_t) pInfo->pGetProcAddress (hDll, pInfo->szProcName);
        if (pOnLoad != 0)
        {
            rc = pOnLoad();
        }
        pInfo->pFreeLibrary (hDll);
    }
    return rc;
}
//
// This is just a dummy used to get the address after RemoteFunction
//
static void  DummyFunc (void)
{
    return;
}
