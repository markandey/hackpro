// PortScanSip.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "PortScanSip.h"
#include "GenericPortScanner.h"


// PortScanSip dialog

IMPLEMENT_DYNAMIC(PortScanSip, CPropertyPage)
PortScanSip::PortScanSip()
	: CPropertyPage(PortScanSip::IDD),ps(&m_List,&m_btScan,&m_Out)
{

}

PortScanSip::~PortScanSip()
{
}

void PortScanSip::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IP);
	DDX_Control(pDX, IDC_BUTSCAN, m_btScan);
	DDX_Control(pDX, IDC_OUT2, m_Out);
}


BEGIN_MESSAGE_MAP(PortScanSip, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTSCAN, OnBnClickedButscan)
	ON_EN_CHANGE(IDC_EDTPORT, OnEnChangeEdtport)
END_MESSAGE_MAP()


// PortScanSip message handlers

BOOL PortScanSip::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT ); 
	m_List.InsertColumn(0,"IP",LVCFMT_LEFT,100);
	m_List.InsertColumn(1,"Port",LVCFMT_LEFT,50);
	m_List.InsertColumn(2,"Status",LVCFMT_LEFT,80);
	m_List.InsertColumn(3,"Description",LVCFMT_LEFT,200);
	  
/*
	LVITEM Item;
	CString temp;
	temp="Hello";
	Item.mask=(LVIF_TEXT|LVIF_PARAM);
	Item.iItem=0;
	Item.iSubItem=0;
	Item.pszText=temp.LockBuffer();
	temp.UnlockBuffer();
	m_List.InsertItem(&Item);

	m_List.SetItemText(0,1,"Its day time");
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void PortScanSip::OnBnClickedButscan()
{
	
	int port1=GetDlgItemInt(IDC_EDTPORT1);
	int port2=GetDlgItemInt(IDC_EDTPORT2);
	CString IP;
	BYTE b1,b2,b3,b4;
	this->m_IP.GetAddress(b1,b2,b3,b4);
	IP.Format("%d.%d.%d.%d",b1,b2,b3,b4);

	
	/*int index=m_List.GetItemCount();
	//Add IP
	index=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE,index,IP,NULL,NULL,0,4);*/

	ps.PortScan(IP,port1,port2);
	
}

void PortScanSip::OnEnChangeEdtport()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
