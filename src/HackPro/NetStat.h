#ifndef NETSTAT_H
#define NETSTAT_H
#include "afxcmn.h"



// CNetStat dialog

class CNetStat : public CDialog
{
	DECLARE_DYNAMIC(CNetStat)

public:
	CNetStat(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNetStat();
	static UINT ThrdUPDate(LPVOID param);
	CWinThread* m_pThrd;
	void GetIPStat();
	void GetTCPStat();
	void GetUDPStat();
	void GetICMPStat();
// Dialog Data
	enum { IDD = IDD_NETSTAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListIPStat;
	CListCtrl m_ListTCPStat;
	CListCtrl m_ListICMPStat;
	CListCtrl m_ListUDPStat;
	virtual BOOL OnInitDialog();
};

#endif