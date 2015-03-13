 

#ifndef __NETSOCKET_H
#define __NETSOCKET_H
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "HackProdlg.h"
       


class CNetSocket
{
	SOCKET  hSock; //handle to the socket
	
public:
	CNetSocket(void);
	~CNetSocket(void);
	int InitSocket(int Protocol);
	int SetOpt();//initializes the hsock to send Raw packets
	int SendPacket(int protocol,int ttl,int port,unsigned long source,unsigned long destination,char* Data,int size);
	int SendPacket(int protocol,PacketParam param,char* Data,int size);
	int SendTCPPacket(PacketParam param,TCP_HEADER tcphead,char* Data,int Size);
	
	int SendTCPPacket(PacketParam param,unsigned short	DestinationPort,unsigned char	Flags=TCPFLAG_SYN,unsigned short	SourcePort=0,unsigned long	SequenceNumber=1);
	
	int SendICMPPacket(PacketParam param, ICMP_HEADER icmphead,char* Data,int Size);
	
	int SendICMPPacket(PacketParam param,unsigned char type=ICMP_ECHO,unsigned short seq=1);
	
	int SendUDPPacket(PacketParam param,UDP_HEADER udphead,char* Data,int Size);
	
    unsigned short checksum( unsigned short *buffer, int size);
	void close();
};
//============================================================
#endif

/* ICMP_ECHOREPLY;ICMP_DEST_UNREACH;ICMP_SOURCE_QUENCH;ICMP_REDIRECT;ICMP_ECHO;ICMP_TIME_EXCEEDED*/