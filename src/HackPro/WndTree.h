#ifndef WNDTREE_H
#define WNDTREE_H
#include "papahead.h"
//#include "Winuser.h"
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "HackProdlg.h"


class CWndTree
{
	CHackClient* Papa;
public:
	WNode *Root;
	int InsertInfo(WNode* pnode);
	int FillInfo();
	void mSendMessage(HWND hWnd,UINT Msg, WPARAM wParam,LPARAM lParam);
	int MyEnumWnd(HWND hWnd,WNode* parent);
	void WndFunction(HWND hwnd,char* str,int fn);
	void FreeMem(WNode* node);
	static void* CallBack_WndFillInfo(void* InData,int size); 
	static void* CallBack_WndFn(void* InData,int size);
	CWndTree(void);
	CWndTree(CHackClient* Parent);
    ~CWndTree(void);
};
#endif