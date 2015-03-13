// NetStat.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "NetStat.h"


// CNetStat dialog

IMPLEMENT_DYNAMIC(CNetStat, CDialog)
CNetStat::CNetStat(CWnd* pParent /*=NULL*/)
	: CDialog(CNetStat::IDD, pParent)
{
	m_pThrd=NULL;
}

CNetStat::~CNetStat()
{
	if(m_pThrd!=NULL)
	{
		m_pThrd->SuspendThread();
	}
}

void CNetStat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST5, m_ListIPStat);
	DDX_Control(pDX, IDC_LIST1, m_ListTCPStat);
	DDX_Control(pDX, IDC_LIST3, m_ListICMPStat);
	DDX_Control(pDX, IDC_LIST2, m_ListUDPStat);
}


BEGIN_MESSAGE_MAP(CNetStat, CDialog)
END_MESSAGE_MAP()




BOOL CNetStat::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ListIPStat.SetExtendedStyle(LVS_EX_FULLROWSELECT ); 
	this->m_ListIPStat.InsertColumn(0,"Property",LVCFMT_LEFT,200);
	this->m_ListIPStat.InsertColumn(1,"Value",LVCFMT_LEFT,100);

	m_ListTCPStat.SetExtendedStyle(LVS_EX_FULLROWSELECT ); 
	this->m_ListTCPStat.InsertColumn(0,"Property",LVCFMT_LEFT,200);
	this->m_ListTCPStat.InsertColumn(1,"Value",LVCFMT_LEFT,100);

	m_ListUDPStat.SetExtendedStyle(LVS_EX_FULLROWSELECT );
	this->m_ListUDPStat.InsertColumn(0,"Property",LVCFMT_LEFT,200);
    this->m_ListUDPStat.InsertColumn(1,"Value",LVCFMT_LEFT,100);

	m_ListICMPStat.SetExtendedStyle(LVS_EX_FULLROWSELECT );
	this->m_ListICMPStat.InsertColumn(0,"Property",LVCFMT_LEFT,200);
	this->m_ListICMPStat.InsertColumn(1,"Value",LVCFMT_LEFT,100);

	CWinThread* m_pThrd=AfxBeginThread(CNetStat::ThrdUPDate,this);
		
	return TRUE;  
}
void CNetStat::GetIPStat()
{
	MIB_IPSTATS Stats;
	int Index;
	GetIpStatistics(&Stats);
	Index=m_ListIPStat.InsertItem(0,"IP Forwording");
	if(Stats.dwForwarding==0)
	{
		m_ListIPStat.SetItemText(Index,1,"Disabled");
	}
	else
	{
		m_ListIPStat.SetItemText(Index,1,"Enabled");
	}

	Index=m_ListIPStat.InsertItem(1,"Default TTL");
	CString strTtl;
	strTtl.Format("%ld",Stats.dwDefaultTTL);
	m_ListIPStat.SetItemText(Index,1,strTtl);

	Index=m_ListIPStat.InsertItem(2,"Datagrams Received");
	CString strDgrmRcv;
	strDgrmRcv.Format("%ld",Stats.dwInReceives);
	m_ListIPStat.SetItemText(Index,1,strDgrmRcv);
	
	Index=m_ListIPStat.InsertItem(3,"Datagrams Received With Hdr Error");
	CString strDgrmRcvErr;
	strDgrmRcvErr.Format("%ld",Stats.dwInHdrErrors);
	m_ListIPStat.SetItemText(Index,1,strDgrmRcvErr);

	Index=m_ListIPStat.InsertItem(4,"Datagrams Received With Address Errors");
	CString strDgrmRcvAddrErr;
	strDgrmRcvAddrErr.Format("%ld",Stats.dwInAddrErrors);
	m_ListIPStat.SetItemText(Index,1,strDgrmRcvAddrErr);

	Index=m_ListIPStat.InsertItem(5,"Datagrams Forwarded");
	CString strDgrmFwd;
	strDgrmFwd.Format("%ld",Stats.dwForwDatagrams);
	m_ListIPStat.SetItemText(Index,1,strDgrmFwd);

	Index=m_ListIPStat.InsertItem(6,"Datagrams with Unknown Protocol");
	CString strDgrmUProto;
	strDgrmUProto.Format("%ld",Stats.dwInUnknownProtos);
	m_ListIPStat.SetItemText(Index,1,strDgrmUProto);

	Index=m_ListIPStat.InsertItem(7,"Received Datagrams Discarded");
	CString strDgrmRcvDescarded;
	strDgrmRcvDescarded.Format("%ld",Stats.dwInDiscards);
	m_ListIPStat.SetItemText(Index,1,strDgrmRcvDescarded);

	
	Index=m_ListIPStat.InsertItem(8,"Received Datagrams Delivered");
	CString strInDelivers;
	strInDelivers.Format("%ld",Stats.dwInDelivers);
	m_ListIPStat.SetItemText(Index,1,strInDelivers);

	Index=m_ListIPStat.InsertItem(9,"Outgoing datagrams that IP is requested to transmit");
	CString strOutRequests ;
	strOutRequests.Format("%ld",Stats.dwOutRequests);
	m_ListIPStat.SetItemText(Index,1,strOutRequests);

	Index=m_ListIPStat.InsertItem(10,"Routed datagrams discarded");
	CString strRoutingDiscards;
	strRoutingDiscards.Format("%ld",Stats.dwRoutingDiscards);
	m_ListIPStat.SetItemText(Index,1,strRoutingDiscards);

	
	Index=m_ListIPStat.InsertItem(11,"Sent datagrams discarded");
	CString strOutDiscards;
	strOutDiscards.Format("%ld",Stats.dwOutDiscards);
	m_ListIPStat.SetItemText(Index,1,strOutDiscards);

	Index=m_ListIPStat.InsertItem(12,"datagrams for which no route exists");
	CString strOutNoRoutes;
	strOutNoRoutes.Format("%ld",Stats.dwOutNoRoutes);
	m_ListIPStat.SetItemText(Index,1,strOutNoRoutes);

	Index=m_ListIPStat.InsertItem(13,"datagrams for which all frags did not arrive");
	CString strReasmTimeout;
	strReasmTimeout.Format("%ld",Stats.dwReasmTimeout);
	m_ListIPStat.SetItemText(Index,1,strReasmTimeout);


	Index=m_ListIPStat.InsertItem(14,"datagrams requiring reassembly");
	CString strReasmReqds;
	strReasmReqds.Format("%ld",Stats.dwReasmReqds);
	m_ListIPStat.SetItemText(Index,1,strReasmReqds);

	Index=m_ListIPStat.InsertItem(15,"successful reassemblies");
	CString strReasmOks;
	strReasmOks.Format("%ld",Stats.dwReasmOks);
	m_ListIPStat.SetItemText(Index,1,strReasmOks);

	Index=m_ListIPStat.InsertItem(16,"failed reassemblies");
	CString strReasmFails;
	strReasmFails.Format("%ld",Stats.dwReasmFails);
	m_ListIPStat.SetItemText(Index,1,strReasmFails);

	Index=m_ListIPStat.InsertItem(17,"successful fragmentations");
	CString strFragOks;
	strFragOks.Format("%ld",Stats.dwFragOks);
    m_ListIPStat.SetItemText(Index,1,strFragOks);

	Index=m_ListIPStat.InsertItem(18,"failed fragmentations");
	CString strFragFails;
	strFragFails.Format("%ld",Stats.dwFragFails);
	m_ListIPStat.SetItemText(Index,1,strFragFails);

	Index=m_ListIPStat.InsertItem(19,"datagrams fragmented");
	CString strFragCreates;
	strFragCreates.Format("%ld",Stats.dwFragCreates);
	m_ListIPStat.SetItemText(Index,1,strFragCreates);

	Index=m_ListIPStat.InsertItem(20,"number of routes in routing table");
	CString strNumRoutes;
	strNumRoutes.Format("%ld",Stats.dwNumRoutes);
	m_ListIPStat.SetItemText(Index,1,strNumRoutes);

}

void CNetStat::GetTCPStat()
{
	CString Des[]=
	{
		"retransmission time-out algorithm",
		"minimum time-out",
		"maximum time-out",
		"maximum connections",
		"active opens",
		"passive opens",
		"failed attempts",
		"established connections reset",
		"established connections",
		"segments received",
		"segment sent",
		"segments retransmitted",
		"incoming errors",
		"outgoing resets",
		"cumulative connections"
	};
	int Index;
	MIB_TCPSTATS Stats;
	GetTcpStatistics(&Stats);

	Index=m_ListTCPStat.InsertItem(0,"time-out algorithm");
	switch(Stats.dwRtoAlgorithm)
	{
		case MIB_TCP_RTO_CONSTANT:
			m_ListTCPStat.SetItemText(Index,1,"Constant Time-out");
			break;
		case MIB_TCP_RTO_RSRE:
			m_ListTCPStat.SetItemText(Index,1,"MIL-STD-1778 Appendix B");
			break;
		case MIB_TCP_RTO_VANJ:
			m_ListTCPStat.SetItemText(Index,1,"Van Jacobson's Algorithm");
			break;
		default:
			m_ListTCPStat.SetItemText(Index,1,"Other");
			break;
	}
	for(int i=1;i<15;i++)
	{
		Index=m_ListTCPStat.InsertItem(i,Des[i]);
		CString val;
		val.Format("%ld",(DWORD)*(((DWORD*)(&Stats))+i));
		m_ListTCPStat.SetItemText(Index,1,val);

	}

	

}
void CNetStat::GetUDPStat()
{
	CString Des[]=
	{
		"received datagrams",
		"datagrams for which no port exists",
		"errors on received datagrams",
		"sent datagrams",
		"entries in UDP listener table"
	};
	int Index;
	MIB_UDPSTATS Stats;
	GetUdpStatistics(&Stats);
	for(int i=0;i<5;i++)
	{
		Index=m_ListUDPStat.InsertItem(i,Des[i]);
		CString val;
		val.Format("%ld",(DWORD)*(((DWORD*)(&Stats))+i));
		m_ListUDPStat.SetItemText(Index,1,val);
	}
}
void CNetStat::GetICMPStat()
{
	int Index;
	MIB_ICMP Stats;
	GetIcmpStatistics(&Stats);
	CString Des[]=
	{
		"number of messages",
		"number of errors",
		"destination unreachable messages",
		"time-to-live exceeded messages",
		"parameter problem messages",
		"source quench messages",
		"redirection messages",
		"echo requests",
		"echo replies",
		"time-stamp requests",
		"time-stamp replies",
		"address mask requests",
		"address mask replies"
	};
	for(int i=0;i<13;i++)
	{
		Index=m_ListICMPStat.InsertItem(i,Des[i]+"(incoming)");
		CString val;
		val.Format("%ld",(DWORD)*(((DWORD*)(&(Stats.stats.icmpInStats)))+i));
		m_ListICMPStat.SetItemText(Index,1,val);
	}
	for(int i=0;i<13;i++)
	{
		Index=m_ListICMPStat.InsertItem(i+13,Des[i]+"(outgoing)");
		CString val;
		val.Format("%ld",(DWORD)*(((DWORD*)(&(Stats.stats.icmpOutStats)))+i));
		m_ListICMPStat.SetItemText(Index,1,val);
	}

	
}
UINT CNetStat::ThrdUPDate(LPVOID param)
{
	CNetStat* This=(CNetStat*)param;
	This->GetIPStat();
	This->GetTCPStat();
	This->GetUDPStat();
	This->GetICMPStat();
	while(1)
	{
		
		MIB_ICMP ICMPStats;
		MIB_UDPSTATS UDPStats;
		MIB_TCPSTATS TCPStats;
		MIB_IPSTATS IPStats;
		GetIcmpStatistics(&ICMPStats);
		GetUdpStatistics(&UDPStats);
		GetTcpStatistics(&TCPStats);
		GetIpStatistics(&IPStats);
		if(This==NULL)
		{
			return 1;
		}
		for(int i=0;i<13;i++)
		{
			CString val;
			val.Format("%ld",(DWORD)*(((DWORD*)(&(ICMPStats.stats.icmpInStats)))+i));
			This->m_ListICMPStat.SetItemText(i,1,val);
		}
		for(int i=0;i<13;i++)
		{
			CString val;
			val.Format("%ld",(DWORD)*(((DWORD*)(&(ICMPStats.stats.icmpOutStats)))+i));
			This->m_ListICMPStat.SetItemText(i+5,1,val);
		}
		for(int i=0;i<5;i++)
		{
			CString val;
			val.Format("%ld",(DWORD)*(((DWORD*)(&(UDPStats)))+i));
			This->m_ListUDPStat.SetItemText(i,1,val);
		}
		for(int i=1;i<15;i++)
		{
			CString val;
			val.Format("%ld",(DWORD)*(((DWORD*)(&(TCPStats)))+i));
			This->m_ListTCPStat.SetItemText(i,1,val);
		}
		for(int i=1;i<21;i++)
		{
			CString val;
			val.Format("%ld",(DWORD)*(((DWORD*)(&(IPStats)))+i));
			This->m_ListIPStat.SetItemText(i,1,val);
		}
		
		::Sleep(500);
	}
	return true;
}
