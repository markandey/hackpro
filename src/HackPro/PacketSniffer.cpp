// PacketSniffer.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "PacketSniffer.h"


// PacketSniffer dialog

IMPLEMENT_DYNAMIC(PacketSniffer, CDialog)
PacketSniffer::PacketSniffer(CWnd* pParent /*=NULL*/)
	: CDialog(PacketSniffer::IDD, pParent)
{
	m_pThrd=NULL;
	m_Running=false;
	m_Sock==NULL;
}

PacketSniffer::~PacketSniffer()
{
	closesocket(this->m_Sock);
}

void PacketSniffer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LSTCAP, m_List);
	DDX_Control(pDX, IDC_IPLIST, m_IPList);
}


BEGIN_MESSAGE_MAP(PacketSniffer, CDialog)
	ON_EN_CHANGE(IDC_DATA, OnEnChangeData)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CLEAR, OnBnClickedClear)
END_MESSAGE_MAP()


BOOL PacketSniffer::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_List.SetExtendedStyle(m_List.GetExtendedStyle()|LVS_EX_FULLROWSELECT );
	m_List.InsertColumn(0,"Src IP",LVCFMT_LEFT,100);
	m_List.InsertColumn(1,"Dest IP",LVCFMT_LEFT,100);
	m_List.InsertColumn(2,"Src Port",LVCFMT_LEFT,80);
	m_List.InsertColumn(3,"Dest Port",LVCFMT_LEFT,80);
	m_List.InsertColumn(4,"Protocol",LVCFMT_LEFT,80);
	m_List.InsertColumn(5,"ttl",LVCFMT_LEFT,30);
	m_List.InsertColumn(6,"Size",LVCFMT_LEFT,50);
	m_List.InsertColumn(7,"TCP Flag",LVCFMT_LEFT,100);
	m_List.InsertColumn(8,"ICMP Type",LVCFMT_LEFT,80);
	m_List.InsertColumn(9,"TCP Sequence",LVCFMT_LEFT,150);
	/*m_List.InsertColumn(10,"",LVCFMT_LEFT,200);
	m_List.InsertColumn(11,"",LVCFMT_LEFT,200);*/
	ListInterfaces();
	InitSock();
	
	if(this->m_isInit==1)
	{
		this->m_pThrd=AfxBeginThread(PacketSniffer::ThrdPacketCapture,this);
		m_Running=true;;
	}
	return TRUE;  
	
}
void PacketSniffer::ListInterfaces()
{
	char* buffer=NULL;
	ULONG Size=0;
	bool isSel=false;
	DWORD err=GetAdaptersInfo((PIP_ADAPTER_INFO)buffer,&Size);
	if(err==ERROR_BUFFER_OVERFLOW)
	{
		buffer=new char[Size];
		err=GetAdaptersInfo((PIP_ADAPTER_INFO)buffer,&Size);
		if(err==ERROR_SUCCESS)
		{
			PIP_ADAPTER_INFO pInfo=(PIP_ADAPTER_INFO)buffer;
			while(pInfo!=NULL)
			{
				//Get Assosiated IP Addresses
				CString strIPAddr=pInfo->IpAddressList.IpAddress.String;
                int i=this->m_IPList.AddString(strIPAddr);
				if(!isSel)
				{
					this->m_IPList.SetCurSel(i);
				}
				pInfo=pInfo->Next;
			}

		}
	}
}
void PacketSniffer::InitSock()
{
	
	if(this->m_Sock!=NULL)
	{
		closesocket(this->m_Sock);
	}
	this->m_isInit=1;
	//Get the raw socket
	this->m_Sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (m_Sock == INVALID_SOCKET)
	{
		this->m_isInit=0;
	}

	//Intitialize the Address
	sockaddr_in SockAddr;
	memset(&SockAddr, 0, sizeof(SockAddr));
	char IP[20];
	this->m_IPList.GetLBText(this->m_IPList.GetCurSel(),IP);
	SockAddr.sin_addr.s_addr = inet_addr(IP);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = 0;

	if (bind(m_Sock, (sockaddr *)&SockAddr, sizeof(SockAddr)) == SOCKET_ERROR)
	{
		this->m_isInit=0;
	}
	
	//set the socket to recieve all packets
	DWORD BytesReturned;
	int I;
	if (WSAIoctl(m_Sock, SIO_RCVALL, &I, sizeof(I), NULL, NULL, &BytesReturned, NULL, NULL) == SOCKET_ERROR)
	{
		this->m_isInit=0;
		MessageBox("WSAIoctl Failed\nUnable to initialize PacketCapture");
	}
	
}
UINT PacketSniffer::ThrdPacketCapture(LPVOID param)
{
	PacketSniffer* This=(PacketSniffer*)param;
	char *RecvBuffer = new char[HACK_MAXPACKETSIZE];
	int BytesRecv, FromLen;
	struct sockaddr_in From;

	if (RecvBuffer == NULL)
	{
		::MessageBox(This->m_hWnd,"out of memory","Error",0);
		return 0;
	}

	FromLen = sizeof(From);

	do
	{
		if(This==NULL)
		{
			return true;
		}
		memset(RecvBuffer, 0, HACK_MAXPACKETSIZE);	// initialize the buffer
		memset(&From, 0, sizeof(From));				//set reciev address to zero

		BytesRecv = recvfrom(This->m_Sock, RecvBuffer, HACK_MAXPACKETSIZE, 0, (sockaddr *)&From, &FromLen);
		
		if (BytesRecv > 0)
		{
			This->AnalyzePacket(RecvBuffer, BytesRecv);
		}
		
		
	} while (BytesRecv > 0);


	delete RecvBuffer;
	return true;
}
void PacketSniffer::AnalyzePacket(char* RecvBuffer,int size)
{
	CString data="";
	for(int i=0;i<size;i++)
	{
		if(RecvBuffer[i]>20 && RecvBuffer[i]<128)
		{
			data.AppendChar(RecvBuffer[i]);
		}
	}
	CString exData;
	this->GetDlgItemText(IDC_DATA,exData);
	exData=exData+"||||"+data;
	this->SetDlgItemText(IDC_DATA,exData.Right(100));

	IP_HEADER* iphdr=(IP_HEADER*)RecvBuffer;
	int iphdrlen=(iphdr->ihl)*4; //since header length is given in no of words and word=4 Bytes
	in_addr a;
	//src ip
	a.S_un.S_addr=iphdr->source;
	CString srcIP= inet_ntoa(a);
	//dest ip
	a.S_un.S_addr=iphdr->destination;
	CString destIP=inet_ntoa(a);
	//src port
	CString srcPort="N/A";
	//dest port 
	CString destPort="N/A";
	//ttl
	CString ttl;
	ttl.Format("%d",iphdr->ttl);
	//Size
	CString PacketSize;
	PacketSize.Format("%d",size);
	//TCP Sequence
	CString TCPSeq="N/A";
	//TCP Flag
	CString TCPFlag="N/A";
	//tcp Flag
	CString ICMPFlag="N/A";
	//protocol
	CString protocol="Unknown";

	ICMP_HEADER* icmphdr=(ICMP_HEADER*)(RecvBuffer+iphdrlen);
	UDP_HEADER* udphdr=(UDP_HEADER*)(RecvBuffer+iphdrlen);
	TCP_HEADER* tcphdr=(TCP_HEADER*)(RecvBuffer+iphdrlen);
	switch(iphdr->protocol)
	{
		case IPPROTO_ICMP:
			protocol="ICMP";
			switch(icmphdr->type)
			{
				case ICMP_ECHOREPLY:
					ICMPFlag="ECHO Reply";
					break;
				case ICMP_DEST_UNREACH:
					ICMPFlag="Destination Unreachable";
					break;
				case ICMP_SOURCE_QUENCH:
					ICMPFlag="Source Quench";
					break;
				case ICMP_REDIRECT:
					ICMPFlag="Redirect";
					break;
				case ICMP_ECHO:
					ICMPFlag="ECHO Request";
					break;
				case ICMP_TIME_EXCEEDED:
					ICMPFlag="Time Exceed";
					break;
			}
			break;
		case IPPROTO_IGMP:
			protocol="IGMP";
			break;
		case IPPROTO_UDP:
			protocol="UDP";
			srcPort.Format("%i",ntohs(udphdr->SourcePort));
			destPort.Format("%i",ntohs(udphdr->DestinationPort));
			break;
		case IPPROTO_ND:
			protocol="UNOFFICIAL net disk";
			break;
		case IPPROTO_RAW:
			protocol="RAW";
			break;
		case IPPROTO_TCP:
			protocol="TCP";
			srcPort.Format("%i",ntohs(tcphdr->SourcePort));
			destPort.Format("%i",ntohs(tcphdr->DestinationPort));
			TCPSeq.Format("%ld",ntohs(tcphdr->SequenceNumber));
			TCPFlag.Format("%0X",ntohs(tcphdr->Flags));			
			break;
	}
	int index=this->m_List.InsertItem(LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE,0,srcIP,NULL,NULL,0,0);
	this->m_List.SetItemText(index,1,destIP);
	this->m_List.SetItemText(index,2,srcPort);
	this->m_List.SetItemText(index,3,destPort);
	this->m_List.SetItemText(index,4,protocol);
	this->m_List.SetItemText(index,5,ttl);
	this->m_List.SetItemText(index,6,PacketSize);
	this->m_List.SetItemText(index,7,TCPFlag);
	this->m_List.SetItemText(index,8,ICMPFlag);
	this->m_List.SetItemText(index,9,TCPSeq);


	

}

void PacketSniffer::OnEnChangeData()
{
	
}

void PacketSniffer::OnBnClickedButton1()
{
	
	if(m_pThrd!=NULL)
	{
		if(this->m_Running)
		{
			this->m_pThrd->SuspendThread();
			SetDlgItemText(IDC_BUTTON1,"Start");
			m_Running=false;

		}
		else
		{
			InitSock();
			if(this->m_isInit==1)
			{
				this->m_pThrd->ResumeThread();
				SetDlgItemText(IDC_BUTTON1,"Stop");
				m_Running=true;
			}
		}
	}
	else
	{
		InitSock();
		if(this->m_isInit==1)
		{
			this->m_pThrd=AfxBeginThread(PacketSniffer::ThrdPacketCapture,this);
			m_Running=true;;
			SetDlgItemText(IDC_BUTTON1,"Stop");			
		}
	}
}

void PacketSniffer::OnBnClickedClear()
{
	this->m_List.DeleteAllItems();
}
 
