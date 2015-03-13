#ifndef SYNATTACKDLG_H
#define SYNATTACKDLG_H
#include "afxcmn.h"
#include "afxwin.h"


// CSynAttackDlg dialog

class CSynAttackDlg : public CDialog
{
	DECLARE_DYNAMIC(CSynAttackDlg)

public:
	CSynAttackDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSynAttackDlg();

// Dialog Data
	enum { IDD = IDD_DOSSYN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	static UINT SndThrd(LPVOID Param);
    CIPAddressCtrl m_victimIP;
	CEdit m_victimsport;
	afx_msg void OnBnClickedStart();
	CIPAddressCtrl m_IP1;
	CIPAddressCtrl m_IP2;
	CEdit m_port;
	CEdit m_delay;
	CProgressCtrl m_Progress;
	CEdit m_Delay;
};
#endif