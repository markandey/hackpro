// RevEngineer.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "RevEngineer.h"
#include "DllInjector.h"


// CRevEngineer dialog

IMPLEMENT_DYNAMIC(CRevEngineer, CDialog)
CRevEngineer::CRevEngineer(CWnd* pParent /*=NULL*/)
	: CDialog(CRevEngineer::IDD, pParent)
{
}

CRevEngineer::~CRevEngineer()
{
}

void CRevEngineer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAME, m_EdtModPath);
	DDX_Control(pDX, IDC_FILENAME2, m_EdtCmdLine);
	DDX_Control(pDX, IDC_PNEW, m_PaneNew);
	DDX_Control(pDX, IDC_POLD, m_PaneOld);
	DDX_Control(pDX, IDC_BRWS, m_btBrowseExe);
	DDX_Control(pDX, IDC_BTEXECUTE, m_btExe);
	DDX_Control(pDX, IDC_COMBO1, m_CmbProcList);
	DDX_Control(pDX, IDC_BTREFRESH, m_Refresh);
}


BEGIN_MESSAGE_MAP(CRevEngineer, CDialog)
	ON_BN_CLICKED(IDC_BRWS, OnBnClickedBrws)
	ON_BN_CLICKED(IDC_BTEXECUTE, OnBnClickedBtexecute)
	ON_BN_CLICKED(IDC_RNEW, OnBnClickedRnew)
	ON_BN_CLICKED(IDC_RRUN, OnBnClickedRrun)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BTREFRESH, OnBnClickedBtrefresh)
	ON_BN_CLICKED(IDC_BRWS2, OnBnClickedBrws2)
	ON_BN_CLICKED(IDC_TERMINATE, OnBnClickedTerminate)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CRevEngineer message handlers

void CRevEngineer::OnBnClickedBrws()
{
	CFileDialog f(true,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"Executable Files(*.exe)|*.exe|All Files(*.*)|*.*||",this);
	f.DoModal();
	CString str=f.GetPathName();
	this->m_EdtModPath.SetWindowText(str);
}

void CRevEngineer::OnBnClickedBtexecute()
{
	LounchProcess();
}
void CRevEngineer::LounchProcess()
{
	char  ModuleName[1024];
	char  CmdLine[1024];
	char  DllPath[]="F:\\hackpro_SP\\apihijack_src\\TestDLL.dll";

	
	STARTUPINFO startupInfo;
	memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);

	PROCESS_INFORMATION processInfo;
	this->m_EdtModPath.GetWindowText(ModuleName,1023);
	this->m_EdtCmdLine.GetWindowText(CmdLine,1023);
	
	DllInjector di("");
    DWORD pid=di.GetProcess(ModuleName,CmdLine,&startupInfo);

	this->SetDlgItemInt(IDC_EDTPID,pid);
	
	
	
}

BOOL CRevEngineer::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->m_EdtModPath.SetWindowText("Calc");
	OnBnClickedRrun();
	this->CheckRadioButton(IDC_RNEW,IDC_RRUN,IDC_RRUN);
	GetProcessList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRevEngineer::OnBnClickedRnew()
{
	this->m_btBrowseExe.EnableWindow();
	this->m_btExe.EnableWindow();
	this->m_EdtCmdLine.EnableWindow();
	this->m_EdtModPath.EnableWindow();

	this->m_CmbProcList.EnableWindow(false);
	this->m_Refresh.EnableWindow(false);

}

void CRevEngineer::OnBnClickedRrun()
{
	this->m_btBrowseExe.EnableWindow(false);
	this->m_btExe.EnableWindow(false);
	this->m_EdtCmdLine.EnableWindow(false);
	this->m_EdtModPath.EnableWindow(false);

	this->m_CmbProcList.EnableWindow();
	this->m_Refresh.EnableWindow();
}
int CRevEngineer::GetProcessList()
{
	HANDLE         hProcessSnap = NULL; 
    BOOL           bRet      = FALSE; 
    PROCESSENTRY32 pe32      = {0}; 
	this->m_CmbProcList.ResetContent();
	
	ProcInfo info;
	
 
    //  Take a snapshot of all processes in the system. 

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

    if (hProcessSnap == INVALID_HANDLE_VALUE) 
        return (FALSE); 
 
    //  Fill in the size of the structure before using it. 

    pe32.dwSize = sizeof(PROCESSENTRY32); 
 
    //  Walk the snapshot of the processes, and for each process, 
    //  display information. 

    if (Process32First(hProcessSnap, &pe32)) 
    { 
        
        do 
        { 
            		
			info.Pid=pe32.th32ProcessID;
			info.Pri=pe32.pcPriClassBase;
			info.ThreadCount=pe32.cntThreads;
            if(strlen(pe32.szExeFile)<30)
			{
				strcpy(info.szExe,pe32.szExeFile);
			}
			else
			{
				strcpy(info.szExe,"Name not found");
			}
			CString str;
			str.Format("%s (Pid=%d)",info.szExe,info.Pid);
			int index=this->m_CmbProcList.AddString(str);
			this->m_CmbProcList.SetItemData(index,info.Pid);
	   } 
        while (Process32Next(hProcessSnap, &pe32)); 
        bRet = TRUE; 
    } 
    else 
        bRet = FALSE;    // could not walk the list of processes 
 
    // Do not forget to clean up the snapshot object. 

    CloseHandle (hProcessSnap); 
	this->m_CmbProcList.SetWindowText("Select Process");
	return (bRet); 
}

void CRevEngineer::OnCbnSelchangeCombo1()
{
	int index=this->m_CmbProcList.GetCurSel();
	DWORD pid=this->m_CmbProcList.GetItemData(index);
	this->SetDlgItemInt(IDC_EDTPID,pid);
}

void CRevEngineer::OnBnClickedBtrefresh()
{
	this->GetProcessList();
}

void CRevEngineer::OnBnClickedBrws2()
{
	CFileDialog f(true,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"Dynamic Link Library(*.dll)|*.dll|All Files(*.*)|*.*||",this);
	f.DoModal();
	CString str=f.GetPathName();
	this->SetDlgItemText(IDC_DLLPATH,str);
}

void CRevEngineer::OnBnClickedTerminate()
{
	
	int index=this->m_CmbProcList.GetCurSel();
	DWORD Pid=this->m_CmbProcList.GetItemData(index);
	HANDLE HProc;
	if(index!=-1)
	{
		DWORD pid=this->m_CmbProcList.GetItemData(index);
		HProc=OpenProcess(PROCESS_TERMINATE,FALSE,Pid);//Get Desired Access
		if(HProc!=NULL)
		{
			TerminateProcess(HProc,0); //Terminate the Process			
		}
		else
		{
			MessageBox("Process not Found");
		}
		this->GetProcessList();
	}
	else
	{
		MessageBox("Select Any Process");
	}
}

void CRevEngineer::OnBnClickedButton1()
{
	char DllPath[1024];
	this->GetDlgItemText(IDC_DLLPATH,DllPath,1023);
	DllInjector di(DllPath);
	di.GetProcess(this->GetDlgItemInt(IDC_EDTPID));
	di.InjectDll();
}
