// PortScanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "PortScanDlg.h"
#include "GenericPortscanner.h"
#include "Globalfn.h"
// CPortScanDlg dialog

IMPLEMENT_DYNAMIC(CPortScanDlg, CDialog)
CPortScanDlg::CPortScanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPortScanDlg::IDD, pParent),gsp(&m_List,&m_btScan,&m_Out)
{

}

CPortScanDlg::~CPortScanDlg()
{
}

void CPortScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IP1);
	DDX_Control(pDX, IDC_IPADDRESS2, m_IP2);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_SCANNOW, m_btScan);
	DDX_Control(pDX, IDC_OUT1, m_Out);
}


BEGIN_MESSAGE_MAP(CPortScanDlg, CDialog)
	
	ON_BN_CLICKED(IDC_SCANNOW, OnBnClickedScannow)
END_MESSAGE_MAP()


// CPortScanDlg message handlers



void CPortScanDlg::OnBnClickedScannow()
{
	int port=GetDlgItemInt(IDC_EDTPORT);
	CString IP1,IP2;
	BYTE b1,b2,b3,b4;
	this->m_IP1.GetAddress(b1,b2,b3,b4);
	IP1.Format("%d.%d.%d.%d",b1,b2,b3,b4);
	this->m_IP2.GetAddress(b1,b2,b3,b4);
	IP2.Format("%d.%d.%d.%d",b1,b2,b3,b4);

	gsp.PortScan(IP1,IP2,port);

}

BOOL CPortScanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT ); 
	m_List.InsertColumn(0,"IP",LVCFMT_LEFT,100);
	m_List.InsertColumn(1,"Port",LVCFMT_LEFT,50);
	m_List.InsertColumn(2,"Status",LVCFMT_LEFT,80);
	m_List.InsertColumn(3,"Description",LVCFMT_LEFT,200);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
