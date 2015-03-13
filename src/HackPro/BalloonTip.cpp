// BalloonTip.cpp : implementation file
//

#include "stdafx.h"
//#include "RegConfigDlg.h"
#include "BalloonTip.h"
// BalloonTip dialog
IMPLEMENT_DYNAMIC(BalloonTip, CDialog)
BalloonTip::BalloonTip(CString Message,CString Caption,CWnd* pParent /*=NULL*/)
: CDialog(BalloonTip::IDD, pParent)
{
	m_msg=Message;
	m_Cap=Caption; 
	
}

BalloonTip::~BalloonTip()
{
}

void BalloonTip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAPTION, m_Caption);
	DDX_Control(pDX, IDC_MESSAGE, m_Message);
}


BEGIN_MESSAGE_MAP(BalloonTip, CDialog)
	
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()
// BalloonTip message handlers
BOOL BalloonTip::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	m_Caption.SetWindowText(m_Cap);  
	m_Message.SetWindowText(m_msg);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void BalloonTip::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);
	this->EndDialog(1); 
}
