#ifndef EXSHEET_H
#define EXSHEET_H
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
//#error compiler reaches in exsheet
// CExSheet

class CExSheet : public CWnd
{
	DECLARE_DYNAMIC(CExSheet)
	CArray<BrowseObj ,BrowseObj>List;
	int Active;

public:
	CExSheet();
	//void AddPage(CDialog* page,UINT TemplateID); Depricated
	void AddPage(CString Path,CDialog* page,UINT TemplateID);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	int GetIndex(CDialog* page);
	BOOL Create(const RECT rt,CWnd* pParentWnd,UINT ID);
	void RemovePage(int index);
	void ActivatePage(int index);
	virtual ~CExSheet();

protected:
	DECLARE_MESSAGE_MAP()
	
};

#endif
