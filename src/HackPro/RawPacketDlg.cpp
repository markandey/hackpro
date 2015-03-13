// RawPacketDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "RawPacketDlg.h"


// CRawPacketDlg dialog

IMPLEMENT_DYNAMIC(CRawPacketDlg, CPropertyPage)
CRawPacketDlg::CRawPacketDlg()
	: CPropertyPage(CRawPacketDlg::IDD)
{
}

CRawPacketDlg::~CRawPacketDlg()
{
}

void CRawPacketDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPSRC, m_IPSrc);
	DDX_Control(pDX, IDC_IPDEST, m_IPDest);
	DDX_Control(pDX, IDC_PROTO, m_Protocol);
	DDX_Control(pDX, IDC_SRCPORT, m_srcport);
	DDX_Control(pDX, IDC_DESTPORT, m_destport);
	DDX_Control(pDX, IDC_DATASIZE, m_datasize);
	DDX_Control(pDX, IDC_LBLSRCPORT, m_lblsrcport);
	DDX_Control(pDX, IDC_LBLDESTPORT, m_lbldestport);
	DDX_Control(pDX, IDC_LBLDATASIZE, m_lbldatasize);
	DDX_Control(pDX, IDC_PGRS, m_progress);
}


BEGIN_MESSAGE_MAP(CRawPacketDlg, CPropertyPage)
	
	ON_EN_CHANGE(IDC_PORT, OnEnChangePort)
	ON_BN_CLICKED(IDC_SEND, OnBnClickedSend)
	ON_CBN_SELCHANGE(IDC_PROTO, OnCbnSelchangeProto)
END_MESSAGE_MAP()


// CRawPacketDlg message handlers

BOOL CRawPacketDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//TODO:  Add extra initialization here
	SetDlgItemInt(IDC_PORT,0,FALSE);
	SetDlgItemInt(IDC_TTL,128,FALSE);
	SetDlgItemInt(IDC_NUMOFPACKET,100,FALSE);
	SetDlgItemInt(IDC_SRCPORT,0,FALSE);
	SetDlgItemInt(IDC_DESTPORT,0,FALSE);
	SetDlgItemInt(IDC_DATASIZE,0,FALSE);
	SetDlgItemInt(IDC_DELAY,0,FALSE);

	/*CString info="Select Type of Packet to send.Different Parameters are---\n\n";
	info+="Source IP:-Packets will be send to this IP\n\n";
	info+="Source IP:-Packets will be spoofed with this IP\n\n";
	info+="Number of Packets:-This many packets will be send\n\n";
	info+="Source Port:-TCP/UDP packets will be send to this port\n\n";
	info+="Destination Port:-TCP/UDP packets will be spoofed with this port\n\n";
	info+="Data Size:-UDP packets will contain this as Length field\n\n\n\n";
	info+="NOTE:- Different kinds of denial of service Attacks can be achieved only by Sending Raw Packets\n\n";*/

	SetDlgItemText(IDC_INFO,"Send some custom build IP packet with specified source and destination address");
	
	m_Protocol.InsertString(0,"ICMP with ECHO");
	m_Protocol.InsertString(1,"ICMP with ECHOREPLY");
	m_Protocol.InsertString(2,"ICMP with DEST_UNREACH");
	m_Protocol.InsertString(3,"ICMP with SOURCE_QUENCH");
	m_Protocol.InsertString(4,"ICMP with REDIRECT");
	m_Protocol.InsertString(5,"ICMP with TIME_EXCEEDED");

	m_Protocol.InsertString(6,"TCP with SYN");
	m_Protocol.InsertString(7,"TCP with FYN");
	m_Protocol.InsertString(8,"TCP with RST");
	m_Protocol.InsertString(9,"TCP with ACK");
	m_Protocol.InsertString(10,"TCP with URG");
	m_Protocol.InsertString(11,"TCP with PSH");

	m_Protocol.InsertString(12,"UDP FLOOD");

	m_Protocol.SetCurSel(0);
	

	
	return TRUE;  
	
}

void CRawPacketDlg::OnEnChangePort()
{
	
}

void CRawPacketDlg::OnBnClickedSend()
{	
	int index=m_Protocol.GetCurSel();

	//initialize different fields from UI input
	
	RawParam* param=new RawParam;
	param->This=this;
	param->Delay=GetDlgItemInt(IDC_DELAY);
	
	DWORD dwDestIP,dwsrcIp;
	this->m_IPDest.GetAddress(dwDestIP);
	this->m_IPSrc.GetAddress(dwsrcIp);
	param->pktParam.source=dwsrcIp;
	param->pktParam.destination=dwDestIP;
	
	param->pktParam.port=(unsigned short)GetDlgItemInt(IDC_DESTPORT);
	param->pktParam.ttl=(unsigned char)GetDlgItemInt(IDC_TTL);
	param->count=GetDlgItemInt(IDC_NUMOFPACKET);
	param->srcPort=(unsigned short)GetDlgItemInt(IDC_SRCPORT);
	param->destPort=(unsigned short)GetDlgItemInt(IDC_DESTPORT);
	param->packetsize=(unsigned short)GetDlgItemInt(IDC_DATASIZE);
	//unsigned long scrIP=inet_addr(strSrcIP);
	//unsigned long destIP=inet_addr(strDestIP);
	//unsigned char ttl=(unsigned char)GetDlgItemInt(IDC_TTL);
	//unsigned int npackets=GetDlgItemInt(IDC_NUMOFPACKET);;
	//unsigned short srcport=(unsigned short)GetDlgItemInt(IDC_SRCPORT);
	//unsigned short destport=(unsigned short)GetDlgItemInt(IDC_DESTPORT);
	//unsigned short packetsize=(unsigned short)GetDlgItemInt(IDC_DATASIZE);
	if(index<=5)
	{
		param->Type=1;
	}
	else if(index <=11)
	{
		param->Type=2;
	}
	else
	{
		param->Type=3;
	}
	switch(index)
	{
		case 0:
			param->Flag=ICMP_ECHO;
			break;
		case 1:
			param->Flag=ICMP_ECHOREPLY;
			break;
		case 2:
			param->Flag=ICMP_TIME_EXCEEDED;
			break;
		case 3:
			param->Flag=ICMP_TIME_EXCEEDED;
			break;
		case 4:
			param->Flag=ICMP_TIME_EXCEEDED;
			break;
		case 5:
			param->Flag=ICMP_TIME_EXCEEDED;
			break;
		case 6:
			param->Flag=TCPFLAG_SYN;
			break;
		case 7:
			param->Flag=TCPFLAG_FYN;
			break;
		case 8:
			param->Flag=TCPFLAG_RST;
			break;
		case 9:
			param->Flag=TCPFLAG_ACK;
			break;
		case 10:
			param->Flag=TCPFLAG_URG;
			break;
		case 11:
			param->Flag=TCPFLAG_PSH;
			break;
		case 12:
			param->Flag=0;
			break;		
	} 
	AfxBeginThread(CRawPacketDlg::SndThrd,param);

}

void CRawPacketDlg::OnCbnSelchangeProto()
{
	int index=m_Protocol.GetCurSel();
	switch(index)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			m_srcport.ShowWindow(SW_HIDE);
			m_lblsrcport.ShowWindow(SW_HIDE);
			m_destport.ShowWindow(SW_HIDE);
			m_lbldestport.ShowWindow(SW_HIDE);
			m_datasize.ShowWindow(SW_HIDE);
			m_lbldatasize.ShowWindow(SW_HIDE);
			break;
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
			m_srcport.ShowWindow(SW_SHOW);
			m_lblsrcport.ShowWindow(SW_SHOW);
			m_destport.ShowWindow(SW_SHOW);
			m_lbldestport.ShowWindow(SW_SHOW);
			m_datasize.ShowWindow(SW_HIDE);
			m_lbldatasize.ShowWindow(SW_HIDE);
			break;
		case 12:
			m_srcport.ShowWindow(SW_SHOW);
			m_lblsrcport.ShowWindow(SW_SHOW);
			m_destport.ShowWindow(SW_SHOW);
			m_lbldestport.ShowWindow(SW_SHOW);
			m_datasize.ShowWindow(SW_SHOW);
			m_lbldatasize.ShowWindow(SW_SHOW);
			break;		
	}
}
UINT CRawPacketDlg::SndThrd(LPVOID Param)
{
	RawParam* RParam=(RawParam*)Param;
	RParam->This->m_progress.SetRange(0,100);
	RParam->This->m_progress.ShowWindow(SW_SHOW);
	CNetSocket sock;
	if(RParam->Type==1)
	{
		sock.InitSocket(IPPROTO_ICMP);
	}
	else if(RParam->Type==2)
	{
		sock.InitSocket(0);
	}
	else
	{
		sock.InitSocket(0);
	}
	sock.SetOpt();
	for(int i=0;i<RParam->count;i++)
	{
		int val=(i*100)/RParam->count;
		int sent=0;
		RParam->This->m_progress.SetPos(val);
		switch(RParam->Type)
		{
			case 1:
				sent=sock.SendICMPPacket(RParam->pktParam,RParam->Flag);
				break;
			case 2:
				sent=sock.SendTCPPacket(RParam->pktParam,RParam->destPort,RParam->Flag,RParam->srcPort);
				break;
			case 3:
				UDP_HEADER udphead;
				udphead.Length=RParam->packetsize;
				udphead.Checksum=0;
				udphead.DestinationPort=RParam->destPort;
				udphead.SourcePort=RParam->srcPort;
				udphead.Checksum=sock.checksum((unsigned short*)&udphead,sizeof(UDP_HEADER));
				sent=sock.SendUDPPacket(RParam->pktParam,udphead,NULL,0);
				break;

		}
		if(sent<0)
		{
			break;
		}
		Sleep(RParam->Delay);
	}
	sock.close();
	RParam->This->m_progress.ShowWindow(SW_HIDE);
	delete RParam;
	return 0;
}