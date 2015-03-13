#ifndef PORTSCANSIP_H
#define PORTSCANSIP_H


#include "afxcmn.h"
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "HackProdlg.h"
#include "GenericPortScanner.h"
#include "afxwin.h"

// PortScanSip dialog

class PortScanSip : public CPropertyPage
{
	DECLARE_DYNAMIC(PortScanSip)

public:
	PortScanSip();
	GenericPortScanner ps;
	/*Declare GenericScannerObject in Such a way that is object remains undeleted utill Scanning not finished*/
	virtual ~PortScanSip();

// Dialog Data
	enum { IDD = IDD_PORTSCANSIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// the list for the result
	CListCtrl m_List;
	afx_msg void OnBnClickedButscan();
	afx_msg void OnEnChangeEdtport();
	CIPAddressCtrl m_IP;
	CButton m_btScan;
	CStatic m_Out;
};
#endif