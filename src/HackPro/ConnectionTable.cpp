// ConnectionTable.cpp : implementation file
//

#include "stdafx.h"
#include "HackPro.h"
#include "ConnectionTable.h"


// CConnectionTable dialog

IMPLEMENT_DYNAMIC(CConnectionTable, CDialog)
CConnectionTable::CConnectionTable(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectionTable::IDD, pParent)
{
}

CConnectionTable::~CConnectionTable()
{
}

void CConnectionTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
}

BEGIN_MESSAGE_MAP(CConnectionTable, CDialog)
	ON_BN_CLICKED(IDC_REFRESHTABLE, OnBnClickedRefreshtable)
END_MESSAGE_MAP()


// CConnectionTable message handlers

BOOL CConnectionTable::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT ); 
	this->m_List.InsertColumn(0,"Local IP",LVCFMT_LEFT,100);
	this->m_List.InsertColumn(1,"Remote IP",LVCFMT_LEFT,100);
	this->m_List.InsertColumn(2,"Local Port",LVCFMT_LEFT,100);
	this->m_List.InsertColumn(3,"Remote Port",LVCFMT_LEFT,100);
	this->m_List.InsertColumn(4,"Protocol",LVCFMT_LEFT,70);
	this->m_List.InsertColumn(5,"State",LVCFMT_LEFT,100);
	GetTable();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CConnectionTable::GetTable()
{
	this->m_List.DeleteAllItems();
	int Offset=0;
	DWORD dwSize=0;
	GetTcpTable(NULL,&dwSize,2);
	if(dwSize>0)
	{
		char* Buffer= new char[dwSize];
		MIB_TCPTABLE* pTable=(MIB_TCPTABLE*)Buffer;
		if(GetTcpTable(pTable,&dwSize,2)==NO_ERROR)
		{
			DWORD count=pTable->dwNumEntries;
			Offset=count;
			MIB_TCPROW* pRows=pTable->table;
			for(DWORD i=0;i<count;i++)
			{
				in_addr a;
				a.S_un.S_addr=pRows[i].dwLocalAddr;
				int index=this->m_List.InsertItem(i,inet_ntoa(a),0);
				
				a.S_un.S_addr=pRows[i].dwRemoteAddr;
				this->m_List.SetItemText(index,1,inet_ntoa(a));

				CString strCov;
				strCov.Format("%u",ntohs(pRows[i].dwLocalPort));
				this->m_List.SetItemText(index,2,strCov);

				strCov.Format("%u",ntohs(pRows[i].dwRemotePort));
				this->m_List.SetItemText(index,3,strCov);

				
				this->m_List.SetItemText(index,4,"TCP");
				
				switch(pRows[i].dwState)
				{
					case MIB_TCP_STATE_CLOSED:
						this->m_List.SetItemText(index,5,"Closed");
						break;
					case MIB_TCP_STATE_LISTEN:
						this->m_List.SetItemText(index,5,"Listening");
						break;
					case MIB_TCP_STATE_SYN_SENT:
						this->m_List.SetItemText(index,5,"SYN Sent");
						break;
					case MIB_TCP_STATE_SYN_RCVD:
						this->m_List.SetItemText(index,5,"SYN Recieved");
						break;
					case MIB_TCP_STATE_ESTAB:
						this->m_List.SetItemText(index,5,"Established");
						break;
					case MIB_TCP_STATE_FIN_WAIT1:
						this->m_List.SetItemText(index,5,"FIN wait1");
						break;
					case MIB_TCP_STATE_FIN_WAIT2:
						this->m_List.SetItemText(index,5,"FIN wait2");
						break;
					case MIB_TCP_STATE_CLOSE_WAIT:
						this->m_List.SetItemText(index,5,"Closed Wait");
						break;
					case MIB_TCP_STATE_CLOSING:
						this->m_List.SetItemText(index,5,"Closing");
						break;
					case MIB_TCP_STATE_LAST_ACK:
						this->m_List.SetItemText(index,5,"Last ACK");
						break;
					case MIB_TCP_STATE_TIME_WAIT:
						this->m_List.SetItemText(index,5,"Time Wait");
						break;
					case MIB_TCP_STATE_DELETE_TCB:
						this->m_List.SetItemText(index,5,"Delete TCB");
						break;
				}			
			}			
		}
		delete Buffer;
	}

	//-------------
	dwSize=0;
	GetUdpTable(NULL,&dwSize,2);
	if(dwSize>0)
	{
		char* Buffer= new char[dwSize];
		MIB_UDPTABLE* pTable=(MIB_UDPTABLE*)Buffer;
		if(GetUdpTable(pTable,&dwSize,2)==NO_ERROR)
		{
			DWORD count=pTable->dwNumEntries;
			MIB_UDPROW* pRows=pTable->table;
			for(DWORD i=0;i<count;i++)
			{
				
				in_addr a;
				a.S_un.S_addr=pRows[i].dwLocalAddr;
				int index=this->m_List.InsertItem(Offset+i,inet_ntoa(a),0);
				
				
				this->m_List.SetItemText(index,1,"--");

				CString strCov;
				strCov.Format("%u",ntohs(pRows[i].dwLocalPort));
				this->m_List.SetItemText(index,2,strCov);

				this->m_List.SetItemText(index,3,"--");

				
				this->m_List.SetItemText(index,4,"UDP");
				
				this->m_List.SetItemText(index,5,"--");						
						
			}			
		}
		delete Buffer;
	}




}
void CConnectionTable::OnBnClickedRefreshtable()
{
	GetTable();
}
