
#ifndef SYSINFO_H
#define SYSINFO_H
#include "papahead.h"
#include "afxwin.h"
// SysInfo dialog

class SysInfo : public CDialog
{
	DECLARE_DYNAMIC(SysInfo)

public:
	SysInfo(CHackClient* pParent = NULL);   // standard constructor
	virtual ~SysInfo();
	CHackClient* Papa;
// Dialog Data
	enum { IDD = IDD_DLGSYSINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	static UINT Thread_GetSysInfo(LPVOID lpParameter);
	static void* CALLBACK_SysInfo(void* data,int size);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedGetinfo();
	CButton Btgetinfo;
};
#endif //SYSINFO_H