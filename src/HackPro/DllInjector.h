#ifndef DLLINJECTOR_H
#define DLLINJECTOR_H
#include "Papahead.h"

void InjectDll(HANDLE hProc);
static char* DllPath;
class DllInjector
{
private:
	char m_szModuleName[1024];
	char m_szDllPath[1024];
	char m_szCmdLine[1024];
	DWORD m_dwPid;
		
	int EnableDebugPriv(void);
public:
	DWORD GetProcess(char* szModuleName,char* szCmdLine,STARTUPINFO* pstartupInfo);
	int GetProcess(DWORD pid);
	int InjectDll ();

	//static DWORD WINAPI RemoteFunction(REMOTE_INFO* Param);
	//static void WINAPI DummyFunc (void);
	DllInjector(char*);
	~DllInjector(void);
};
#endif //DLLINJECTOR_H