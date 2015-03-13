// Tip.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "Tip.h"


// Tip dialog

IMPLEMENT_DYNAMIC(Tip, CDialog)
Tip::Tip(CString Message,CString Caption,CWnd* pParent /*=NULL*/)
	: CDialog(Tip::IDD, pParent)
{
	m_msg=Message;
	m_Cap=Caption; 
}
BOOL Tip::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	m_Caption.SetWindowText(m_Cap);  
	m_Message.SetWindowText(m_msg);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

Tip::~Tip()
{
}

void Tip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAPTION, m_Caption);
	DDX_Control(pDX, IDC_MESSAGE, m_Message);
}


BEGIN_MESSAGE_MAP(Tip, CDialog)
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// Tip message handlers

void Tip::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	this->EndDialog(1);
	CDialog::OnMButtonDown(nFlags, point);
	 
}

void Tip::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	this->EndDialog(1);

	CDialog::OnLButtonDown(nFlags, point);
}
