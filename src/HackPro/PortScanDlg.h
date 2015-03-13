#ifndef PORTSCANNERDLG_H
#define PORTSCANNERDLG_H
#include "afxcmn.h"
#include "afxwin.h"
#include "GenericPortScanner.h"


// CPortScanDlg dialog

class CPortScanDlg : public CDialog
{
	DECLARE_DYNAMIC(CPortScanDlg)

public:
	CPortScanDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPortScanDlg();

// Dialog Data
	enum { IDD = IDD_PORTSCAN};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedScannow();
	GenericPortScanner gsp;
	CIPAddressCtrl m_IP1;
	CIPAddressCtrl m_IP2;
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	CButton m_btScan;
	CStatic m_Out;
};
#endif