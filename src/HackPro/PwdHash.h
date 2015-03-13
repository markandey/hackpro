#ifndef PWDHASH_H
#define PWDHASH_H

#include "afxcmn.h"
#include "afxwin.h"
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "HackProdlg.h"
// CPwdHash dialog
//===========================
/*#define DUMP_PIPE_SIZE 1024

typedef HINSTANCE (WINAPI *pLoadLib_t) (CHAR *);
typedef HINSTANCE (WINAPI *pGetProcAddr_t) (HINSTANCE, CHAR *);
typedef HINSTANCE (WINAPI *pFreeLib_t) (HINSTANCE);
typedef int (*pDumpSam_t) (CHAR *);

typedef struct _remote_info {
    pLoadLib_t      pLoadLibrary;
    pGetProcAddr_t pGetProcAddress;
    pFreeLib_t     pFreeLibrary;
    CHAR  szDllName[MAX_PATH+1];
    CHAR  szProcName[MAX_PATH+1];
    CHAR  szPipeName[MAX_PATH+1];
} REMOTE_INFO;


*/

//============================

class CPwdHash : public CPropertyPage
{
	DECLARE_DYNAMIC(CPwdHash)

public:
	CPwdHash();
	virtual ~CPwdHash();
	void ExtactHash();

// Dialog Data
	enum { IDD = IDD_PWD_HASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_HashList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedGethash();
	afx_msg void OnBnClickedExtrhash();
	// textbox for password given by auother
	CEdit m_txtpwd;
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};
#endif
