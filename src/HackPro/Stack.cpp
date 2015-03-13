#include "stdafx.h"
#include "hackpro.h"
#include "stack.h"
CStack::CStack(): Count(-1)
{
}

CStack::~CStack()
{
}

void CStack::Push(void* pData)
{
	this->m_PtrArray.Add(pData);
	this->Count++;
}

bool CStack::Pop(void** pData)
{
	if(Count==-1)
	{
		*pData=NULL;
		return(false);
	}
	else
	{
		*pData=m_PtrArray.GetAt(Count--);
		m_PtrArray.RemoveAt(Count+1);
		return(true);
	}	
}