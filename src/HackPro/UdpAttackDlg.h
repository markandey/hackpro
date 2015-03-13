#ifndef UDPATTACKDLG_H
#define UDPATTACKDLG_H
#include "afxcmn.h"


// CUdpAttackDlg dialog

class CUdpAttackDlg : public CDialog
{
	DECLARE_DYNAMIC(CUdpAttackDlg)

public:
	CUdpAttackDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUdpAttackDlg();

// Dialog Data
	enum { IDD = IDD_UDPFLOOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	static UINT SndThrd(LPVOID Param);
	CIPAddressCtrl M_Vip;
	CIPAddressCtrl m_IP1;
	CIPAddressCtrl m_IP2;
	CProgressCtrl m_Progress;
	afx_msg void OnBnClickedBtstart();
};
#endif