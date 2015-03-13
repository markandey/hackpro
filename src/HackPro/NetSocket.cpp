#include "StdAfx.h"
#include "netsocket.h"


CNetSocket::CNetSocket(void)
{
	
	this->hSock=NULL; 
}

CNetSocket::~CNetSocket(void)
{
	
}


/**
IPPROTO_IP           dummy for IP
IPPROTO_ICMP         control message protocol
IPPROTO_IGMP         internet group management protocol
IPPROTO_GGP			 gateway^2 (deprecated) 
IPPROTO_TCP          tcp 
IPPROTO_PUP          pup
IPPROTO_UDP          user datagram protocol
IPPROTO_IDP          xns idp
IPPROTO_ND           UNOFFICIAL net disk proto
IPPROTO_RAW          raw IP packet
*/
//on error return zero
int CNetSocket::InitSocket(int Protocol)
{
	/*Initialization of socket
	The socket function creates a socket that is bound to a specific service provider.
	AF_INET   internetwork: UDP, TCP, etc.
	#define SOCK_RAW        3               raw-protocol interface
	*/
	hSock = socket(AF_INET,SOCK_RAW, Protocol);
	
	if(hSock==INVALID_SOCKET)
	{
		CString s;
		s.Format("Could not initialize the socket Error\nsocket(AF_INET,SOCK_RAW, Protocol) function failed return=%d\n",hSock);
		s+="you may not have enough permission to create RAW socket";
		AfxMessageBox(s,MB_OK);
		return 0;
	}
	return 1;
    
}
int CNetSocket::SetOpt()
{
	unsigned long i=TRUE;
	int err;
		
	if(hSock==NULL)
	{
		MessageBox(NULL,"Socket is not Initialized!","Error:NetSocket",MB_OK);
		return 0;
	}
	
	
	//-------------------------Get the BroadCast Permission---------
	setsockopt( hSock, SOL_SOCKET, SO_BROADCAST, (char *)&i, sizeof( i )); 
	if( err=WSAGetLastError())
	{ 
		CString s;
		s.Format("BroadCast Permission Failed\n",err);
		MessageBox(NULL,s,"Error",MB_OK);
		return 0;	
		
	}

	//-------------------------Get NonBlocking Socket Permission---------
	if( ioctlsocket( hSock, FIONBIO, (unsigned long *)&i ))
	{
        
        MessageBox( NULL, "Setting socket to nonblocking failed\n","Error", MB_OK);

	}

	//-------------------------Get IP Header Inclusion Permission---------
	setsockopt( hSock, IPPROTO_IP, IP_HDRINCL, (char *)&i, sizeof( i ));
    if( err = WSAGetLastError())
	{ 
                                  
		CString msg;
		msg.Format("you don't have permission to include IP header",err);
		MessageBox(NULL,msg,"Error",MB_OK);
		return 0;
            
	}
	return 1;
}
/**
	return the number of bytes send
*/
int CNetSocket::SendPacket(int protocol,int ttl,int port,unsigned long source,unsigned long destination,char* Data,int size)
{
	//create the packet
	char* Packet;	//Packet to send
	int totlen=sizeof(IP_HEADER)+size; //calculate total size of packet
	Packet=new char[totlen];		//allocate memeory for packet

	IP_HEADER head;//=(PIP_HEADER)Packet;
		//Initialize the Packet
		head.ihl=sizeof(IP_HEADER)>>2;			// Size of the header/4
		head.version= 4;						//IP Version 4
		head.type_service=	NULL;				//Type of Service
		head.tot_len=NULL;						//total length will be Calculated letter
		head.id=0;								//ID
		head.frag_off=	NULL;					//fragment offset NULL
		head.ttl=	ttl;						//keep it 128 (Microsoft Standard)
		head.protocol=	protocol;				//protocol as told
		head.check =	NULL;					//checksum will be calculated letter
		head.source=htonl(source);				//Packet came where from ? 
		head.destination=htonl(destination);	//Packet going to where

	head.tot_len= htons(totlen);
	
	//Add the data to be send in the packet
	memcpy((void*)Packet,(void*)(&head),sizeof(IP_HEADER));
	memcpy((void*)(Packet+sizeof(IP_HEADER)),(void*)Data,size);
	
	//Finalize the packet
	IP_HEADER* fhead=(IP_HEADER*)Packet;
	fhead->check =this->checksum((unsigned short*)Packet,totlen); 
	
	//===============Now send the Finalized Packet================
			//initializing destination address
			sockaddr_in d;
			d.sin_family=AF_INET;
			d.sin_port=htons(port);
			d.sin_addr.S_un.S_addr=htonl(destination);
	int sentbyte=sendto(hSock,Packet,totlen,NULL,(sockaddr*)&d,sizeof(d));
	if(sentbyte<0)
	{
		int err=WSAGetLastError();
		if(err!=NULL)
		{
			CString s;
			s.Format("Error in Sending the Packet\n",err);
			
			//----------------
			LPVOID lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,  NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL);
			s=s+(char*)lpMsgBuf;
			LocalFree( lpMsgBuf );
			MessageBox(NULL,s,"Error:CNetSocket",MB_OK);
			//----------------
			
		}
	}	
		 delete Packet;
	     return sentbyte;

}
/**
	calculates the checksum
*/
int CNetSocket::SendPacket(int protocol,PacketParam param,char* Data,int size)
{
	return this->SendPacket(protocol,param.ttl,param.port,param.source,param.destination,Data,size);
}
/**
calculates the checksum
*/
unsigned short CNetSocket::checksum( unsigned short *buffer, int size )
{
   /*
		internet checksum is the complement of sum of of the
			1.all the words in buffer
			2.high order carry
			i.e.
		checksum=~(Sall words + high carry)
   */
	register unsigned long cksum = 0;
//=====================Add All Words===============
	while( size > 1 )
   {

			cksum += *buffer++;
			size -= sizeof( unsigned short );
   }
   if( size )
   {
       cksum += (unsigned short)(*(unsigned char *)buffer);
   }
//=============Add High Carry=======================

   cksum =  (cksum >>16)+(cksum & 0xFFFF);
   cksum += (cksum >>16);
//==================Take Complement of this==========
   cksum=~cksum;
   return (unsigned short)(cksum);
}
/**
This function sends the ICMP packet with the given header
*/
int CNetSocket::SendTCPPacket(PacketParam param,TCP_HEADER tcphead,char* Data,int Size)
{
	//Allocate the packet data to send
	int buffsize=Size+sizeof(TCP_HEADER);
	char* Buffer=new char[buffsize];

	//Keep the checksum =0;
	tcphead.Checksum=0;
		
	//Copy all data to Buffer
	memcpy((void*)Buffer,&tcphead,sizeof(TCP_HEADER));
	if(Size>0)
	{
		memcpy((void*)(Buffer+sizeof(TCP_HEADER)),(void*)Data,Size);
	}

	//Calculate the checksum
	unsigned short cksum=checksum((unsigned short*)Buffer,buffsize);
	((TCP_HEADER*)Buffer)->Checksum=cksum;

	//Send the Packet
	int ret=SendPacket(IPPROTO_TCP,param,Buffer,buffsize);
	delete Buffer;
	return ret;
}
/**
This function sends the UDP packet to the parameter specified
*/			
int CNetSocket::SendTCPPacket(PacketParam param,unsigned short DestinationPort,unsigned char Flags,unsigned short SourcePort,unsigned long SequenceNumber)
{
	TCP_HEADER hdr;
	hdr.AcknowledgeNumber=1;
	hdr.Checksum=0;
	hdr.DataOffset=0;
	hdr.DestinationPort=htons(DestinationPort);
	hdr.Flags=Flags;
	hdr.SequenceNumber=SequenceNumber;
	hdr.SourcePort=htons(SourcePort);
	hdr.UrgentPointer=0;
	hdr.Windows=0;

	return SendTCPPacket(param,hdr,NULL,0);
}
/**
This function sends the ICMP packet with the given header
*/
int CNetSocket::SendICMPPacket(PacketParam param,ICMP_HEADER icmphead,char* Data,int Size)
{
	//Allocate the packet data to send
	int buffsize=Size+sizeof(ICMP_HEADER);
	char* Buffer=new char[buffsize];

	
	//Copy all data to Buffer
	memcpy((void*)Buffer,&icmphead,sizeof(ICMP_HEADER));
	if(Size>0)
	{
		memcpy((void*)(Buffer+sizeof(ICMP_HEADER)),(void*)Data,Size);
	}
	ICMP_HEADER* head=(ICMP_HEADER*)Buffer;
	head->checksum=this->checksum((unsigned short*)Buffer,buffsize);
	//Send the Packet
	int ret=SendPacket(IPPROTO_ICMP,param,Buffer,buffsize);
	delete Buffer;
	return ret;
}
/**
This function sends the ICMP packet to the parameter specified
*/
int CNetSocket::SendICMPPacket(PacketParam param,unsigned char type,unsigned short seq)
{
	
	char *Buffer="abcdefghijklmnopqrstuvwxyzabcd";
	ICMP_HEADER hdr;
	hdr.type=type;      
	hdr.code=0;             //No sub type.
	hdr.un.echo.id=5;     //set something random
	hdr.un.echo.sequence = (unsigned short)seq;   //Sequence number.
	hdr.checksum         = 0;              // Zero checksum.
	return this->SendICMPPacket(param,hdr,Buffer,28);
}
/**
This function sends the UDP packet to the parameter specified
*/
int CNetSocket::SendUDPPacket(PacketParam param,UDP_HEADER udphead,char* Data,int Size)
{
	//Allocate the packet data to send
	int buffsize=Size+sizeof(UDP_HEADER);
	char* Buffer=new char[buffsize];

	
	//Copy header and data to Buffer
	memcpy((void*)Buffer,&udphead,sizeof(UDP_HEADER));
	if(Size>0)
	{
		memcpy((void*)(Buffer+sizeof(UDP_HEADER)),(void*)Data,Size);
	}
	
	//calculate the checksum
	UDP_HEADER* head=(UDP_HEADER*)Buffer;
    head->Checksum=this->checksum((unsigned short*)Buffer,sizeof(UDP_HEADER)+Size);

	//Send the Packet
	int ret=SendPacket(IPPROTO_UDP,param,Buffer,buffsize);
	
	delete Buffer;
	return ret;
}
/**
call this function to close the socket attched with its object
*/
void CNetSocket::close()
{
	closesocket(this->hSock);
}
