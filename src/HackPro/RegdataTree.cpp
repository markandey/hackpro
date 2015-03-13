#include "StdAfx.h"
#include "regdatatree.h"
char* StrToCharn(CString s)
{
	char *c =new char[s.GetLength()+1];
	strcpy(c,s);
	return c;
}
//============================================================================
//Some icons should be alloted to the some Particular Caption

int GetIconFromName(char* name)
{
	if(strcmp(name,"Desktop")==0)
	{
		return(34); 
	}
	else if(strcmp(name,"Control Panel")==0)
	{
		return(21);
	}
	else if(strcmp(name,"Hardware")==0)
	{
		return(255);
	}
	else if(strcmp(name,"Internet")==0)
	{
		return(220);
	}
	else if(strcmp(name,"Security")==0)
	{
		return(211);
	}
	else if(strcmp(name,"Drive")==0)
	{
		return(124);
	}
	else if(strcmp(name,"Security Policy")==0)
	{
		return(111);
	}
	else if(strcmp(name,"Floppy")==0)
	{
		return(6);
	}
	else if(strcmp(name,"Software & Applications")==0)
	{
		return(162);
	}
	else if(strcmp(name,"Start Menu")==0)
	{
		return(39);
	}
	else if(strcmp(name,"Application")==0)
	{
		return(165);
	}
	else if(strcmp(name,"Privacy")==0)
	{
		return(170);
	}
	else
	{	
		return IMG_DEFAULT;
	}
}
//============================================================================
//===================ItemData=================================================
ItemData::ItemData()//Default constructor
{
	this->value=NULL;
	this->link=NULL; 
	this->image=NULL; 
}

ItemData::ItemData(char* caption,int str,int img)//assignment of a DATA/CONTROL string
{
	value=str;
	this->link=NULL;
	this->image=img;
	strcpy(this->ItemCaption ,caption);
}

ItemData::ItemData(char* caption,Level* childlevel,int img)
{
	this->value=0;
	this->link=childlevel;
	this->image=img;
	strcpy(this->ItemCaption ,caption);
}

ItemData::~ItemData()
{
	
}
int ItemData::GetValue()
{
	return (this->value);
}

Level* ItemData::GetLink()
{
	return (this->link); 
}
void ItemData::SetLink(Level* lnk)
{
	this->link=lnk;
	return;
}
int ItemData::GetImage()
{
	return(this->image); 
}
char* ItemData::GetCaption()
{
	return(this->ItemCaption); 
}

//============================================================================
//=========================Level==============================================
Level::Level()
{
}
Level::Level(Level &l)
{
	this->Item.Copy(l.Item);  
	
}
void Level::AddItem(ItemData d)  
{
	for(int i=0;i<this->GetSize();i++)
	{
		ItemData pd=Item.GetAt(i);
		if(strcmp(pd.GetCaption(),d.GetCaption())==0 )
		{
			return;
		}
	}
	Item.Add(d); 
}
ItemData Level::GetItem(int i)
{
	
	
	return (Item.GetAt(i)); 
		
}
int Level::GetSize()
{
	return(Item.GetCount()); 
}
Level* Level::AddItem(char* Caption,int img)
{
	//get any previously existing item
	for(int i=0;i<this->Item.GetCount();i++)
	{
		if(strcmp((GetItem(i).GetCaption()),Caption)==0)
		{
			
			return(GetItem(i).GetLink());
		}
	}
	Level* l =new Level;
	ItemData d(Caption,l,img);
	Item.Add(d);
	return(l);
}
//===============================REGDATATREE=============================================
void RegdataTree::AddItem(ItemData d)
{
	this->Root->AddItem(d); 
}
Level RegdataTree::GetRoot()
{
	return (*(this->Root));
}
RegdataTree::RegdataTree()
{
	this->Root=new Level; 
}
Level* RegdataTree::GetRootPtr()
{
	return Root;
}
/*======================AddItem(CString Path,CString Caption,CString Info,int img)===========
this is an important fuction to Add the tree with the path velue
Arguments:
CString Path: this is the path of item to be added
			   it sould be of format
		    	eg.software\security\privacy
CString Caption:It is the the Caption to be Addded to the leaf item;
CString Info:: it is the info assiciated to the leaf item;
int img::	it is the image assiciated to the leaf item;
===========================================================================================*/
void RegdataTree::AddItem(CString Path,CString Caption,int Info,int img)
{
	
	CArray<CString,CString> str; 
	{
		CString resToken;
		int curPos= 0;
		resToken= Path.Tokenize("\\",curPos);
		while (resToken != "")
		{
			str.Add(resToken);
			resToken= Path.Tokenize("\\",curPos);		
		}
	}//string path is tokenised in str
	//===========================
		char* cap=new char[Caption.GetLength()+1];
		strcpy(cap,Caption);
		ItemData leaf(cap,Info,img);//creates the requested leaf item
		delete cap;
	//==========================
	char *c=StrToCharn(str.GetAt(0)); 
	Level* l=this->Root->AddItem(c,GetIconFromName(c));
	//GetIconFromName is the mapping fuction to return the icons from name
	delete c;
	for(int i=1;i<str.GetCount();i++)
	{
		char *cc=StrToCharn(str.GetAt(i));
		l=l->AddItem(cc,GetIconFromName(cc)); 
	}
	l->AddItem(leaf);  
	
}
RegdataTree::~RegdataTree()
{
	Del(Root);//Del is recursive function to delete the all dynamicly allocated memory
}
void RegdataTree::Del(Level *l)
{
		if(l==NULL)
		{
			return;
		}
		for(int i=0;i<l->GetSize();i++)
		{
			ItemData tempitem=l->GetItem(i);
			Del(tempitem.GetLink());
			delete l;
		}
}
