#ifndef RAWPACKETDLG_H
#define RAWPACKETDLG_H
#include "hackpro.h"
#include "papahead.h"
#include "NetSocket.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
//#include "HackProdlg.h"
// CRawPacketDlg dialog



class CRawPacketDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CRawPacketDlg)

public:
	CRawPacketDlg();
	virtual ~CRawPacketDlg();

// Dialog Data
	enum { IDD = IDD_DIALOGRAW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	static UINT SndThrd(LPVOID param);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangePort();
	afx_msg void OnBnClickedSend();
	CIPAddressCtrl m_IPSrc;
	CIPAddressCtrl m_IPDest;
	CComboBox m_Protocol;
	afx_msg void OnCbnSelchangeProto();
	CEdit m_srcport;
	CEdit m_destport;
	CEdit m_datasize;
	CStatic m_lblsrcport;
	CStatic m_lbldestport;
	CStatic m_lbldatasize;
	CProgressCtrl m_progress;
};
#endif