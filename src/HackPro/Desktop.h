#ifndef DESKTOP_H
#define DESKTOP_H
#include "papahead.h"



class CDesktop : public CDialog
{
	DECLARE_DYNAMIC(CDesktop)
private:
	HBITMAP bmpdisp;
	int isRefeshing;

public:
	CDesktop(CHackClient* Papa);   // standard constructor
	virtual ~CDesktop();
	CHackClient* Parent;
// Dialog Data
	enum { IDD = IDD_DESKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	static void* CALLBACK_SetDesktop(void* data,int size); //call back for setting desktop;
	static void* CALLBACK_InputMsg(void* data,int size); //call back for MessageInput has been send;
	void SendRemoteMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	int PaintDisp(HDC srcdc,HDC distdc);
	int PaintDisp();
	void GetDisp(void* pBits,int width,int height);
	int CovAbsX(int x);
	int CovAbsY(int y);
	UINT_PTR m_nTimer;
	
	afx_msg void OnNMThemeChangedVscroll(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CStatic m_Disp;
	
	
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	
};
#endif