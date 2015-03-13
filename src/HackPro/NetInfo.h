#ifndef NETINFO_H
#define NETINFO_H


// CNetInfo dialog

class CNetInfo : public CDialog
{
	DECLARE_DYNAMIC(CNetInfo)
	CString GetInfo();

public:
	static UINT ThrdGetInfo(LPVOID param);
	CNetInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNetInfo();

// Dialog Data
	enum { IDD = IDD_NETINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
#endif