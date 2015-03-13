#ifndef REGDATATREE
#define REGDATATREE
#define LOCATION_ROOT 0
#define IMG_DEFAULT 46
class Level;
#include "stdafx.h"
#include "resource.h"
#include "HackPro.h"
#include "HackProdlg.h"
//=======================================================================================
class ItemData
{
	int  value;//value is the stringtable constant  containg various information about what to do with level;	
	Level* link;//it will be NULL if vlaue has some length;
	int image;//tells which icon will be associated with this Item;
	char ItemCaption[40];//it is the Caption of that item;
public:
	ItemData();
	ItemData(char* caption,int str,int img);//for adding the leaf
	ItemData(char* caption,Level* childlevel,int img);//for adding the link
	~ItemData();
	int GetImage();
	int GetValue();
	Level* GetLink();
	void SetLink(Level* lnk);
	char* GetCaption();

};

class Level
{
	CArray <ItemData,ItemData> Item;
public:
	Level();
	Level(Level &l);
	void AddItem(ItemData d);
	Level* AddItem(char* Caption,int img);
	ItemData GetItem(int i);
	int GetSize();

};
/*
	this class manages the whole tree and its travelsal
	AddLevel:: creates a new level and returns the pointer of newly created level;
	AddData::Adds the new data to the level specified to it;
*/
class RegdataTree
{
	Level *Root;
public:
	RegdataTree();
	~RegdataTree();
	void Del(Level* l);
    void AddItem(ItemData d);//its add at root of the tree
	void AddItem(CString Path,CString Caption,int Info,int img=72);
	Level GetRoot();
	Level* GetRootPtr();

};
#endif