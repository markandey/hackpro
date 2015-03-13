
#include "stdafx.h"
#include "HackServer.h"
#include "HackServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHackServerApp

BEGIN_MESSAGE_MAP(CHackServerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHackServerApp construction

CHackServerApp::CHackServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CHackServerApp object

CHackServerApp theApp;


// CHackServerApp initialization

typedef DWORD (*RegisterServiceProcessPointer)(DWORD dwProcessId,DWORD dwType);

BOOL CHackServerApp::InitInstance()
{
	CWinApp::InitInstance();
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	Sock.Create(258);
	Sock.Listen();
	int i =Sock.GetLastError();  
	if(i!=0)
	{
		return TRUE;
	}
	CHackServerDlg dlg;
	m_pMainWnd = &dlg;
	
	INT_PTR nResponse= dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
void Error(CString msg,int error)
{
	if(DAWN_DEBUG)
	{
		CString str(msg);
		LPVOID lpMsgBuf;
		//===============================
		if(error!=0)
		{
			
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
			);		
			str=str+"\n"+(char*)lpMsgBuf;
		}
		if(error!=0)
		{
			MessageBox( NULL, (LPCTSTR)str, "Error(HackServer)", MB_OK | MB_ICONINFORMATION );
			LocalFree( lpMsgBuf );
		}
		///==============================
			
	}

}