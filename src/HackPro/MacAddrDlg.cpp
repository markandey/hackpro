// MacAddrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "MacAddrDlg.h"
#include "HackProdlg.h"
#include "globalfn.h"
//#include "globalfn.h"


// MacAddrDlg dialog

IMPLEMENT_DYNAMIC(MacAddrDlg, CPropertyPage)
MacAddrDlg::MacAddrDlg()
	: CPropertyPage(MacAddrDlg::IDD)
{

}

MacAddrDlg::~MacAddrDlg()
{
}

void MacAddrDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_G_IPADDRESS1, TextIp1);
	DDX_Control(pDX, IDC_G_IPADDRESS2, TextIp2);
	DDX_Control(pDX, IDC_G_IPADDRESS, TextIp3);
	DDX_Control(pDX, IDC_S_MAC, TextMac);
	DDX_Control(pDX, IDC_G_LIST, m_List);
}


BEGIN_MESSAGE_MAP(MacAddrDlg, CPropertyPage)
	
	ON_BN_CLICKED(IDC_G_BUTGET, OnBnClickedGButget)
	ON_BN_CLICKED(IDC_S_BUTGET, OnBnClickedSButget)
	ON_EN_CHANGE(IDC_S_MAC, OnEnChangeSMac)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_G_IPADDRESS2, OnIpnFieldchangedGIpaddress2)
	
END_MESSAGE_MAP()


// MacAddrDlg message handlers


void MacAddrDlg::OnBnClickedGButget()
{
	// TODO: Add your control notification handler code here
	AfxBeginThread(MacAddrDlg::THRD,this);
}

BOOL MacAddrDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();	
	//===============
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT ); 
	m_List.InsertColumn(0,"IP",LVCFMT_LEFT,100);
	m_List.InsertColumn(1,"MAC Address",LVCFMT_LEFT,200);
	CHackProDlg* Papa=(CHackProDlg*)::AfxGetApp()->m_pMainWnd;
	CImageList* temp=m_List.GetImageList(0);
	if(m_List.SetImageList(&(Papa->ImgLst),LVSIL_NORMAL)!=temp)
	{
		MessageBox("Could not set image list");
	}

	//m_List.InsertItem(0,"jjj",46);
	//int i=m_List.InsertItem(1,"172.116.111.126",80);
	//m_List.SetItemText(i,1,"hh:jj:dd:77:44:77"); 

 
	//===============
	return TRUE;  // return TRUE unless you set the focus to a control
	
}
UINT MacAddrDlg::THRD(LPVOID param)
{
	MacAddrDlg *ptr = (MacAddrDlg*) param;
	int index;
	ptr->m_List.DeleteAllItems();  
	BYTE Af1,Af2,Af3,Af4;
	BYTE Bf1,Bf2,Bf3,Bf4;
	//BYTE p,q,r,s;
    ptr->TextIp1.GetAddress(Af1,Af2,Af3,Af4);
	ptr->TextIp2.GetAddress(Bf1,Bf2,Bf3,Bf4);
	
    while((GetIpVal(Af1,Af2,Af3,Af4)-GetIpVal(Bf1,Bf2,Bf3,Bf4))<0)
	{
					CString ip;
					CString Tmp;
					ip.Format("%d.%d.%d.%d",Af1,Af2,Af3,Af4); 
					IPAddr ipaddr;
                    ipaddr=inet_addr(ip);
					ULONG PhAdd[2],Len=6;
					DWORD Error;
					int i=ptr->m_List.GetItemCount(); 
					int count=ptr->m_List.GetImageList(0)->GetImageCount();
					index=ptr->m_List.InsertItem(LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE,i,ip,NULL,NULL,0,4);
					//ptr->m_List.SetItemDataExtractIcon(AfxGetApp()->m_hInstance,"shell32.dll",46),0);
					Error=SendARP(ipaddr,0,PhAdd,&Len);
					if(Error==NO_ERROR)
					{
						PBYTE pbHexMac = (PBYTE) PhAdd;
						CString temp;
						CString PhyAddr;
						
						for(int i=0;i<(int)Len-1;i++)
						{
							temp.Format("%02X:",pbHexMac[i]);
							PhyAddr+=temp;
						}

						temp.Format("%02X",pbHexMac[i]);
						PhyAddr+=temp;
						ptr->m_List.SetItemText(index,1,PhyAddr); 
						temp.Empty();
					}
					else
					{
						ptr->m_List.SetItemText(index,1,"Not Found!"); 	
						
					}
					IncrIp(&Af1,&Af2,&Af3,&Af4);
	}
	return 0;
}

void MacAddrDlg::OnBnClickedSButget()
{
	char ip[16];
	GetDlgItemText(IDC_G_IPADDRESS,ip,15);
	HRESULT hr;
    IPAddr  ipAddr;
    ULONG   pulMac[2];
    ULONG   ulLen;

    ipAddr = inet_addr (ip);
    memset (pulMac, 0xff, sizeof (pulMac));
    ulLen = 6;
    
    hr = SendARP (ipAddr, 0, pulMac, &ulLen);
    //printf ("Return %08x, length %8d\n", hr, ulLen);
    if(hr==S_OK)
	{
		size_t i, j;
		char* szMac = new char[ulLen*3+1];
		PBYTE pbHexMac = (PBYTE) pulMac;

		//
		// Convert the binary MAC address into human-readable
		//
		for (i = 0, j = 0; i < ulLen - 1; ++i)
		{
			j += sprintf (szMac + j, "%02X:", pbHexMac[i]);
		}
	    
		sprintf (szMac + j, "%02X", pbHexMac[i]);
		TextMac.SetWindowText(szMac);
		delete[] szMac;	
	}
	else
	{
		TextMac.SetWindowText("Not Found!");
	}
}

void MacAddrDlg::OnEnChangeSMac()
{
	
}

void MacAddrDlg::OnIpnFieldchangedGIpaddress2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

