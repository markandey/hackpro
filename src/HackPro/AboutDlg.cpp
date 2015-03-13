// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "AboutDlg.h"
#include "HackProDlg.h"


// AboutDlg dialog

IMPLEMENT_DYNAMIC(AboutDlg, CDialog)
AboutDlg::AboutDlg()
	: CDialog(AboutDlg::IDD)
{
	m_pIndex=-1;
}

AboutDlg::~AboutDlg()
{
}

void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AboutDlg, CDialog)
	ON_STN_CLICKED(IDC_ABOUT, OnStnClickedAbout)
	ON_BN_CLICKED(IDC_BTCONNECT, OnBnClickedBtconnect)
	ON_BN_CLICKED(IDC_BTEXIT, OnBnClickedBtexit)
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTCONNECT, OnBnClickedBtconnect2)
END_MESSAGE_MAP()


// AboutDlg message handlers

void AboutDlg::OnStnClickedAbout()
{
	// TODO: Add your control notification handler code here
}

void AboutDlg::OnBnClickedBtconnect()
{
	CHackProDlg* mwnd =(CHackProDlg*)AfxGetMainWnd();
	mwnd->OnFileConnect();
}

void AboutDlg::OnBnClickedBtexit()
{
	CHackProDlg* mwnd =(CHackProDlg*)AfxGetMainWnd();
	mwnd->EndDialog(0);
	
}

void AboutDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	

	int w=105,h=38;
	int x=point.x,y=point.y;
	int c=-1,r=-1;

	if(x<(49+w) && x>49)
	{
		c=0;
		
	}
	else if(x<(215+w) && x>215)
	{
		c=1;
	}
	else if(x<(377+w) && x>377)
	{
		c=2;
	}
	
	if(y>99 && y<(99+h))
	{
		r=0;
	}
	else if(y>179 && y<(179+h))
	{
		r=1;
	}
	else if(y>261 && y<(261+h))
	{
		r=2;
	}
    if(x>213 && y>344 && x<318 && y<382)
	{
		r=2;
		c=2;
	}
	int Item=-1;
	if(r!=-1 && c!=-1)
	{
		Item=c*3+r;
		if(Item!=m_pIndex)
		{
			if(!(c==2 && r==2))
			{
				DispInformation(Item);
			}

		}

    }
	m_pIndex=Item;
	
	/*CString str;
	str.Format("(%d,%d) c=%d r=%d Item=%d",point.x,point.y,c,r,Item);
	SetDlgItemText(IDC_POINT,str);*/



	CDialog::OnMouseMove(nFlags, point);
}
void AboutDlg::DispInformation(int block)
{
	CString txtBlock="";
	CString Obj="";
	CString Tools="";
	::Beep(555*(block+1),100);
	
	switch(block)
	{
		case 0://FootPrinting
			 txtBlock="FootPrinting";
			 Obj="Target address range,namespace acquisition, and various information "
				 "gathering.";
			 Tools="MAC Address Tabulator,Packet Sniffer,Raw Packet Sender";
			 break;
		case 1://Scanning
			 txtBlock="Scanning";
			 Obj="Bulk target assessment, identification of Operating System and listening services";
			 Tools="Port Scanner SingleIP,PortScanner SinglePort,Raw Packet Sender";
			 break;
	    case 2://Enumeration
			 txtBlock="Enumeration";
			 Obj="In this step attacker lists the user accounts and poorly protected resource shares."
				 "";
			 Tools="None (Since highly specific to the attack)";
			 break;
		case 3://Gaining Access
			 txtBlock="Gaining Access";
			 Obj="Enough data that has been gathered at previous steps can be used for gaining access"
				 "";
			 Tools="None";
			 break;
		 case 4://Escalating privilege
			 txtBlock="Escalating Privilege";
			 Obj="If only user level access was obtained in last step, the attacker will now seek to gain "
				 "complete control of system";
			 Tools="System Control,Window List,Remote Desktop,SystemInfo";
			 break;
		 case 5:
			txtBlock="Pilfering";
			 Obj="Once more information gathering for more access gain"
				 "";
			 Tools="System Control,Window List,Remote Desktop";
			 break;
		 case 6:
			txtBlock="Covering Tracks";
			 Obj="Clear logs and hide tools"
				 "";
			 Tools="Window List,Remote Desktop";
			 break;
		 case 7:
			txtBlock="Creating Backdoors";
			 Obj="Creation of rogue user accounts,schedule batch jobs"
				 "";
			 Tools="Remote Desktop,Registry Editor";
			 break;
		 case 8:
			txtBlock="Denial of Service";
			 Obj="As a lost resort attacker tries to disable target by exploiting flaws"
				 "";
			 Tools="Smurf,SYN Flood,UDP Flood,Raw packet Sender";
			 break;
    }
	CString str;
	str.Format("\t  %s\nObjective: %s\n\nTools in HackPro: %s",txtBlock,Obj,Tools);
	if(Obj!="")
	{
		SetDlgItemText(IDC_DETAIL,str);
	}
	

	/*
	RECT Rect;
	(this->GetDlgItem(IDC_DETAIL))->GetWindowRect(&Rect);
	CWindowDC wdc(this->GetDlgItem(IDC_DETAIL));
	CBrush *cb;
	cb=new CBrush(RGB(255,0,0));
	wdc.FillRect(&Rect,cb);
	wdc.SetBkColor(0);
	wdc.SetDCBrushColor(RGB(255,0,0));
	wdc.SetDCPenColor(RGB(255,0,0));
	wdc.TextOut(1,1,str);*/


}

HBRUSH AboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// Call the base class implementation first! Otherwise, it may
   // undo what we're trying to accomplish here.
   HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

   // Are we painting the IDC_MYSTATIC control? We can use
   // CWnd::GetDlgCtrlID() to perform the most efficient test.
   if (pWnd->GetDlgCtrlID() == IDC_DETAIL)
   {
	   /*COLORREF kcolor[]={RGB(0,0,128),RGB(0,0,0),RGB(0,64,64)};
		int Rnd=rand();
		int i=Rnd%3;*/
		pDC->SetTextColor(RGB(255,255,0));
		pDC->SetBkColor(RGB(52,56,32));
			
		/*RECT Rect;
		pWnd->GetWindowRect(&Rect);
		pDC->FillSolidRect(Rect.left,Rect.top,Rect.right,Rect.bottom,RGB(0,0,0));*/
			
		hbr=CreateSolidBrush(RGB(52,56,32));//kcolor[i]);
	  
   }

   return hbr;
}

void AboutDlg::OnPaint()
{
	CPaintDC dc(this);

	
	
	// device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL AboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DispInformation(0);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void AboutDlg::OnBnClickedBtconnect2()
{
	CHackProDlg* mwnd =(CHackProDlg*)AfxGetMainWnd();
	mwnd->OnHelpAbout();
}
