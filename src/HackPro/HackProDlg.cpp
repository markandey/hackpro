// HackProDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hackclient.h"
#include "HackPro.h"
#include "HackProDlg.h"



#include "regdatatree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INDEX 17
#define MAXCOUNT 20
#define RESCOUNT 10
int IndexDefault=44;
int IndexNode;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	
END_MESSAGE_MAP()


// CHackProDlg dialog

CHackProDlg::CHackProDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHackProDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//Page_RegConfig=new CRegConfigDlg;
}
CHackProDlg::~CHackProDlg()
{
	
	Shell_NotifyIcon(NIM_DELETE ,&NotifyData);
	for(int i=0;i<Comm.GetCount();i++)
	{
		CHackClient* T= Comm.GetAt(i);
		if(T!=NULL)
		{
			delete T;
		}
	}
	
}
void CHackProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MTREE, MTree);
}

BEGIN_MESSAGE_MAP(CHackProDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, IDC_MTREE, OnTvnSelchangedMtree)
	ON_NOTIFY(NM_CLICK, IDC_MTREE, OnNMClickMtree)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_TRAY_NOTIFY,OnTrayNotify)
	ON_COMMAND(ID_FILE_CONNECT, OnFileConnect)
	ON_COMMAND(ID_Menu, OnMenu)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_WINDOW_SHOWMAINWINDOW, OnWindowShowmainwindow)
	ON_COMMAND(ID_WINDOW_ABOUT, OnWindowAbout)
END_MESSAGE_MAP()


// CHackProDlg message handlers

BOOL CHackProDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	
	InitApplication();
	
	//---------------Add tray Icon-----------
	PNOTIFYICONDATA data=&NotifyData;
	data->cbSize=sizeof(NOTIFYICONDATA);
	data->hIcon=m_hIcon;
	data->hWnd=this->GetSafeHwnd();
	data->uFlags =NIF_ICON|NIF_MESSAGE|NIF_TIP;//|NIF_INFO;
    data->hIcon = m_hIcon;
	data->uVersion=NOTIFYICON_VERSION;
	strcpy(data->szInfo,"HackPro");
	strcpy(data->szTip,"HackPro By:Markandey Singh");
	strcpy(data->szInfoTitle,"HackPro");
	data->uID=WM_TRAY_NOTIFY;
	data->dwInfoFlags=NIIF_INFO;
	data->uTimeout=1000;										//Set Time out for the ballon Tooltip
	data->uCallbackMessage = WM_TRAY_NOTIFY;					//Set Tray Notification Handler
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHackProDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHackProDlg::OnPaint() 
{
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
HCURSOR CHackProDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//===========================ON Init Appl;ication===============================
void CHackProDlg::AddIcon()
{
	HICON hiconLarge[238],hiconSmall[238],I;
	HICON Misc[17],HDW[3],REG[9];
	ExtractIconEx("Shell32.dll",0,hiconLarge,hiconSmall,238);
	ExtractIconEx("main.cpl",0,hiconSmall,Misc,17);
	ExtractIconEx("Hdwwiz.cpl",0,hiconSmall,HDW,3);
	ExtractIconEx("regedit.exe",0,hiconSmall,REG,8);	
	ImgLst.Create(16,16,ILC_COLOR32,1,1);
	for(int i=0;i<238;i++)
	{
		I=hiconSmall[i];
		//I=ExtractIcon(AfxGetApp()->m_hInstance,"Shell32",i);
		ImgLst.Add(I);			 
	}
	for(i=0;i<18;i++)
	{
		I=Misc[i];
		ImgLst.Add(I);
	}
	for(i=0;i<3;i++)
	{
		I=HDW[i];
		ImgLst.Add(I);
	}
	for(i=0;i<8;i++)
	{
		I=REG[i];
		ImgLst.Add(I);
	}
	HICON ic;
	ic=AfxGetApp()->LoadIcon(IDI_ICON1);
	IndexNode=ImgLst.Add(ic);
	ImgLst.SetBkColor(15);//WHITE;
	MTree.SetBkColor(RGB(255,255,220)) ;
	//MTree.SetBkColor(RGB(100,200,255)) ;
	//MTree.SetTextColor(RGB(255,0,0)); 
	  
	

}
void CHackProDlg::InitApplication()
{
	
	AddIcon();
	MTree.SetImageList(&ImgLst,TVSIL_NORMAL);
	//========================imageList is Added with HTree and icons are loaded
	HTREEITEM hItem=MTree.InsertItem("HackPro",IndexNode,IndexNode);
	
	//============================
	RECT rt;
	GetDlgItem(IDC_MHOLD)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	Hold.Create(rt,this,678); 
	Hold.AddPage("Anatomy of a Hack",&Page_About,IDD_ABOUT);
	
	
	Hold.AddPage("Network\\Local\\Information",&Page_NetInfo,IDD_NETINFO); 
	Hold.AddPage("Network\\Local\\Statistics",&Page_NetStat,IDD_NETSTAT);
	Hold.AddPage("Network\\Local\\Connection Table",&Page_ConnectionTable,IDD_CONNECTIONTABLE);
	Hold.AddPage("Network\\FootPrint\\MAC Address",&Page_MacAddr,IDD_MACADD); 
	Hold.AddPage("Network\\FootPrint\\Packet Sniffer",&Page_PacketCap,IDD_PACKETCAP); 
	Hold.AddPage("Network\\Scan\\Single IP",&Page_PortScanSip,IDD_PORTSCANSIP);
	Hold.AddPage("Network\\Scan\\Single Port",&Page_PortScan,IDD_PORTSCAN);
	//Hold.AddPage("Network\\Scan\\Breach Detection",&Page_BreachDetector,IDD_VSCAN);
    Hold.AddPage("Network\\Denial of Service\\Smurf Attack",&Page_Smurf,IDD_DOSSMURF);
	Hold.AddPage("Network\\Denial of Service\\SYN Flag Attack",&Page_SynAttack,IDD_DOSSYN);
	Hold.AddPage("Network\\Denial of Service\\UDP Flood Attack",&Page_UDPFloodAttack,IDD_UDPFLOOD);
	Hold.AddPage("Network\\FootPrint\\Raw Packet Sender",&Page_RawPacket,IDD_DIALOGRAW);

	Hold.AddPage("Process Hacking\\Dll Injection",&Page_RevEngineer,IDD_REVENGG);
	
    this->AddHost("127.0.0.1"); 
    //============================
	//Expande the tree
	 MTree.Expand(hItem,TVE_EXPAND);
	

}
void CHackProDlg::AddHost(char* ip)
{
	CHackClient* C =new CHackClient();
	//e.g. Path = "System(127.0.0.1)"
	CString Path="System (";
	Path+=ip;
	Path+=")";
	if(C->Connect(ip)==true)
	{
		Hold.AddPage(Path+"\\Information\\SystemInfo",&(C->Page_SysInfo),IDD_DLGSYSINFO);
		Hold.AddPage(Path+"\\Control\\Process",&(C->Page_SysCtrl),IDD_DLGSYSCONTROL);
		Hold.AddPage(Path+"\\Control\\RemoteDesktop",&(C->Page_RemoteDesktop),IDD_DESKDLG);
		Hold.AddPage(Path+"\\Information\\Window List",&(C->Page_WndList), IDD_WNDLSTDLG); 
		//Hold.AddPage(Path+"\\Password\\Password Hash",&(C->Page_PwdHash), IDD_PWD_HASH); 
		Hold.AddPage(Path+"\\Registry\\Registry Editor",&(C->Page_RegConfig), IDD_REGCONFIG); 
		Comm.Add(C);
	}
	else
	{
		delete C;
	}
}
void CHackProDlg::RemoveHost(CString IP)
{
	this->SetFocus();
	CTreeCtrl* pCtrl = &MTree;
	HTREEITEM hCurrent = pCtrl->GetRootItem();
	hCurrent=pCtrl->GetNextItem(hCurrent,TVGN_NEXTVISIBLE);
	CString IPText="System (";
	IPText+=IP;
	IPText+=")";
	while (hCurrent != NULL)
	{
		CString ItemText;
		ItemText=pCtrl->GetItemText(hCurrent);
		if(ItemText==IPText)
		{
			pCtrl->DeleteItem(hCurrent);
			break;
		}
		hCurrent=pCtrl->GetNextItem(hCurrent,TVGN_NEXT);
	}
	int C=this->Comm.GetCount();
	for(int i=0;i<C;i++)
	{
		
		CHackClient* hc=Comm.GetAt(i);
		if(hc!=NULL)
		{
			if(hc->HostIP==IP)
			{
				delete hc;
				Comm[i]=NULL;
			}
		}
	}
}

void CHackProDlg::OnTvnSelchangedMtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM si=MTree.GetSelectedItem();  //Get the selected field;
	DWORD_PTR d = MTree.GetItemData(si);
	Hold.ActivatePage(d);
	
	
	/*int IntVal=d/MAXCOUNT;
	switch(d%MAXCOUNT)
	{
		case 0://about page
			Hold.ActivatePage(0); 
			OutStatus("Get all Hacking Utility in HackPro");
			break;
		case 1://Mac address
			Hold.ActivatePage(Hold.GetIndex(&(Comm.GetAt(IntVal)->Page_MacAddr)));
			OutStatus("List the MAC Address of Computers in your NetWork");
			break;
		case 2://portScan single port
			Hold.ActivatePage(Hold.GetIndex(&(Comm.GetAt(IntVal)->Page_PortScan)));
			OutStatus("Scan the ports opened at remote computer (Single Port)");
			//Page_PortScan.ShowWindow(SW_SHOW); 
			break;
		case 3:
			Hold.ActivatePage(Hold.GetIndex(&(Comm.GetAt(IntVal)->Page_PortScanSip)));
			OutStatus("Scan the ports opened at remote computer (Single Computer)");
			//Page_PortScan.ShowWindow(SW_SHOW); 
			break;
		case 9://Regconfig
			Hold.ActivatePage(Hold.GetIndex(&(Comm.GetAt(IntVal)->Page_RegConfig)));
			OutStatus("Edit the Windows Registry in new way");
			break;
		case 5://smurf attack
			Hold.ActivatePage(Hold.GetIndex(&(Comm.GetAt(IntVal)->Page_Smurf)));
			OutStatus("Attack (Denial of Service) on Remote system");
			break;
		case 6://Raw Packet Sender
			Hold.ActivatePage(Hold.GetIndex(&(Comm.GetAt(IntVal)->Page_RawPacket)));
			OutStatus("Send a tcp/ip Raw Packet");
			break;
		case 7://password hash extractar
			Hold.ActivatePage(Hold.GetIndex(&(Comm.GetAt(IntVal)->Page_PwdHash)));
			OutStatus("Extract System Password Hash from SAM DataBase and Compare it with your guess");
			break;
		case 8://Windows list
			Hold.ActivatePage(Hold.GetIndex(&(Comm.GetAt(IntVal)->Page_WndList)));
			OutStatus("Get List of all Openned Windows and their handles");
			break;
		case 4://System information
			Hold.ActivatePage(Hold.GetIndex(&(Comm.GetAt(IntVal)->Page_SysInfo)));
			OutStatus("Get List of all Openned Windows and their handles");
			break;
		case 10://System Control
			Hold.ActivatePage(Hold.GetIndex(&(Comm.GetAt(IntVal)->Page_SysCtrl)));
			OutStatus("Get full Control on remote system");
			break;
		case 11://remote descktop
			Hold.ActivatePage(Hold.GetIndex(&(Comm.GetAt(IntVal)->Page_RemoteDesktop)));
			OutStatus("Get the GUI of remote system");
			break;
		default:
			OutStatus("");
	}*/
	//m_hold.ShowWindow(SW_SHOW);
	MTree.SetFocus();
//============================================================
	*pResult = 0;	 
}

void CHackProDlg::OnNMClickMtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	//HTREEITEM si=MTree.GetSelectedItem();  //Get the selected field;
	//DWORD_PTR d = MTree.GetItemData(si);//
	*pResult = 0;
}
void CHackProDlg::AddTreeItem(CString Path,LPARAM lParam)
{
	//TOKENIZING THE Path STRING
	CArray<CString,CString> str;
    CString resToken;
	int curPos= 0;
	resToken= Path.Tokenize("\\",curPos);
	while (resToken != "")
	{
		str.Add(resToken);
	    resToken= Path.Tokenize("\\",curPos);
	};
	
	//GET THE ROOT ITEM
	HTREEITEM TreeItem;
	TreeItem=this->MTree.GetRootItem();

	//if not found exit
	if(TreeItem==NULL)
	{
		return;	
	}
	
	//tree first member found	
	//Go through the all the members and add only if they are not present 
	CString temp;
	//TreeItem=MTree.GetNextItem(TreeItem,TVGN_NEXT);
	for(int i=0;i<str.GetCount();i++)
	{
		HTREEITEM Parent=TreeItem;
		CString strtemp=str.GetAt(i);
		CString temptreeitem;
		TreeItem=MTree.GetChildItem(TreeItem);
		temptreeitem=MTree.GetItemText(TreeItem);
		while((strtemp!=temptreeitem)&&(TreeItem!=NULL))
		{
			TreeItem=MTree.GetNextItem(TreeItem,TVGN_NEXT);
			temptreeitem=MTree.GetItemText(TreeItem);
		}
		if((TreeItem==NULL))
		{
			if(i!=0)
			{
				TreeItem=MTree.InsertItem(strtemp,IndexDefault,IndexDefault,Parent);
				MTree.SetItemImage(TreeItem,IndexDefault,IndexDefault);
				MTree.SetItemData(TreeItem,lParam);
			}
			else
			{
				TreeItem=MTree.InsertItem(strtemp,INDEX,INDEX,Parent);
				MTree.SetItemImage(TreeItem,INDEX,INDEX);
				MTree.SetItemData(TreeItem,lParam);
			}
		}
		
	}
	
}
void CHackProDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	//
}

void CHackProDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	PostQuitMessage(0);
}

void CHackProDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	//CDialog::OnOK();
}
void CHackProDlg::OutStatus(CString s)
{
	SetDlgItemText(IDC_MSTATUS,s);
}
void CHackProDlg::OnFileConnect()
{
	m_InputIp.DoModal(); 
	CString ip;
	m_InputIp.GetIp(ip);
	this->AddHost(ip.LockBuffer()); 
}

void CHackProDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	int Count=(int)Comm.GetCount();
	for(int i=0;i<Count;i++)
	{
		CHackClient* pComm=(CHackClient*)Comm.GetAt(i);
		if(pComm!=NULL)
		{
			pComm->Close();
		}
	}
	CDialog::PostNcDestroy();
}

void CHackProDlg::OnMenu()
{
	this->EndDialog(0);
}



void CHackProDlg::OnHelpAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CHackProDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
LONG CHackProDlg::OnTrayNotify(WPARAM wparam,LPARAM lparam)
{
	switch(lparam)
	{
		case WM_RBUTTONDOWN:
		{
			CMenu *menu;
			CMenu *popup;
			int count;
			menu=this->GetMenu();
			popup=menu->GetSubMenu(2);
			count = popup->GetMenuItemCount();
			POINT pt ;
			GetCursorPos (&pt) ;
			SetForegroundWindow();
			popup->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON,pt.x, pt.y, AfxGetMainWnd());
			break ;
		}
		case WM_LBUTTONDBLCLK:
		{
			Shell_NotifyIcon(NIM_DELETE,&NotifyData);
			this->ShowWindow(SW_SHOW);
		}
	}
	return 0;
}

void CHackProDlg::OnWindowShowmainwindow()
{
	Shell_NotifyIcon(NIM_DELETE,&NotifyData);
	this->ShowWindow(SW_SHOW);
}

void CHackProDlg::OnWindowAbout()
{
	Shell_NotifyIcon(NIM_ADD,&NotifyData);
	this->ShowWindow(SW_HIDE);
}
