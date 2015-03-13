
#include "stdafx.h"
#include "hackpro.h"
#include "WndTree.h"
#include "HackClient.h"
CWndTree::CWndTree(void)
{

}
CWndTree::CWndTree(CHackClient* Parent)
{
	this->Papa = Parent;
	this->Root=NULL;
	Papa->FT[2]=CWndTree::CallBack_WndFillInfo;
	Papa->FT[3]=CWndTree::CallBack_WndFn; 
}

int CWndTree::FillInfo()
{
	this->Root = new WNode;
	Root->Hwnd =NULL;
	Root->Parent =NULL;
	strcpy(Root->Text ,"");
	return this->MyEnumWnd(NULL,Root); //enumerates from remote system
	//EnumWindows(CWndTree::EnumWindowsProc,(LPARAM)Root); 
	
}
int CWndTree::InsertInfo(PWNode pnode)
{
	
	//EnumChildWindows(pnode->Hwnd, CWndTree::EnumWindowsProc,(LPARAM)pnode);
	return this->MyEnumWnd(pnode->Hwnd,pnode);
}
void CWndTree::mSendMessage(HWND hWnd,UINT Msg, WPARAM wParam,LPARAM lParam)
{
	SendMessage(hWnd,Msg,wParam,lParam);
}
CWndTree::~CWndTree(void)
{
	this->FreeMem(this->Root);  
}
void* CWndTree::CallBack_WndFillInfo(void* InData,int size)
{
	if(size<sizeof(MyEnumArgv))
	{
		goto EXIT;
	}
	int count =(size-sizeof(MyEnumArgv)) / sizeof(WndItem);
	MyEnumArgv* argv =(MyEnumArgv*)InData;
	 
	WNode *node=(WNode*)argv->Parent;
	CWndTree* This =(CWndTree*)argv->This ;
	WndItem* arr =(WndItem*)(((char*)InData)+sizeof(MyEnumArgv));
	for(int i=0;i<count;i++)
	{
		WNode *nnode =new WNode;
		nnode->Hwnd =arr[i].Hwnd;
		nnode->Parent =(WNode*)argv->Parent;
		strcpy(nnode->Text ,arr[i].Text);
		strcpy(nnode->ClassName,arr[i].ClassName);
		nnode->WndInfo =arr[i] .WndInfo;
		node->Children.Add(nnode);
	}
EXIT:
	HANDLE HEnumWindows = OpenEvent(EVENT_ALL_ACCESS,FALSE,"HACK_EnumWindows");
	SetEvent(HEnumWindows);
	CloseHandle(HEnumWindows);
	int *retsize = new int[2];
	retsize[0]=0;
	retsize[1]=0;
	return (void*)retsize;
}
int CWndTree::MyEnumWnd(HWND hWnd,WNode* parent)
{
	MyEnumArgv argv;
	argv.Hwnd =(unsigned long)hWnd;
	argv.This=(unsigned long)this;
	argv.Parent =(unsigned long)parent;
	Papa->SendRequest(2,2,sizeof(argv),(void*)&argv);
	HANDLE HEnumWindows=OpenEvent(EVENT_ALL_ACCESS,FALSE,"HACK_EnumWindows");
	if(HEnumWindows==NULL)
	{
		HEnumWindows=CreateEvent(NULL,FALSE,FALSE,"HACK_EnumWindows");
	}
	
	DWORD ret =WaitForSingleObject(HEnumWindows,2500);
	if(ret ==WAIT_OBJECT_0)
	{
		return 1;
	}
	else
	{
		Papa->Error("Error During Enumerating Window List\nCode: event named \"HACK_EnumWindows\"",GetLastError());
	}
	return 0;
}
void* CWndTree::CallBack_WndFn(void* InData,int size)
{
	MessageBox(NULL,"Completed Successfully","Info(HackPro)",MB_OK);
	int *retsize = new int[2];
	retsize[0]=0;
	retsize[1]=0;
	return (void*)retsize;
}
void CWndTree::WndFunction(HWND hwnd,char* str,int fn)
{
	WndFunctionArgv args;
	args.Hwnd =(DWORD)hwnd;
	strcpy(args.str ,str);
	args.fn =fn;
	Papa->SendRequest(3,3,sizeof(args),(void*)&args );
	return;
}
void CWndTree::FreeMem(WNode* node)
{
	if(node!=NULL)
	{
		int count=node->Children.GetCount();
		for(int i=0;i<count;i++)
		{
			FreeMem( node->Children.GetAt(i));  
		}
		delete node;
	}
}