#ifndef SMURFDLG_H
#define SMURFDLG_H

#include "afxcmn.h"
#include "afxwin.h"

#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "HackProdlg.h"

// CSmurfDlg dialog

class CSmurfDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CSmurfDlg)

public:
	CSmurfDlg();
	virtual ~CSmurfDlg();

// Dialog Data
	enum { IDD = IDD_DOSSMURF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	DWORD m_IP;
	int m_Delay;
	int m_NumOfPackets;
	CIPAddressCtrl m_VictimIP;
	CProgressCtrl m_Progress;
	static UINT SndThrd(LPVOID Param);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedStart();
};
#endif