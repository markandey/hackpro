// HackProDlg.h : header file
//
#ifndef HACKPRODLG_H
#define HACKPRODLG_H
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"

#include "MacAddrDlg.h"
#include "Portscandlg.h"
#include "PortScanSip.h"
#include "smurfdlg.h"
#include "RawPacketdlg.h"
#include "UDPAttackDlg.h"
#include "SynAttackDlg.h"
#include "PacketSniffer.h"

#include "Exsheet.h"
#include "inputip.h"
#include "aboutdlg.h"
#include "papahead.h"
#include "NetInfo.h"
#include "NetStat.h"
#include "ConnectionTable.h"
#include "BreachDetector.h"
#include "RevEngineer.h"


// CHackProDlg dialog
#define WM_TRAY_NOTIFY 2222
class CHackClient;
class CExSheet;
class CHackProDlg : public CDialog
{
// Construction
	CArray<CHackClient*,CHackClient*> Comm;
	CExSheet Hold;
	InputIp m_InputIp;
public:
	
	CHackProDlg(CWnd* pParent = NULL);	// standard constructor
	~CHackProDlg();
	//==============================================
	//Pages to be Shown without any conectivity;
	MacAddrDlg Page_MacAddr;
	CPortScanDlg Page_PortScan;
	PortScanSip Page_PortScanSip;
	CSmurfDlg Page_Smurf;
	CRawPacketDlg Page_RawPacket;
    AboutDlg Page_About;
	CSynAttackDlg Page_SynAttack;
	CUdpAttackDlg Page_UDPFloodAttack;
	PacketSniffer Page_PacketCap;
	CNetInfo Page_NetInfo;
	CNetStat Page_NetStat;
	CConnectionTable Page_ConnectionTable;
	CBreachDetector Page_BreachDetector;
	CRevEngineer Page_RevEngineer;
	//============Fuctions Created My Me=============
	void AddIcon();
	void InitApplication();
    void AddTreeItem(CString Path,LPARAM lParam);
	void AddHost(char* ip);
	void RemoveHost(CString IP);
	//===============================================
// Dialog Data
	enum { IDD = IDD_HACKPRO_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
// Implementation
protected:
	HICON m_hIcon;
	CPropertySheet m_hold;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// This is the tree for browsing among the fields
	CTreeCtrl MTree;
	CImageList ImgLst;
	NOTIFYICONDATA NotifyData;
	void OutStatus(CString s);
    afx_msg void OnTvnSelchangedMtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickMtree(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void OnCancel();
	virtual void OnOK();
	virtual void PostNcDestroy();
public:
	afx_msg void OnClose();
	afx_msg void OnFileConnect();
	afx_msg void OnMenu();
	afx_msg void OnHelpAbout();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	LONG OnTrayNotify(WPARAM wparam,LPARAM lparam);
	afx_msg void OnWindowShowmainwindow();
	afx_msg void OnWindowAbout();
};
#endif