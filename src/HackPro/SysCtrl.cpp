// SysCtrl.cpp : implementation file
//

#include "StdAfx.h"
#include "SysCtrl.h"
#include "HackClient.h"
#include "HackPro.h"


// SysCtrl dialog
class CHackClient;
IMPLEMENT_DYNAMIC(SysCtrl, CDialog)
SysCtrl::SysCtrl(CHackClient* Papa)
	: CDialog(SysCtrl::IDD, NULL)
{
	this->Parent=Papa;
	Papa->FT[CALLBACK_SYSCTRL]=SysCtrl::CALLBACK_SysCtrl;
}

SysCtrl::~SysCtrl()
{

}

void SysCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LSTPROCESS, m_PList);
	DDX_Control(pDX, IDC_CMBPRTY, m_ListPri);
	DDX_Control(pDX, IDC_COMBO1, m_listOp);
	DDX_Control(pDX, IDC_COMBO2, m_listMode);
}
BEGIN_MESSAGE_MAP(SysCtrl, CDialog)
	ON_BN_CLICKED(IDC_BTCLEAR, OnBnClickedBtclear)
	ON_BN_CLICKED(IDC_REFRESH, OnBnClickedRefresh)
	ON_BN_CLICKED(IDC_BTENDTASK, OnBnClickedBtendtask)
	ON_BN_CLICKED(IDC_BTSETPRTY, OnBnClickedBtsetprty)
	ON_BN_CLICKED(IDC_BTEXE, OnBnClickedBtexe)
	ON_BN_CLICKED(IDC_BTSHUTDOWN, OnBnClickedBtshutdown)
	ON_BN_CLICKED(IDC_BTLOGOFF, OnBnClickedBtlogoff)
	ON_BN_CLICKED(IDC_BTLOCK, OnBnClickedBtlock)
	ON_BN_CLICKED(IDC_BTRESTART, OnBnClickedBtrestart)
END_MESSAGE_MAP()


// SysCtrl message handlers

BOOL SysCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->m_PList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	Init_Plist();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void SysCtrl::Init_Plist()
{
	
	//MessageBox("Hiiiiiiiiiiiiiiii");
	//CHackProDlg* MainWnd=(CHackProDlg*)AfxGetMainWnd();
	//CImageList* img=&(MainWnd->ImgLst);
	//this->m_PList.SetImageList(img,LVSIL_NORMAL);
	m_PList.SetSelectionMark(2);
	m_ListPri.SetCurSel(1);
	this->m_PList.InsertColumn(0,"Process",LVCFMT_LEFT,100);
	this->m_PList.InsertColumn(1,"Process ID",LVCFMT_LEFT,100);
	this->m_PList.InsertColumn(2,"Threads",LVCFMT_LEFT,100);
	this->m_PList.InsertColumn(3,"Priority",LVCFMT_LEFT,100);
	
	
	this->m_ListPri.InsertString(0,"High");
	this->m_ListPri.SetItemData(0,13);
	this->m_ListPri.InsertString(1,"Normal");
	this->m_ListPri.SetItemData(1,8);
	this->m_ListPri.InsertString(2,"Low");
	this->m_ListPri.SetItemData(2,5);
	this->m_ListPri.SetCurSel(0);
	
	this->m_listOp.InsertString(0,"edit");
	this->m_listOp.InsertString(1,"explore");
	this->m_listOp.InsertString(2,"open");
	this->m_listOp.SetCurSel(0);
	
	this->m_listMode.InsertString(0,"Hidden");
	this->m_listMode.SetItemData(0,SW_HIDE );
	this->m_listMode.InsertString(1,"Maximized");
	this->m_listMode.SetItemData(1,SW_MAXIMIZE  );
	this->m_listMode.InsertString(2,"Minimized");
	this->m_listMode.SetItemData(2,SW_MINIMIZE );
	this->m_listMode.SetCurSel(0);
	
	SysCtrlReq Req;
	Req.Cmd=10;
	Req.This=(unsigned int)this;
	Parent->SendRequest(5,5,sizeof(SysCtrlReq),&Req);

	//5 (LOW);8 (Normal);13 (High);
}
void* SysCtrl::CALLBACK_SysCtrl(void* data,int size)
{
	SysCtrlInfo* inf=(SysCtrlInfo*)data;
	SysCtrl* This=(SysCtrl*) inf->This;
	if(inf->isSucceed)
	{
		This->ProcList.RemoveAll();
		This->m_PList.DeleteAllItems();
		ProcInfo temp;
		ProcInfo* Ptr=(ProcInfo*)((char*)data+sizeof(SysCtrlInfo));
		for(int i=0;i<inf->lenPinfo;i++)
		{
			memcpy(&temp,Ptr,sizeof(temp));
			Ptr++;
			This->AddProc(temp);
		}
		
	}
	else
	{
		::MessageBox(NULL,"Some Error has been Occured","Eroor",MB_OK);
	}
	int* RetData =new int[2];
	RetData[0]=0;
	RetData[1]=0;
	return (void* )RetData;
}
void SysCtrl::AddProc(ProcInfo inf)
{
	char Pri[30];
	//strcpy(Pri[ABOVE_NORMAL_PRIORITY_CLASS],"Above Normal");
	//strcpy(Pri[BELOW_NORMAL_PRIORITY_CLASS],"Below Normal");
	//strcpy(Pri[IDLE_PRIORITY_CLASS],"Idle");
	strcpy(Pri," ");
	if(inf.Pri>0)
	{
		strcpy(Pri," Low");
	}
	if(inf.Pri>7)
	{
		strcpy(Pri," Normal");
	}
	if(inf.Pri>10)
	{
		strcpy(Pri," High");
	}
	char tempStr[30];
	int index=this->ProcList.Add(inf);//this will be parameter int particular list item
	   
	this->m_PList.InsertItem(0,inf.szExe,5);
	this->m_PList.SetItemData(0,index);
		
	itoa(inf.Pid,tempStr,10);
	this->m_PList.SetItemText(0,1,tempStr);
		
	itoa(inf.ThreadCount,tempStr,10);
	this->m_PList.SetItemText(0,2,tempStr);
	
	itoa(inf.Pri,tempStr,10);
	strcat(tempStr,Pri);
	this->m_PList.SetItemText(0,3,tempStr);
	
	
	//CString s;
	//s.Format("Proc:%s,PID=%d,Thread=%d,pri=%d",inf.szExe,inf.Pid,inf.ThreadCount,inf.Pri);
	return ;


}
void SysCtrl::OnBnClickedBtclear()
{
	SetDlgItemText(IDC_TXTFILE,"");
	//SetDlgItemText(IDC_TXTFILE1,"");
	SetDlgItemText(IDC_TXTFILE2,"");
	SetDlgItemText(IDC_TXTFILE3,"");
}

void SysCtrl::OnBnClickedRefresh()
{
	SysCtrlReq Req;
	Req.Cmd=10;
	Req.This=(unsigned int)this;
	Parent->SendRequest(5,5,sizeof(SysCtrlReq),&Req);
}

void SysCtrl::OnBnClickedBtendtask()
{
	int mark=m_PList.GetSelectionMark();
	if(mark==-1)
	{
		MessageBox("Select any process");
		return;
	}
	int index=this->m_PList.GetItemData(mark);
	if(index>=ProcList.GetCount() || index<0)
	{
		MessageBox("Error");
		return;
	}
	ProcInfo inf=this->ProcList.GetAt(index);
	CString s;
	index=m_ListPri.GetItemData(m_ListPri.GetCurSel());
	SysCtrlReq Req;
	Req.Cmd=SYS_TERMINATE;
	Req.Pid=inf.Pid;
	Req.pri=index;
	Req.This=(unsigned long)this;
	Parent->SendRequest(5,5,sizeof(SysCtrlReq),&Req);
	Req.Cmd=10;
	Parent->SendRequest(5,5,sizeof(SysCtrlReq),&Req);
	//s.Format("Proc:%s,PID=%d,Thread=%d,pri=%d,ToSet=%d",inf.szExe,inf.Pid,inf.ThreadCount,inf.Pri,index);
	//MessageBox(s);
	return;
	//return;
}

void SysCtrl::OnBnClickedBtsetprty()
{
	int mark=m_PList.GetSelectionMark();
	if(mark==-1)
	{
		MessageBox("Select any process");
		return;
	}
	int index=this->m_PList.GetItemData(mark);
	if(index>=ProcList.GetCount() || index<0)
	{
		MessageBox("Error");
		return;
	}
	ProcInfo inf=this->ProcList.GetAt(index);
	CString s;
	index=m_ListPri.GetItemData(m_ListPri.GetCurSel());
	SysCtrlReq Req;
	Req.Pid=inf.Pid;
	Req.pri=index;
	Req.Cmd=SYS_SETPRIORITY;
	Parent->SendRequest(5,5,sizeof(SysCtrlReq),&Req);
	//s.Format("Proc:%s,PID=%d,Thread=%d,pri=%d,ToSet=%d",inf.szExe,inf.Pid,inf.ThreadCount,inf.Pri,index);
	//MessageBox(s);
	return;
}

void SysCtrl::OnBnClickedBtexe()
{
	CString Str,open,path,dir,arg;
	m_listOp.GetLBText(m_listOp.GetCurSel(),open);
	int nShowCmd = m_listMode.GetItemData(m_listMode.GetCurSel());
	GetDlgItemText(IDC_TXTFILE,path);
	GetDlgItemText(IDC_TXTFILE2,arg);
	GetDlgItemText(IDC_TXTFILE3,dir);

	SysCtrlReq Req;
	Req.Cmd=SYS_EXEC;
	Req.This=(unsigned int)this;
	strcpy(Req.CmdLine,arg);
	strcpy(Req.verb,open);
	strcpy(Req.FileName,path);
	strcpy(Req.dir,dir);
	Req.CmdShow=nShowCmd;
	Parent->SendRequest(5,5,sizeof(SysCtrlReq),&Req);
	
	
	//Str.Format("Cmd:%s\nCmdShow:%d\n,Path:%s\n,Arg:%s\n,Dir:%s\n",open,nShowCmd,path,arg,dir);
	//MessageBox(Str);
}

void SysCtrl::OnBnClickedBtshutdown()
{
	SysCtrlReq Req;
	Req.Cmd=SYS_SHUTDOWN;
	Req.This=(unsigned int)this;
	Parent->SendRequest(5,5,sizeof(SysCtrlReq),&Req);

}

void SysCtrl::OnBnClickedBtlogoff()
{
	SysCtrlReq Req;
	Req.Cmd=SYS_LOGOFF;
	Req.This=(unsigned int)this;
	Parent->SendRequest(5,5,sizeof(SysCtrlReq),&Req);
}

void SysCtrl::OnBnClickedBtlock()
{
	SysCtrlReq Req;
	Req.Cmd=SYS_LOCK;
	Req.This=(unsigned int)this;
	Parent->SendRequest(5,5,sizeof(SysCtrlReq),&Req);
}

void SysCtrl::OnBnClickedBtrestart()
{
	SysCtrlReq Req;
	Req.Cmd=SYS_RESTART;
	Req.This=(unsigned int)this;
	Parent->SendRequest(5,5,sizeof(SysCtrlReq),&Req);
}
