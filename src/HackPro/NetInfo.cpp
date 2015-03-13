// NetInfo.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "NetInfo.h"
#include <IPIfCons.h>


// CNetInfo dialog

IMPLEMENT_DYNAMIC(CNetInfo, CDialog)
CNetInfo::CNetInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CNetInfo::IDD, pParent)
{
}

CNetInfo::~CNetInfo()
{
}

void CNetInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNetInfo, CDialog)
END_MESSAGE_MAP()




BOOL CNetInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	AfxBeginThread(CNetInfo::ThrdGetInfo,this);
	return TRUE; 
	
}
UINT CNetInfo::ThrdGetInfo(LPVOID param)
{
	CNetInfo* This=(CNetInfo*)param;
	CString str=This->GetInfo();
	This->SetDlgItemText(IDC_INFO,str);
	FIXED_INFO finfo;
	DWORD Size=sizeof(finfo);
	GetNetworkParams(&finfo,&Size);
	This->SetDlgItemText(IDC_INFOHOSTNAME,finfo.HostName);

	//domain name
	if(strlen(finfo.DomainName)<1)
	{
		This->SetDlgItemText(IDC_INFODOMAINNAME,"Not Found");
	}
	else
	{
		This->SetDlgItemText(IDC_INFODOMAINNAME,finfo.DomainName);
	}
	//dnsserver ip
	if(strlen(finfo.DnsServerList.IpAddress.String)<1)
	{
		This->SetDlgItemText(IDC_INFODNSSERVER,"0.0.0.0");
	}
	else
	{
		This->SetDlgItemText(IDC_INFODNSSERVER,finfo.DnsServerList.IpAddress.String);
	}
	//node type
	switch(finfo.NodeType)
	{
		case BROADCAST_NODETYPE:
			This->SetDlgItemText(IDC_INFONODETYPE,"Broadcast");
			break;
		case PEER_TO_PEER_NODETYPE:
			This->SetDlgItemText(IDC_INFONODETYPE,"Peer to Peer");
			break;
		case MIXED_NODETYPE:
			This->SetDlgItemText(IDC_INFONODETYPE,"Mixed Node Type");
			break;
		case HYBRID_NODETYPE:
			This->SetDlgItemText(IDC_INFONODETYPE,"Hybrid Node");
			break;
		default:
			This->SetDlgItemText(IDC_INFONODETYPE,"Unknown Node Type");
			break;
	}
	//Dns Scope Id
	if(strlen(finfo.ScopeId)<1)
	{
		This->SetDlgItemText(IDC_INFODHCPSNAME,"Not found");
	}
	else
	{
		This->SetDlgItemText(IDC_INFODHCPSNAME,finfo.ScopeId);
	}
	//routing
	This->SetDlgItemText(IDC_INFOROUTING,"Enable");
	if(finfo.EnableRouting==0)
	{
		This->SetDlgItemText(IDC_INFOROUTING,"Disable");
	}
	//DNS
	This->SetDlgItemText(IDC_INFODNS,"Enable");
	if(finfo.EnableDns==0)
	{
		This->SetDlgItemText(IDC_INFODNS,"Disable");
	}
	//Proxy
	This->SetDlgItemText(IDC_INFOPROXY,"Enable");
	if(finfo.EnableProxy==0)
	{
		This->SetDlgItemText(IDC_INFOPROXY,"Disable");
	}
	return true;
}
CString CNetInfo::GetInfo()
{
	char* buffer=NULL;
	DWORD NumOfInterfaces;
	GetNumberOfInterfaces(&NumOfInterfaces);
	CString AdapterInformation;
	AdapterInformation.Format("Number of Inerfaces=%d",NumOfInterfaces);
	if(NumOfInterfaces>0)
	{
		CString LoopBackInfo;
		LoopBackInfo.Format("Name := %s\r\nDescription := %s\r\nMAC := %s\r\nType := %s\r\nDHCP := %s"
					"\r\nIP Address := %s\r\nGateway := %s\r\nDHCP := %s"
					,"Loopback","loop back Interface","N/A","Loopback","N/A","127.0.0.1","N/A","N/A");
		AdapterInformation=AdapterInformation+"\r\n---------------------------------------\r\n"+LoopBackInfo;
	}
	
	ULONG Size=0;
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
				CString str;
				//Name
				CString Name=pInfo->AdapterName;
				//Description
				CString Des=pInfo->Description;
				
				//MAC Address
				int Len=pInfo->AddressLength;
				PBYTE pbHexMac = pInfo->Address;
				CString PhyAddr;
				for(int i=0;i<(int)Len;i++)
				{
					CString temp;
					temp.Format("%02X:",pbHexMac[i]);
					PhyAddr+=temp;
				}
				//Type
				CString Type;
				switch(pInfo->Type)
				{
					case MIB_IF_TYPE_ETHERNET:
						 Type="Ethernet Adapter";
						 break;
					case MIB_IF_TYPE_TOKENRING:
						Type="Token Ring Adapter";
						break;
					case MIB_IF_TYPE_FDDI :
						Type="FDDI Adapter";
					case MIB_IF_TYPE_PPP :
						Type="PPP Type Adapter";
						break;
					case MIB_IF_TYPE_LOOPBACK:
						Type="LoopBack Type Adapter";
						break;
					case MIB_IF_TYPE_SLIP:
						Type="SLIP Type Adapter";
						break;
					default:
						Type="Other";
				}
				//Is DHCP enables
				CString isDHCP="Enabled";
				if(pInfo->DhcpEnabled==0)
				{
					isDHCP="Disabled";
				}
				
				//Get Assosiated IP Addresses
				CString strIPAddr=pInfo->IpAddressList.IpAddress.String;
				
				//Get The Gateway Address
				CString strGatewayAddr=pInfo->GatewayList.IpAddress.String;

				//Get the DHCP Server Address
				CString strDHCPAddr=pInfo->DhcpServer.IpAddress.String;

				str.Format("Name := %s\r\nDescription := %s\r\nMAC := %s\r\nType := %s\r\nDHCP := %s"
					"\r\nIP Address := %s\r\nGateway := %s\r\nDHCP := %s"
					,Name,Des,PhyAddr,Type,isDHCP,strIPAddr,strGatewayAddr,strDHCPAddr);
				
				AdapterInformation=AdapterInformation+"\r\n---------------------------------------\r\n"+str;
				pInfo=pInfo->Next;

				


			}

		}

	}

	return AdapterInformation;


}
