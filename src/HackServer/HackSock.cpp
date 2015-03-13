// HackSock.cpp : implementation file
//

#include "stdafx.h"
#include "HackServer.h"
#include "HackSock.h"
#include "RemoteSock.h"


// CHackSock

CHackSock::CHackSock()
{

}

CHackSock::~CHackSock()
{

}


// CHackSock member functions

void CHackSock::OnAccept(int nErrorCode)
{
	
	
	if(nErrorCode!=0)
	{
		Error("Error During Accepting the Connection",0);
		return;
	}
	
	CRemoteSock* NewCons=new CRemoteSock(this);
	int len=sizeof(SOCKADDR);
	BOOL b=this->Accept(*NewCons,&(NewCons->addr),&len);
	if(b==0)
	{
		Error("Error During the Accepting the Connection",GetLastError());
		::Sleep(50000);
		delete NewCons;
		
	}
	else
	{
		//AfxGetMainWnd()->MakeLog("New Connection Accepted");
	}
	CSocket::OnAccept(nErrorCode);
}
