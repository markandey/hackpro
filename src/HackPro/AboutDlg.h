#ifndef ABOUTDLG_H
#define ABOUTDLG_H
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"


class AboutDlg : public CDialog
{
	DECLARE_DYNAMIC(AboutDlg)
	int m_pIndex;

public:
	AboutDlg();
	HBRUSH m_brush;
	virtual ~AboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	void DispInformation(int block);
public:
	afx_msg void OnStnClickedAbout();
	afx_msg void OnBnClickedBtconnect();
	afx_msg void OnBnClickedBtexit();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtconnect2();
};
#endif