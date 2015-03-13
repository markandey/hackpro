#pragma once
#include "afxwin.h"
#include "resource.h"


// BalloonTip dialog

class BalloonTip : public CDialog
{
	DECLARE_DYNAMIC(BalloonTip)
	CString m_msg;
	CString m_Cap;
	/*CStatic m_Message;
	CStatic m_Caption;*/
public:
	BalloonTip(CString Message,CString Caption,CWnd* pParent = NULL);   // standard constructor
	virtual ~BalloonTip();

// Dialog Data
	enum { IDD = IDD_TIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CStatic m_Message;
};
