// MacAddrDlg dialog
#ifndef MACADDRDLG_H
#define MACADDRDLG_H
#include "afxcmn.h"
#include "afxwin.h"
#include "stdafx.h"
#include "resource.h"

class MacAddrDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(MacAddrDlg)

public:
	MacAddrDlg();
	virtual ~MacAddrDlg();
	static UINT THRD(LPVOID param);//thred

// Dialog Data
	enum { IDD = IDD_MACADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedGButget();
	virtual BOOL OnInitDialog();
	CIPAddressCtrl TextIp1;
	CIPAddressCtrl TextIp2;
	CIPAddressCtrl TextIp3;
	CEdit TextMac;
	CListCtrl m_List;
	afx_msg void OnBnClickedSButget();
	afx_msg void OnEnChangeSMac();
	afx_msg void OnIpnFieldchangedGIpaddress2(NMHDR *pNMHDR, LRESULT *pResult);
};
#endif