#ifndef __SYSCTRL_H
#define __SYSCTRL_H

#include "afxcmn.h"
#include "papahead.h"
#include "resource.h"
#include "afxwin.h"



// SysCtrl dialog

class SysCtrl : public CDialog
{
	DECLARE_DYNAMIC(SysCtrl)

public:
	SysCtrl(CHackClient* Papa);   // standard constructor
    virtual ~SysCtrl();
	CHackClient* Parent;
	CArray<ProcInfo,ProcInfo> ProcList;

// Dialog Data
	enum { IDD = IDD_DLGSYSCONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_PList;
	static void* CALLBACK_SysCtrl(void* data,int size);
	virtual BOOL OnInitDialog();
	void AddProc(ProcInfo inf);
	void Init_Plist();
	afx_msg void OnBnClickedBtclear();
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedBtendtask();
	afx_msg void OnBnClickedBtsetprty();
	CComboBox m_ListPri;
	CComboBox m_listOp;
	CComboBox m_listMode;
	afx_msg void OnBnClickedBtexe();
	afx_msg void OnBnClickedBtshutdown();
	afx_msg void OnBnClickedBtlogoff();
	afx_msg void OnBnClickedBtlock();
	afx_msg void OnBnClickedBtrestart();
};
#endif //SYSCTRL_H
