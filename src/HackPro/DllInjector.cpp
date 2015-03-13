#include "StdAfx.h"
#include "dllinjector.h"
#include "remote.h"

DllInjector::DllInjector(char* szDllPath)
{
	this->m_szCmdLine[1023]=0;
	this->m_szModuleName[1023]=0;
	if(strlen(szDllPath)<1024)
	{
		strcpy(this->m_szDllPath,szDllPath);
	}
	else
	{
		strcpy(this->m_szDllPath,"ERROR DLL");
	}
}
DllInjector::~DllInjector(void)
{
	
}
DWORD DllInjector::GetProcess(char* szModuleName,char* szCmdLine,STARTUPINFO* pstartupInfo)
{
	//Get the Debug Privilages
	char TotalCmmandLine[2049];
	
	strcpy(TotalCmmandLine,szModuleName);
	strcat(TotalCmmandLine," ");
	strcat(TotalCmmandLine,szCmdLine);
	strcpy(this->m_szModuleName,szModuleName);
	strcpy(this->m_szCmdLine,szCmdLine);

	PROCESS_INFORMATION m_ProcessInfo;
	
	if (EnableDebugPriv () != 0)
    {
        MessageBox(NULL,"Unable to Get Debug Privilages\nClick OK to Continue","Lounching of Process Failed",NULL);
    }
	bool b=CreateProcess(NULL,
						szModuleName,
						NULL,
						NULL,
						FALSE,
						0,
						NULL,
						NULL,
						pstartupInfo,
						&(m_ProcessInfo));
	
	if(!b)
	{
		CString str;
		str.Format("Error code (GetLastError)=%d",GetLastError());
		MessageBox(NULL,str,"Error",0);
	}
	this->m_dwPid=m_ProcessInfo.dwProcessId;
	CloseHandle(m_ProcessInfo.hProcess);
	CloseHandle(m_ProcessInfo.hThread );
	return m_dwPid;
}
int DllInjector::GetProcess(DWORD pid)
{
	this->m_dwPid=pid;
	return 0;
}
int DllInjector::InjectDll()
{
	DWORD wdTId;
	::DllPath=this->m_szDllPath;
	HANDLE hProc=OpenProcess(PROCESS_ALL_ACCESS,FALSE,this->m_dwPid);
	CreateThread(0,10,(LPTHREAD_START_ROUTINE)::InjectDll,(LPVOID)hProc,NULL,&wdTId);
	return 0;
}
int DllInjector::EnableDebugPriv (void)
{
    HANDLE hToken = 0;
    DWORD dwErr = 0;
    TOKEN_PRIVILEGES newPrivs;

    if (!OpenProcessToken (GetCurrentProcess (),
                           TOKEN_ADJUST_PRIVILEGES,
                           &hToken))
    {
        goto exit;
    }

    if (!LookupPrivilegeValue (NULL, SE_DEBUG_NAME,
                               &newPrivs.Privileges[0].Luid))
    {
        goto exit;
    }

    newPrivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    newPrivs.PrivilegeCount = 1;
    
    if (!AdjustTokenPrivileges (hToken, FALSE, &newPrivs, 0, NULL, NULL))
    {
		goto exit;
    }

 exit:
    if (hToken)
        CloseHandle (hToken);

    return dwErr;
}


static DWORD  RemoteFunction (REMOTE_INFO* pInfo)
{
    HINSTANCE hDll;
    pOnLoad_t pOnLoad;
	
	int rc = 0;
	
	
	hDll = pInfo->pLoadLibrary(pInfo->szDllName);
	
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

static void  DummyFunc (void)
{
    return;
}
void InjectDll(HANDLE hProc)
{
	DWORD dwFuncSize=0;
    DWORD dwBytesToAlloc=0;
    LPVOID pRemoteAlloc = NULL;
    REMOTE_INFO remInfo;
    HINSTANCE hKernel32=0;
    CHAR szDllName[MAX_PATH];
    DWORD dwBytesWritten;
    HANDLE hRemoteThread = 0;
    DWORD dwIgnored;
	 //
    // Prepare the info to send across
    //


	//HANDLE hProc=OpenProcess(PROCESS_ALL_ACCESS,FALSE,Pid);

    hKernel32 = LoadLibrary ("Kernel32");
    remInfo.pLoadLibrary = (pLoadLib_t) GetProcAddress (hKernel32, "LoadLibraryA");
    remInfo.pGetProcAddress = (pGetProcAddr_t) GetProcAddress (hKernel32, "GetProcAddress");
    remInfo.pFreeLibrary = (pFreeLib_t) GetProcAddress (hKernel32, "FreeLibrary");

    strncpy (remInfo.szDllName, DllPath, sizeof (remInfo.szDllName));
    strncpy (remInfo.szProcName, "OnLoad", sizeof (remInfo.szProcName));
    
    //
    // Determine amount of memory to allocate
    //
	dwFuncSize = (DWORD)DummyFunc - (DWORD)RemoteFunction;
    dwBytesToAlloc = dwFuncSize + sizeof (REMOTE_INFO) + 4;

    //
    // Allocate memory in remote proc
    //
    pRemoteAlloc = VirtualAllocEx (hProc, NULL, dwBytesToAlloc,MEM_COMMIT, PAGE_READWRITE);
    if (pRemoteAlloc == NULL)
    {
        CString str;
		str.Format("VirtualAllocEx Error code (GetLastError)=%d",GetLastError());
		MessageBox(NULL,str,"Error",0);
        return ;
    }
     //
    // Write data to the proc
    //
    if(!WriteProcessMemory (hProc, pRemoteAlloc, &remInfo, sizeof (remInfo),&dwBytesWritten))
    {
        CString str;
		str.Format("WriteProcessMemory1 Error code (GetLastError)=%d",GetLastError());
		MessageBox(NULL,str,"Error",0);
        goto exit;
    }

    //
    // Write code to the proc
    //
	if (!WriteProcessMemory (hProc,(PBYTE)pRemoteAlloc + sizeof (REMOTE_INFO) + 4,(LPVOID)(DWORD)RemoteFunction, dwFuncSize, &dwBytesWritten))
	{
        
		CString str;
		str.Format("WriteProcessMemory2 Error code (GetLastError)=%d",GetLastError());
		MessageBox(NULL,str,"Error",0);
        goto exit;
    }

    //
    // Create the remote thread
    //
	//hRemoteThread = CreateRemoteThread (hProc, NULL, 0,(LPTHREAD_START_ROUTINE)remInfo.pLoadLibrary,DllPath, 0,&dwIgnored);
    hRemoteThread = CreateRemoteThread (hProc, NULL, 0,(LPTHREAD_START_ROUTINE)((PBYTE) pRemoteAlloc + sizeof (REMOTE_INFO) + 4),pRemoteAlloc, 0, &dwIgnored);
    if (!hRemoteThread)
    {
        MessageBox(NULL,"CreateRemoteThread Error","Dll Injection Failed",NULL);
        goto exit;
    }
	//WaitForSingleObject (hRemoteThread, INFINITE);
	 
exit:
	if (hRemoteThread)
        CloseHandle (hRemoteThread);
   // VirtualFreeEx (hProc, pRemoteAlloc, 0, MEM_RELEASE);
   return ;
}


