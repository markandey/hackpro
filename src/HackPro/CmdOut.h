
#ifndef CMDOUT_H
#define CMDOUT_H
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"

class CCmdOut
{
public:
	static DWORD BUFSIZE;
protected:
	// the thread details
	CWinThread* m_hThread;
	// the read and write handles for me (w.r.t. me)
	HANDLE m_hRead,m_hWrite;
	// the read and write handles for the child process (w.r.t. child)
	HANDLE m_hCRead,m_hCWrite;

	// the name of the executable , i.e. the child process
	LPCTSTR m_ChildProcess;
	// the output of the child process 
	CString m_ChOutPut;

	// functions
	// create the pipes
	bool CreateComnPipes();
	// the thread for the listening of the pipe and register the output
	static UINT ReadOutput(LPARAM lParam);
	// report the errors
	void ReportError(DWORD Error);

public:
	// constructor
	 CCmdOut(void);
	// destructor
	 ~CCmdOut(void);
	// the command line based program to execute
	bool Execute(LPCTSTR ExeName);
	// get the output
	void GetOutput(CString& Out);
};

#endif