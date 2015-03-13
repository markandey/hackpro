#ifndef GLOBALFN_H
#define GLOBALFN_H

#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "HackProdlg.h"

//----------------------------------
#include "hackpro.h"
#include "stdafx.h"
//
// Finds the pid of LSASS.EXE
//
int find_pid (DWORD *ppid);
//----------------------------------
__int32 GetIpVal(BYTE f1,BYTE f2,BYTE f3,BYTE f4);
void IncrIp(BYTE *f1,BYTE *f2,BYTE *f3,BYTE *f4);

#endif


