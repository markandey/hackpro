// HackPro.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "HackPro_i.h"
#include "stdafx.h"
#include "resource.h"
#include "HackProdlg.h"

// CHackProApp:
// See HackPro.cpp for the implementation of this class
//


class CHackProApp : public CWinApp
{
public:
	CHackProApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	BOOL ExitInstance(void);
};

extern CHackProApp theApp;