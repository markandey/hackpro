// HackPro.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HackPro.h"
#include "HackProDlg.h"
#include <initguid.h>
#include "HackPro_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHackProApp


class CHackProModule :
	public CAtlMfcModule
{
public:
	DECLARE_LIBID(LIBID_HackProLib);
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_HACKPRO, "{FFF03D3E-70DF-4512-8140-E0C0031D26BC}");};

CHackProModule _AtlModule;

BEGIN_MESSAGE_MAP(CHackProApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHackProApp construction

CHackProApp::CHackProApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CHackProApp object

CHackProApp theApp;

// CHackProApp initialization

BOOL CHackProApp::InitInstance()
{
	AfxOleInit();
	if (!AfxSocketInit())
	{
		MessageBox(NULL,"Socket Failed !","Error",0);
		return FALSE;
	}
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Register class factories via CoRegisterClassObject().
	if (FAILED(_AtlModule.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE)))
		return FALSE;
	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Don't show the main window
		return TRUE;
	}
	// App was launched with /Unregserver or /Unregister switch.
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		_AtlModule.UpdateRegistryAppId(FALSE);
		_AtlModule.UnregisterServer(TRUE);
		return FALSE;
	}
	// App was launched with /Register or /Regserver switch.
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
	{
		_AtlModule.UpdateRegistryAppId(TRUE);
		_AtlModule.RegisterServer(TRUE);
		return FALSE;
	}


	CHackProDlg* dlg=new CHackProDlg();
	m_pMainWnd = dlg;
	INT_PTR nResponse = dlg->DoModal();
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
	delete dlg;
	return FALSE;
}


BOOL CHackProApp::ExitInstance(void)
{
	_AtlModule.RevokeClassObjects();
	return CWinApp::ExitInstance();
}
