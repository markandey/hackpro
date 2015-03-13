#ifndef REGCONFIGDLG_H
#define REGCONFIGDLG_H

#include "afxcmn.h"
#include "afxwin.h"
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "stack.h"
#include "HackProdlg.h"
#include "regdatatree.h"

//#include "papahead.h"



//=============================Some important structures==============
struct Add_Key_Reg
{
	char  hKey_str[25];
	char Key[1024];
	char  value[255];
	char  type_str[10];
	char  data[255];
};
#pragma message ("RegConfigDlg: structure Add_Key_Reg completed")
//====================================================================
class CRegConfigDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CRegConfigDlg)

	
private:
	//==================My Functions============
	RegdataTree tree;
	Level* CurrentField;//for getting and setting current fields
	CStack Stack;
	CHackClient* Papa;
	void ShowLevel(Level* l);
	void AddEntries();
	void AdditionalTask(int value);
	void ShowInfo(CString s);
	int AddKeyReg(CString hKey_str,LPCTSTR Key,LPCTSTR value,CString type_str,char* data,int i=1);
	void ReadArchive(CString filename);
	void Tooltip(CString msg,CString cap);
	//==========================================
public:
	CImageList ImgLst;
	//bool done;
	CRegConfigDlg();
	CRegConfigDlg(CHackClient* Parent);
	static void* CALLBACK_RegVal(void* data,int size);
	void InitControl();
	virtual ~CRegConfigDlg();

// Dialog Data
	enum {IDD=IDD_REGCONFIG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	CComboBox m_EdtHkey;
	CEdit EdtPath;
	CEdit m_EdtValName;
	CComboBox m_EdtType;
	CEdit m_EdtData;
	afx_msg void OnHdnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdtpath();
	afx_msg void OnEnChangeEdtdata();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
};
#pragma message ("Here regconfigdlg class defined")
#endif