// PwdHash.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "PwdHash.h"
#include "globalfn.h"
#include "cmdout.h"
#include "hacker.h"
//#import "C:\WINDOWS\Microsoft.NET\Framework\v1.0.3705\Mscorlib.tlb" raw_interfaces_only
//#using <Mscorlib.dll> 
//using namespace System::Security::Cryptography;

// CPwdHash dialog

IMPLEMENT_DYNAMIC(CPwdHash, CPropertyPage)
CPwdHash::CPwdHash()
	: CPropertyPage(CPwdHash::IDD)
{

}

CPwdHash::~CPwdHash()
{
}

void CPwdHash::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTHASH, m_HashList);
	DDX_Control(pDX, IDC_PASSTEXT, m_txtpwd);
	//this->m_txtpwd.GetWindow();  
}


BEGIN_MESSAGE_MAP(CPwdHash, CPropertyPage)
	ON_BN_CLICKED(IDC_GETHASH, OnBnClickedGethash)
	ON_BN_CLICKED(IDC_EXTRHASH, OnBnClickedExtrhash)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// CPwdHash message handlers

BOOL CPwdHash::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
//==========================
	this->m_HashList.InsertColumn(0,"User Name",LVCFMT_LEFT,100);
	this->m_HashList.InsertColumn(1,"User ID",LVCFMT_LEFT,100);
	this->m_HashList.InsertColumn(2,"LM Hash",LVCFMT_LEFT,100);
	this->m_HashList.InsertColumn(3,"NTLM Hash",LVCFMT_LEFT,100);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CPwdHash::OnBnClickedGethash()
{
	
}

void CPwdHash::OnBnClickedExtrhash()
{
	//=====================================
	ExtactHash();
	//=====================================
}

BOOL CPwdHash::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CPwdHash::ExtactHash()
{
	
	
}