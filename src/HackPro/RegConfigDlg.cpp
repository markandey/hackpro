// RegConfigDlg.cpp : implementation file
//

#include "Stdafx.h"
#include "Hackpro.h"
#include "Tip.h"
#include "RegConfigDlg.h"
#include "hackclient.h"



IMPLEMENT_DYNAMIC(CRegConfigDlg, CPropertyPage)
CRegConfigDlg::CRegConfigDlg()
: CPropertyPage(CRegConfigDlg::IDD)
{	
}

CRegConfigDlg::CRegConfigDlg(CHackClient *Parent)
{
	this->Papa=Parent;
}
CRegConfigDlg::~CRegConfigDlg()
{
	
}

void CRegConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Control(pDX, IDC_EDTHKEY, m_EdtHkey);
	DDX_Control(pDX, IDC_EDTPATH, EdtPath);
	DDX_Control(pDX, IDC_EDTVALNAME, m_EdtValName);
	DDX_Control(pDX, IDC_EDTTYPE, m_EdtType);
	DDX_Control(pDX, IDC_EDTDATA, m_EdtData);
}


BEGIN_MESSAGE_MAP(CRegConfigDlg, CPropertyPage)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickList1)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
	ON_EN_CHANGE(IDC_EDTPATH, OnEnChangeEdtpath)
	ON_EN_CHANGE(IDC_EDTDATA, OnEnChangeEdtdata)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CRegConfigDlg message handlers
//==================================================
void CRegConfigDlg::InitControl()
{
	if(true)
	{	HICON hiconLarge[238],hiconSmall[238],I;
		HICON Misc[17],HDW[3],REG[9];
		ExtractIconEx("Shell32.dll",0,hiconLarge,hiconSmall,238);
		ExtractIconEx("main.cpl",0,Misc,hiconSmall,17);
		ExtractIconEx("Hdwwiz.cpl",0,HDW,hiconSmall,3);
		ExtractIconEx("regedit.exe",0,REG,hiconSmall,8);	
		ImgLst.Create(32,32,ILC_COLOR32,1,1);
		for(int i=0;i<238;i++)
		{
			I=hiconLarge[i];
			//I=ExtractIcon(AfxGetApp()->m_hInstance,"Shell32",i);
			ImgLst.Add(I);			 
		}
		for(i=0;i<18;i++)
		{
			I=Misc[i];
			ImgLst.Add(I);
		}
		for(i=0;i<3;i++)
		{
			I=HDW[i];
			ImgLst.Add(I);
		}
		for(i=0;i<8;i++)
		{
			I=REG[i];
			ImgLst.Add(I);
		}
		
	}
	//done=true;
	//==================================
	//CHackProDlg* Papa=(CHackProDlg*) ::AfxGetApp()->m_pMainWnd;
	m_ListCtrl.SetImageList(&ImgLst,LVSIL_NORMAL);
	AddEntries();		
	ShowLevel(tree.GetRootPtr());
}
void CRegConfigDlg::AddEntries()
{
	//tree.AddItem("","",); 
	tree.AddItem("Desktop\\Start Menu","Start Menu Scroll",START_MENU_SCROLL);
	tree.AddItem("Desktop","Remove MyComputer Shortcut",MYCOMPUTER_REMOVE);
	tree.AddItem("Desktop\\Start Menu","Remove Run From Start Menu",RUN_REMOVE);
	tree.AddItem("Desktop","Disable Right Click",DESKTOP_RIGHTCLICK);
    tree.AddItem("Desktop","Position the Wallpaper",DESKTOP_CHANGEWALLPOS);

	tree.AddItem("Software & Applications\\Windows Media Player","Disable Recent Files",DISABLE_RECENT_WMEDIA);
	tree.AddItem("Software & Applications\\Internet","Disable New Window", INTERNET_NEWWINDOW);
	tree.AddItem("Software & Applications\\Explorer","Open Cmd from Explorer", SOFTAWARE_OPENCMD);

	tree.AddItem("Privacy","Clear the Page File",PRIVACY_CLEARPAGEPFILE);
	tree.AddItem ("Privacy","Disable the LastUser Name", PRIVACY_LASTUSER);

	tree.AddItem("Security","Disable User Tracking",SECURITY_USERTRACKING); 
	tree.AddItem("Security\\MMC","Disable MMC Snaping",SECURITY_MMCSNAPING);
	tree.AddItem("Security\\MMC","Disable MMC AuthorMode",SECURITY_MMCAUTHORMODE);
	tree.AddItem("Security\\Security Dialog","Customize Security Dialog Title",SECURITY_SECURITYDIALOG);
	tree.AddItem("Security","Add Message Before Logon",SECURITY_MESSAGEBLOG);
	tree.AddItem("Security\\Security Dialog","Disable the Change Password",SECURITY_CHANGEPASSDISABLE);
	tree.AddItem("Security","Disable TaskManager",SECURITY_DISABLETASKM);
	tree.AddItem("Security","Disable AutoImport Registry",SECURITY_IMPORTREG);
	tree.AddItem("Security","Disable Display Settings",SECURITY_DISDISPLAY); 


	tree.AddItem("Hardware\\Drive","Hide drives",PRIVACY_HIDEDRIVES);
	tree.AddItem("Hardware\\CD","Disable AutoRun", HARDWARE_CDAUTORUN);
	
}	
	 
//================================
//+++++++++++++++++++++++++++++++++++++++++++++++++++
//this->ShowMainF(); 
//++++++++++++++++++++++++++++++++++++++++++++++++++++
void CRegConfigDlg::ShowLevel(Level* l)
{
	if(l==NULL)
	{
		return;
	}
	this->CurrentField=l;
	m_ListCtrl.DeleteAllItems(); 
	for(int i=0;i<l->GetSize();i++ )
	{
		char Caption[40];
		strcpy(Caption,l->GetItem(i).GetCaption());  
		int im=l->GetItem(i).GetImage();  
		m_ListCtrl.InsertItem(LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM,1,Caption ,NULL,NULL,im,i); 
	}
}



void CRegConfigDlg::AdditionalTask(int value)
{
	switch(value)
	{
		case SOFTAWARE_OPENCMD:
				this->AddKeyReg("HKEY_CLASSES_ROOT","Directory\\shell\\Command","(Default)","REG_SZ","Open Cmd here",15);
				break;
	}
}
void CRegConfigDlg::Tooltip(CString msg,CString cap)
{
	Tip Bt(msg,cap);
	Bt.DoModal();	
}

/************************************************************************
FUNCTION:: AddKeyReg
Return::   Void
hKey_str=any of the str in  {HKEY_CLASSES_ROOT;HKEY_CURRENT_CONFIG;HKEY_CURRENT_USER;HKEY_LOCAL_MACHINE}
Key= it is the path of the value to be added or modified
value=it is the name of the value to be created or modified
type_str=it can be any belongs to {REG_SZ;REG_DWORD;REG_BINARY}
data =it should be the string form of data to be added 
i=lenth of the data 
*************************************************************************/

int CRegConfigDlg::AddKeyReg(CString hKey_str,LPCTSTR Key,LPCTSTR value,CString type_str,char* data,int i/*=1*/)
{
	//=================================================================
	Add_Key_Reg d;
	strcpy(d.hKey_str ,hKey_str);
	strcpy(d.Key ,Key);
	strcpy(d.value ,value);
	strcpy(d.type_str ,type_str);
	strcpy(d.data ,data);
	this->Papa->SendRequest(1,1,sizeof(d),&d);   
	//=================================================================
	return 0;
	
}
void* CRegConfigDlg::CALLBACK_RegVal(void* data,int size)
{
	int *iData = (int*)data;
	if(*iData!=1)
	{
		AfxMessageBox("Registry Entry made successfully",MB_OK);
	}
	else
	{
		AfxMessageBox("Registry Access not completed successfully",MB_OK);
	}
	int* RetData =new int[2];
	RetData[0]=0;
	RetData[1]=0;
	return (void*)RetData;
}
/*
this function displays various information to be displayed in different
places dependin upon the single string argument of Format.
		<Title>~<Comment>~<hkey>~<Path>~<ValueName>~<TypeStr>~<Value>
		
		The Format of the O/p string will be
		0 : Title
		1 : Comment
		2 : hkey
		3 : Path
		4 : ValueName
		5 : TypeStr
		6 : Value
		

*/
void CRegConfigDlg::ShowInfo(CString s)
{
	
	CString resToken;
	CArray<CString,CString> str; 
	int curPos= 0;
	resToken= s.Tokenize("~",curPos);
	while (resToken != "")
	{
		str.Add(resToken);
		resToken= s.Tokenize("~",curPos);		
	}
    m_EdtHkey.SetWindowText(str.GetAt(2));
	m_EdtType.SetWindowText(str.GetAt(5));
	m_EdtValName.SetWindowText(str.GetAt(4));
	m_EdtData.SetWindowText(str.GetAt(6));
	EdtPath.SetWindowText(str.GetAt(3));
	Tooltip(str.GetAt(1),str.GetAt(0));   
	
}

void CRegConfigDlg::ReadArchive(CString filename)
{
	
	return;

}

//==================================================

void CRegConfigDlg::OnHdnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	int sm=m_ListCtrl.GetSelectionMark();  //Get the selected field;
	DWORD_PTR p = m_ListCtrl.GetItemData(sm);
	ItemData d =this->CurrentField->GetItem(p);    
	if(d.GetValue()==0  )
	{
		Stack.Push(this->CurrentField);
		//this->PreField=this->CurrentField;  
		this->ShowLevel(d.GetLink());
    }
	else 
	{
		CString s;
		s.LoadString(d.GetValue());
        this->ShowInfo(s);
		this->AdditionalTask(d.GetValue());
	}

	*pResult = 0;
}

void CRegConfigDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString hkey_str,Key_str,value_str ,type_str,data_str;
	m_EdtHkey.GetWindowText(hkey_str);
	EdtPath.GetWindowText(Key_str);
	m_EdtValName.GetWindowText(value_str);
	m_EdtType.GetWindowText(type_str);
	m_EdtData.GetWindowText(data_str);
	char d[100];
	strcpy(d,data_str);
	int ret = this->AddKeyReg(hkey_str,Key_str,value_str,type_str,d,strlen(d)); 
	if(ret==1)
	{
		AfxMessageBox("Data added to the registry.\nYou may have to log off or restart the system to take the effect.");
	}
}

void CRegConfigDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, "open", "regedit", NULL, NULL,SW_MAXIMIZE);

}

void CRegConfigDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	Level* ToGoto=NULL;
	if(Stack.Pop((void**)&ToGoto))
	{
		this->ShowLevel(ToGoto);
	}
}

BOOL CRegConfigDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	Papa->FT[CALLBACK_REGVAL]= CALLBACK_RegVal;  
	// TODO:  Add extra initialization here
	InitControl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRegConfigDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int sm=m_ListCtrl.GetSelectionMark();  //Get the selected field;
	DWORD_PTR p = m_ListCtrl.GetItemData(sm);
	ItemData d =this->CurrentField->GetItem(p);    
	if(d.GetValue()==0  )
	{
		Stack.Push(this->CurrentField);
		//this->PreField=this->CurrentField;  
		this->ShowLevel(d.GetLink());
    }
	else 
	{
		CString s;
		s.LoadString(d.GetValue());
        this->ShowInfo(s);
		this->AdditionalTask(d.GetValue());
	}
	*pResult = 0;
}

void CRegConfigDlg::OnEnChangeEdtpath()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CRegConfigDlg::OnEnChangeEdtdata()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CRegConfigDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//=================================
	int sm=m_ListCtrl.GetSelectionMark();  //Get the selected field;
	DWORD_PTR p = m_ListCtrl.GetItemData(sm);
	ItemData d =this->CurrentField->GetItem(p);
	if(d.GetValue()==0  )
	{
		CHackProDlg* MainDlg=(CHackProDlg*) ::AfxGetApp()->m_pMainWnd;
		MainDlg->OutStatus(d.GetCaption());  
			
    }
	else 
	{
		CString s;
		s.LoadString(d.GetValue());
		CString resToken;
		CArray<CString,CString> str; 
		int curPos= 0;
		resToken= s.Tokenize("~",curPos);
		while (resToken != "")
		{
			str.Add(resToken);
			resToken= s.Tokenize("~",curPos);		
		}
		CHackProDlg* Papa=(CHackProDlg*) ::AfxGetApp()->m_pMainWnd;
		Papa->OutStatus(str.GetAt(1));          
	}
	//=================================
	*pResult = 0;
}
