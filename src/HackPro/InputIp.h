#ifndef INPUTIP_H
#define INPUTIP_H

#include "afxcmn.h"
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "HackProdlg.h"
// InputIp dialog

class InputIp : public CDialog
{
	DECLARE_DYNAMIC(InputIp)
public:
	void GetIp(CString& ip);
	InputIp(CWnd* pParent = NULL);   // standard constructor
	virtual ~InputIp();

// Dialog Data
	enum { IDD = IDD_GETIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl IpText;
	CString IP;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
#endif