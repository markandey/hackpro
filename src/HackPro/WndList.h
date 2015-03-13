#ifndef WNDLIST_H
#define WNDLIST_H
#include "wndtree.h"
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "HackProdlg.h"

//struct __WndNode;
//class CWndTree;
#include "WndTree.h"
class CWndList : public CPropertyPage
{
	DECLARE_DYNAMIC(CWndList)
	CWndTree* Data;
	HBITMAP bmpdisp;//Bitmap that stores the Image of Window with is selected

	void SetData(__WndNode* node);
	//char* RectToStr(RECT r,char* str);

public:
	CHackClient* Papa;
	CWndList();
	CWndList(CHackClient* parent);
	virtual ~CWndList();
	//
	static void* CALLBACK_CaptureWindow(void* data,int size); //call back for window capture
	void GetDisp(void* pBits,int width,int height);

// Dialog Data
	enum { IDD = IDD_WNDLSTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	static UINT Thread_Init(LPVOID lpParameter);
	static UINT Thread_Add(LPVOID lpParameter);
	virtual BOOL OnInitDialog();
	CTreeCtrl m_Tree;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnEnChangeFindtxt();
	afx_msg void OnBnClickedBtfindwindow();
};
#endif