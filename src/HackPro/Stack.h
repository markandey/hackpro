

#ifndef __STACK_H
#define __STACK_H
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "HackProdlg.h"
class CStack
{
	CPtrArray m_PtrArray;
	INT_PTR Count;
public:
	CStack();
	~CStack();
	// the functions
	void Push(void* pData);
	bool Pop(void** pRetData);
};
#endif