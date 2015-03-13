#ifndef BREACHDETECTOR_H
#define BREACHDETECTOR_H
#include "afxwin.h"
#include "GenericPortScanner.h"
#include "afxcmn.h"


// CBreachDetector dialog

class CBreachDetector : public CDialog
{
	DECLARE_DYNAMIC(CBreachDetector)
	GenericPortScanner m_Gps;


public:
	CBreachDetector(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBreachDetector();

// Dialog Data
	enum { IDD = IDD_VSCAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedScannow();
	CStatic m_Out;
	CListCtrl m_List;
	CEdit m_EdtDes;
	CButton m_btScan;
	virtual BOOL OnInitDialog();
};
#endif //BREACHDETECTOR_H