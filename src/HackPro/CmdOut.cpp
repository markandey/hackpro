#include "StdAfx.h"
#include "cmdout.h"

DWORD CCmdOut::BUFSIZE=4096;

CCmdOut::CCmdOut(void):
	m_hRead(NULL),m_hWrite(NULL),m_hCRead(NULL),m_hCWrite(NULL)
{
	this->m_ChOutPut="";
}

CCmdOut::~CCmdOut(void)
{
	if(m_hRead)
	{
		CloseHandle(m_hRead);
	}
	if(m_hWrite)
	{
		CloseHandle(m_hWrite);
	}
	if(m_hCRead)
	{
		CloseHandle(m_hCRead);
	}
	if(m_hCWrite)
	{
		CloseHandle(m_hCWrite);
	}
}

// report the errors
void CCmdOut::ReportError(DWORD Error)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,Error,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,0,NULL);
	MessageBox(NULL,(LPCTSTR)lpMsgBuf,"Error",MB_OK|MB_ICONINFORMATION);
	// Free the buffer.
	LocalFree( lpMsgBuf );
}

// the command line based program to execute
bool CCmdOut::Execute(LPCTSTR ExeName)
{
	CString Temp;
	Temp=ExeName;
	if(Temp=="Redeemer")
	{
		this->m_ChOutPut="Abhishek,Aman Kumar,Bhaskar Priya,Markanday Singh,Manu Dev and Ritwaj Ratan Constitute Redeemer.";
		this->m_ChOutPut+="\r\nWe are Team Redeemer.";
		return true;
	}
	// make the output variable empty to prepare for the next output
	m_ChOutPut.Empty();
	// the return value
	bool Ret=false;
	// if we are not executing anything currently
	// then we can surely execute the ExeName
	m_ChildProcess=ExeName;
	// initialize the pipe and the thread
	if(CreateComnPipes()==true)
	{
		BOOL Success;
		// the security attributes
		// set the security attributes such that child can inherit the handles
		// and default security descriptor
		SECURITY_ATTRIBUTES SecAttr;
		SecAttr.nLength=sizeof(SECURITY_ATTRIBUTES);
		SecAttr.lpSecurityDescriptor=NULL;
		SecAttr.bInheritHandle=TRUE;

		// the process information
		PROCESS_INFORMATION	ChildProcInfo;
		
		// the startup information
		STARTUPINFO SInfo;
		ZeroMemory(&SInfo,sizeof(STARTUPINFO));
		SInfo.cb=sizeof(STARTUPINFO);
		SInfo.dwFlags=STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
		// the window is hidden
		SInfo.wShowWindow=SW_HIDE;
		// the child should use th following handles as StdIn and StdOut
		// there is no StdError handle use NULL
		SInfo.hStdOutput=m_hCWrite;
		SInfo.hStdInput=m_hCRead;
		SInfo.hStdError=NULL;

		// now create the process
		Success=CreateProcess(NULL,(LPSTR)m_ChildProcess,&SecAttr,&SecAttr,TRUE,NORMAL_PRIORITY_CLASS,
			NULL,NULL,&SInfo,&ChildProcInfo);
		if(!Success)
		{
			this->ReportError(GetLastError());
			Ret=false;
		}
		// the pipe and process creation has been successful
		// start the thread to listen to the output of the command line program
		m_hThread=AfxBeginThread((AFX_THREADPROC)ReadOutput,this,THREAD_PRIORITY_NORMAL);
		if(m_hThread->m_hThread==NULL)
		{
			// the thread could not be created successfully
			Ret=false;
		}

		{
			// wait for the process to complete and then close the thread
			HANDLE Proc;
			Proc=OpenProcess(PROCESS_ALL_ACCESS,TRUE,ChildProcInfo.dwProcessId);
			if(Proc!=INVALID_HANDLE_VALUE)
			{
				WaitForSingleObject(Proc,INFINITE);
				TerminateProcess(Proc,0);
				CloseHandle(Proc);
				Ret=true;
			}
			// just for safety's sake terminate the thread
			TerminateThread(m_hThread->m_hThread,0);
		}
	}
	Ret=true;
	return Ret;
}

// create the pipes
bool CCmdOut::CreateComnPipes()
{
	bool Ret=true;
	BOOL Success;

	SECURITY_ATTRIBUTES SecAttr;
	SecAttr.nLength=sizeof(SECURITY_ATTRIBUTES);
	SecAttr.bInheritHandle=TRUE;
	SecAttr.lpSecurityDescriptor=NULL;

	// create the pipe for where we can read from and the child process can write to
	// the Read Handle w.r.t. 'Us' is m_hRead;
	// the Write Handle w.r.t. 'Child' is m_hCWrite;
	// initialize both of them
	Success=CreatePipe(&m_hRead,&m_hCWrite,&SecAttr,CCmdOut::BUFSIZE);
	if(!Success)
	{
		// there is an error in creation of the pipe
		// report the error and set the return variable to false
		this->ReportError(GetLastError());
		Ret=false;
	}

	// now for creation of the pipe where we will write to and the child process will read from
	// the Write Handle w.r.t. 'Us' is m_hWrite
	// the Read Handle w.r.t. 'Child' is m_hCRead;
	Success=CreatePipe(&m_hCRead,&m_hWrite,&SecAttr,CCmdOut::BUFSIZE);
	if(!Success)
	{
		// there is an error in creation of the pipe
		// report the error and set the return variable to false
		this->ReportError(GetLastError());
		Ret=false;
	}

	return(Ret);
}

// the thread for the listening of the pipe and register the output
UINT CCmdOut::ReadOutput(LPARAM lParam)
{
	CCmdOut* pThis=(CCmdOut*)lParam;
	ASSERT(pThis);
	// just in case the full buffer is utilized even then there will be
	// a space for the terminating NULL
	char chBuf[4097];
	chBuf[BUFSIZE]='\0';
	// the amount of data actually read into the buffer
	DWORD dwRead;
	// connect to the pipe and start reading from the pipe till
	// 1.Pipe is broken
	// 2.The Stop Flag in the papa is reset
	while(true)
	{
		if(!ReadFile(pThis->m_hRead,chBuf,BUFSIZE,&dwRead,NULL)||dwRead==0)
		{
			// some error
			if(GetLastError()==ERROR_BROKEN_PIPE)
			{
				// the child process has exited
				// let us also exit
				break;
			}
		}
		else
		{
			// success we got something at the other end of the pipe
			chBuf[dwRead]='\0';
			pThis->m_ChOutPut+=chBuf;
		}
	}
	return(0);
}

// get the output
void CCmdOut::GetOutput(CString& Out)
{
	Out=m_ChOutPut;
}