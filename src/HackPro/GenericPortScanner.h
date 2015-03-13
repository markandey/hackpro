#ifndef GENERICPORTSCANNER_H
#define GENERICPORTSCANNER_H

#include "papahead.h"
#include "globalfn.h"
#include "stdafx.h"

class GenericPortScanner
{
	
public:
	CListCtrl* m_OutPutList;
	CButton* m_btScan;
	CStatic* m_out;
	int ThrdCount;
	GenericPortScanner(CListCtrl*,CButton*,CStatic* out);
	~GenericPortScanner(void);
	void PortScan(CString IP,unsigned short Port1,unsigned short Port2);
	void PortScan(CString IP1,CString IP2,unsigned short Port1);
	void PortScan(CString IP,unsigned short* Port1,int no);
	CString GetDescription(int port);
	CString IncrIP(CString IP);
	int IPValCmp(CString IP1,CString IP2);
	static UINT TryOnPort(LPVOID pParam);
	static UINT Start(LPVOID pParam);
	
};
#endif
