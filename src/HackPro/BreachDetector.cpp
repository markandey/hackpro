// BreachDetector.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "BreachDetector.h"


// CBreachDetector dialog

IMPLEMENT_DYNAMIC(CBreachDetector, CDialog)
CBreachDetector::CBreachDetector(CWnd* pParent /*=NULL*/)
	: CDialog(CBreachDetector::IDD, pParent),m_Gps(&m_List,&m_btScan,&m_Out)
{
}

CBreachDetector::~CBreachDetector()
{
}

void CBreachDetector::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OUT3, m_Out);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_EDIT1, m_EdtDes);
	DDX_Control(pDX, IDC_SCANNOW, m_btScan);
}


BEGIN_MESSAGE_MAP(CBreachDetector, CDialog)
	ON_BN_CLICKED(IDC_SCANNOW, OnBnClickedScannow)
END_MESSAGE_MAP()


// CBreachDetector message handlers
BOOL CBreachDetector::OnInitDialog()
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

void CBreachDetector::OnBnClickedScannow()
{
	unsigned short a[]={7,9,13,17,19,80,135,139,443,445,1025,1026,1027,1031};
	m_Gps.PortScan("127.0.0.1",a,14);
}


