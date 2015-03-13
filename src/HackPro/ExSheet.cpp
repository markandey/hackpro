// ExSheet.cpp : implementation file
//

#include "stdafx.h"
#include "hackpro.h"
#include "ExSheet.h"

// CExSheet

IMPLEMENT_DYNAMIC(CExSheet, CWnd)
CExSheet::CExSheet()
{
	
}

CExSheet::~CExSheet()
{

}
BOOL CExSheet::Create(const RECT rt,CWnd* pParentWnd,UINT ID)
{
	return CWnd::Create(_T("STATIC"),NULL, WS_CHILD | WS_VISIBLE|WS_SIZEBOX |WS_DLGFRAME ,rt,pParentWnd,ID) ;
}
void CExSheet::AddPage(CString Path,CDialog* page,UINT TemplateID)
{
	BrowseObj bo;
	bo.page=page;
	bo.Path=Path;
	int index=List.Add(bo);

	CHackProDlg* MainWnd=(CHackProDlg*)AfxGetMainWnd ();
	MainWnd->AddTreeItem(Path,index);

	page->Create(TemplateID,this); 
	page->SetParent(this);
	if(List.GetCount()==1)
	{
		this->Active = 0;
		page->ShowWindow(SW_NORMAL);  
	}	
	
	RECT rt;
	this->GetWindowRect(&rt);
	ScreenToClient(&rt);
	page->MoveWindow(&rt); 
}
void CExSheet::ActivatePage(int index)
{
	(List.GetAt(Active)).page->ShowWindow(SW_HIDE);
	(List.GetAt(index)).page ->ShowWindow(SW_NORMAL);
	(List.GetAt(index)).page->BringWindowToTop();
	(List.GetAt(index)).page->SetFocus( );
	Active=index;
}
void CExSheet::RemovePage(int index)
{
	CWnd* dlg = (List.GetAt(index)).page;
	dlg->DestroyWindow();
	List.RemoveAt(index); 
}
int CExSheet::GetIndex(CDialog* page)
{
	bool found =false;
	for(int i=0;i<=List.GetCount();i++)
	{
		if(i==List.GetCount())
		{
			break;
		}
		if(page==(List.GetAt(i)).page)
		{
			return i;
		}
		
	}
	//MessageBox("Page not Found","Error");
	return -1;
}
BEGIN_MESSAGE_MAP(CExSheet, CWnd)
END_MESSAGE_MAP()



// CExSheet message handlers

void CExSheet::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	/*for(int i=0;i<List.GetCount();i++)
	{
		CWnd* dlg = List.GetAt(i);
		RECT rt= {lpwndpos->x,lpwndpos->y,lpwndpos->cx ,lpwndpos->cy};
		ClientToScreen(&rt);
		dlg->SetWindowPos(this,rt.left ,rt.top ,rt.right  ,rt.bottom,SWP_NOSIZE);  
	}*/
}

