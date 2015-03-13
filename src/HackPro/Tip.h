#ifndef TIP_H
#define TIP_H

#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "HackProdlg.h"

// Tip dialog

class Tip : public CDialog
{
	DECLARE_DYNAMIC(Tip)
	CString m_msg;
	CString m_Cap;
	CStatic m_Message;
	CStatic m_Caption;

public:
	Tip(CString Message,CString Caption,CWnd* pParent = NULL);// standard constructor
	BOOL OnInitDialog(); 
	virtual ~Tip();

// Dialog Data
	enum { IDD = IDD_TIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
#endif