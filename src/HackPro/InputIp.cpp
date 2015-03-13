// InputIp.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "InputIp.h"


// InputIp dialog

IMPLEMENT_DYNAMIC(InputIp, CDialog)
InputIp::InputIp(CWnd* pParent /*=NULL*/)
	: CDialog(InputIp::IDD, pParent)
{
}

InputIp::~InputIp()
{
}

void InputIp::GetIp(CString& ip)
{
	ip=IP;
}

void InputIp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, IpText);
}


BEGIN_MESSAGE_MAP(InputIp, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// InputIp message handlers

void InputIp::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	BYTE fld[4];
    IpText.GetAddress(fld[0],fld[1],fld[2],fld[3]);
	IP.Format("%ld.%ld.%ld.%ld",fld[0],fld[1],fld[2],fld[3]);
	OnOK();
}

void InputIp::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
