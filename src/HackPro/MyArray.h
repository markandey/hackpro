#pragma once
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "HackProdlg.h"
template <class T>
class MyArray
{
	T* arr;
	int c;
public:
	int Add(T i)
	{
		if (c==0)
		{
			arr=new T;
			*arr =i;
			c=1;
			return 0;
		}
		else
		{
			T* temp=arr;
			arr= new T[c+1];
			for(int k =0;k<c;k++)
			{
				arr[k] = temp[k];
			}
			arr[k+1]=i;
			c++;
			delete temp;
			return k+1;
		}
	}
	T GetAt(int i)
	{
		if(i>c)
		{
			T garbage;
			return garbage;
		}
		else
		{
			return arr[i];
		}
	}
	int GetCount()
	{
		return c;
	}
	MyArray(void)
	{
		this->arr=NULL;
		this->c =0;
	}
	~MyArray(void)
	{
		delete[] T;
	}
};
