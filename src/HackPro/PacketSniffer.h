#ifndef PACKETSNIFFER_H
#define PACKETSNIFFER_H
#include <queue>
#include "afxcmn.h"
#include "afxwin.h"


// PacketSniffer dialog
#define HACK_MAXPACKETSIZE 65535 //(2^16=65536)
#define SIO_RCVALL            _WSAIOW(IOC_VENDOR,1)
class PacketSniffer : public CDialog
{
	DECLARE_DYNAMIC(PacketSniffer)
	SOCKET m_Sock;
	int m_isInit;
	CWinThread* m_pThrd;
	bool m_Running;
	

public:
	void ListInterfaces();
	void InitSock();	
	static UINT ThrdPacketCapture(LPVOID param);
	void AnalyzePacket(char* RecvBuffer,int size);	
	PacketSniffer(CWnd* pParent = NULL);   // standard constructor
	virtual ~PacketSniffer();

// Dialog Data
	enum { IDD = IDD_PACKETCAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	afx_msg void OnEnChangeData();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedClear();
	CComboBox m_IPList;
};
#endif