#pragma once
#include "afxcmn.h"


// CConnectionTable dialog

class CConnectionTable : public CDialog
{
	DECLARE_DYNAMIC(CConnectionTable)

public:
	CConnectionTable(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConnectionTable();
	void GetTable();

// Dialog Data
	enum { IDD = IDD_CONNECTIONTABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRefreshtable();
};
