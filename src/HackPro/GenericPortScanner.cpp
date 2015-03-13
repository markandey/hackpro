#include "StdAfx.h"
#include "Hackpro.h"
#include "HackProDlg.h"
#include "genericportscanner.h"

GenericPortScanner::GenericPortScanner(CListCtrl* List,CButton* btscan,CStatic* out)
{
	m_OutPutList=List;
	m_btScan=btscan;
	m_out=out;
}

GenericPortScanner::~GenericPortScanner(void)
{

}

CString GenericPortScanner::IncrIP(CString IP)
{
	CString resToken;
	CArray<BYTE,BYTE> ArrByte; 
	int curPos= 0;
	resToken= IP.Tokenize(".",curPos);
	while (resToken != "")
	{
		BYTE temp=(BYTE)atoi(resToken);
		ArrByte.Add(temp);
		resToken= IP.Tokenize(".",curPos);		
	}
	BYTE b1=ArrByte[0],b2=ArrByte[1],b3=ArrByte[2],b4=ArrByte[3];
	::IncrIp(&b1,&b2,&b3,&b4);
	CString ret;
	ret.Format("%d.%d.%d.%d",b1,b2,b3,b4);
	return ret;
}
void GenericPortScanner::PortScan(CString IP,unsigned short Port1,unsigned short Port2)
{
	int p1,p2;
	p1=(Port1<Port2)?Port1:Port2;
	p2=(Port1<Port2)?Port2:Port1;
	if(p1==p2)
	{
		CString def;
		def="Starting and Ending Ports are same";
		MessageBox(NULL,def,"Information",MB_OK);
		return;
	}
	this->m_btScan->EnableWindow(false);
	int N=p2-p1;//Count total number of ports to scan;
    ScanResult *r=new ScanResult[N+1];
	for(int i=0;i<=N;i++)
	{
		r[i].IP=IP;
		r[i].Port=p1+i;
		r[i].Result=false;
		r[i].This=this;
	}
	ScanInput* si=new ScanInput;
	si->This=this;
	si->Input=r;
	si->InputCount=N+1;
	AfxBeginThread(GenericPortScanner::Start,(LPVOID)(si));
	return;
}
void GenericPortScanner::PortScan(CString IP1,CString IP2,unsigned short Port)
{
	this->m_btScan->EnableWindow(false);
	CString IP=IP1;
	int N=this->IPValCmp(IP,IP2);//Count total number of Objects to scan;
	if(N<0)
	{
		return;
	}
    ScanResult *r=new ScanResult[N+1];//Allocate the packet
	int ptr=0;
	while(this->IPValCmp(IP,IP2)>=0)
	{
		ScanResult temp;
		temp.IP=IP;
		temp.Port=Port;
		temp.Result=false;
		temp.This=this;
		r[ptr++]=temp;
		IP=this->IncrIP(IP);
		if(ptr>N)
		{
			break;
		}
	}
	ScanInput* si=new ScanInput;
	si->This=this;
	si->Input=r;
	si->InputCount=N+1;
	AfxBeginThread(GenericPortScanner::Start,(LPVOID)(si));
	return;
}
void GenericPortScanner::PortScan(CString IP,unsigned short* Port,int no)
{
	this->m_btScan->EnableWindow(false);
	int N=no;
    ScanResult *r=new ScanResult[N+1];
	for(int i=0;i<=N;i++)
	{
		r[i].IP=IP;
		r[i].Port=Port[i];
		r[i].Result=false;
		r[i].This=this;
	}
	ScanInput* si=new ScanInput;
	si->This=this;
	si->Input=r;
	si->InputCount=N+1;
	AfxBeginThread(GenericPortScanner::Start,(LPVOID)(si));
	return;
}
UINT GenericPortScanner::Start(LPVOID pParam)
{
	ScanInput* si=(ScanInput*)pParam;
	si->This->m_out->SetWindowText("Scaning..........");
	si->This->ThrdCount=0;
	int N=si->InputCount;
	for(int i=0;i<N;i++)
	{
		AfxBeginThread(GenericPortScanner::TryOnPort,(LPVOID)((si->Input)+i));
		(si->This->ThrdCount)++;
		if(!(i%10))
		{
			int p=(i)*100;
			p/=N;
			CString str;
			str.Format("%d  percent completed",p);
			si->This->m_out->SetWindowText(str);
		}
	}
	//---------
	si->This->m_OutPutList->DeleteAllItems();
	//---------
	while(si->This->ThrdCount>0)
	{
		/*float t=(float)si->This->ThrdCount;
		float p=(N-(si->This->ThrdCount))*50;
		p/=N;
		CString str;
		str.Format("%f  percent completed",p+50);
		si->This->m_out->SetWindowText(str);*/
		Sleep((si->This->ThrdCount*100)/N);
	}
	si->This->m_out->SetWindowText("100 percent completed");
	for(i=0;i<N;i++)
	{
		if(si->Input[i].Result==true)
		{
			int index=si->This->m_OutPutList->GetItemCount();
			index=si->This->m_OutPutList->InsertItem(LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE,index,si->Input[i].IP,NULL,NULL,0,4);
			CString Port;
			Port.Format("%d",si->Input[i].Port);
			si->This->m_OutPutList->SetItemText(index,1,Port); 	
			si->This->m_OutPutList->SetItemText(index,2,"Open");
			CString Des=si->This->GetDescription(si->Input[i].Port);
			si->This->m_OutPutList->SetItemText(index,3,Des);        
		}
			
	}
	si->This->m_btScan->EnableWindow(true);

	//-----Delete Allocated Memory----
	delete[] (si->Input);
	delete si;
	//--------------------------------
	return 0;

}
UINT GenericPortScanner::TryOnPort(LPVOID pParam)
{
		ScanResult* res=(ScanResult*)pParam;
		SOCKET Sock;
		Sock=socket(AF_INET,SOCK_STREAM,0);
		sockaddr_in addr;
		addr.sin_family=AF_INET;
		addr.sin_port=htons(res->Port);
		addr.sin_addr.S_un.S_addr=(inet_addr(res->IP));
		int err=connect(Sock,(sockaddr*)&addr,sizeof(addr));
		if(err==0)
		{
			res->Result=true;
			
		}
		(res->This->ThrdCount)--;
		closesocket(Sock);
		return 0;
}
CString GenericPortScanner::GetDescription(int port)
{
	PortData t;
	PortData pd[]=
		{
		{7,"echo"},{11,"Systat"},{19,"Chargen"},{21,"ftp-data"},{22,"ssh"},
		{23,"telnet"},{25,"SMTP"},{42,"nameserver"},{43,"whois"},{52,"xns-time"},
		{53,"dns-zone"},{63,"Whois++"},{67,"bootps"},{68,"bootps"},{66,"oracle sqlnet"},
		{69,"tftp"},{70,"gopher"},{79,"finger"},{80,"HTTP"},{81,"Alternate HTTP"},
		{88,"Kerberose or alternate HTTP"},{109,"POP2"},{110,"POP3"},{111,"Sun RPC"},{118,"SQL server"},
		{119,"NNTP"},{123,"NTP"},{135,"Windows RPC"},{137,"Netbios ns"},{138,"Netbios dgm"},
		{139,"Netbios"},{143,"iMap"},{161,"SNMP"},{162,"SNMP -trap"},{177,"xdmcp"},
		{179,"bgp"},{256,"SNMP check Point"},{258,"HackPro"},{389,"ldap"},{396,"Netware -IP"},
		{407,"timbuktu"},{443,"HTTP/SSL"},{445,"MS-smb-alternate"},{500,"IPSec(ike)"},{512,"Exec"},
		{513,"rLogin/rWho"},{514,"rshell/syslog"},{515,"printer"},{517,"talk"},{518,"ntalk"},
		{520,"route"},{524,"Netware-ncp"},{529,"irc-serv"},{540,"uucp"},{543,"klogin"},	
		{645,"mount"},{799,"RemotelyPossible"},{873,"rsync"},{901,"samba-swat"},{1024,"Windows RPC Service"},
		{1025,"Windows RPC Service"},{1026,"Windows RPC Service"},{1027,"Windows RPC Service"},{1028,"Windows RPC Service"},{1029,"Windows RPC Service"},
		{1030,"Windows RPC Service"},{1080,"socks"},{1109,"kpop"},{1313,"bmc-patrol-db"},{1352,"notes"},
		{1433,"MS-SQL"},{1494,"citrix"},{1498,"sybase-sql-anywhere"},{1505,"funk-proxy"},{1525,"Oracle Server"},
		{1527,"Oracle tli"},{1734,"PPTP"},{1745,"WinProxy"},{1812,"RADIUS"},{2000,"RemotelyAnyWhere"},
		{2001,"CISCO Mgmt"},{2049,"nfs"},{3306,"MySQL"},{3351,"sSQL"},{3389,"ms-termserv"},
		{4001,"CISCO mgnt"},{5000,"Windows Scvhost"},{6000,"XWindows"},{6001,"CISCO mgnt"},
		{6050,"Arc Server"},{6549,"APC"},{6667,"IRC"},{8000,"Web"},{8001,"Web"},
		{8002,"Web"},{8080,"Web"},{9991,"iss system scanner agent"},{9992,"iss system scanner console"},{32771,"RPC -solaris"}
		};
	int Count=((unsigned long)&t-(unsigned long)pd)/sizeof(PortData);
	for(int i=0;i<Count;i++)
	{
		if(pd[i].port==port)
		{
			return pd[i].Des;
		}
	}
	return "Unknown";


}
int GenericPortScanner::IPValCmp(CString IP1,CString IP2)
{
	CString resToken;
	CArray<BYTE,BYTE> ArrByte; 
	int curPos= 0;
	resToken= IP1.Tokenize(".",curPos);
	while (resToken != "")
	{
		BYTE temp=(BYTE)atoi(resToken);
		ArrByte.Add(temp);
		resToken= IP1.Tokenize(".",curPos);		
	}
	BYTE a1=ArrByte[0],a2=ArrByte[1],a3=ArrByte[2],a4=ArrByte[3];

	//-------------
	ArrByte.RemoveAll();; 
	curPos= 0;
	resToken= IP2.Tokenize(".",curPos);
	while (resToken != "")
	{
		BYTE temp=(BYTE)atoi(resToken);
		ArrByte.Add(temp);
		resToken= IP2.Tokenize(".",curPos);		
	}
	BYTE b1=ArrByte[0],b2=ArrByte[1],b3=ArrByte[2],b4=ArrByte[3];

	int A=::GetIpVal(a1,a2,a3,a4);
	int B=::GetIpVal(b1,b2,b3,b4);
	return B-A;

}