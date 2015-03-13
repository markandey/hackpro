// HackServer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "Hacksock.h"
// CHackServerApp:
// See HackServer.cpp for the implementation of this class
//
#define DAWN_DEBUG true
void Error(CString msg,int error=0); //For Error Reporting


class CHackServerApp : public CWinApp
{
public:
	CHackServerApp();
	CHackSock Sock;
// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CHackServerApp theApp;