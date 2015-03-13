#ifndef CREVENGINEER_H
#define CREVENGINEER_H
#include "afxwin.h"


// CRevEngineer dialog

class CRevEngineer : public CDialog
{
	DECLARE_DYNAMIC(CRevEngineer)
private:
	void LounchProcess();
	int GetProcessList();
public:
	CRevEngineer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRevEngineer();

// Dialog Data
	enum { IDD = IDD_REVENGG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBrws();
	afx_msg void OnBnClickedBtexecute();
	CEdit m_EdtModPath;
	CEdit m_EdtCmdLine;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRnew();
	CStatic m_PaneNew;
	CStatic m_PaneOld;
	CButton m_btBrowseExe;
	CButton m_btExe;
	CComboBox m_CmbProcList;
	afx_msg void OnBnClickedRrun();
	CWnd m_RadOld;
	CButton m_Refresh;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedBtrefresh();
	afx_msg void OnBnClickedBrws2();
	afx_msg void OnBnClickedTerminate();
	afx_msg void OnBnClickedButton1();
};
#endif //CREVENGINEER_H
