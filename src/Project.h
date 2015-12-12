/////////////////////////////////////////////////////////////////////////////////
//
// Name        :fiiProject.h
// Author      :MING
// Created     :2010-01-28 ¿ÀÈÄ 8:11:51
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////


#ifndef CFPROJECT_H
#define CFPROJECT_H

#include <list>
#include <vector>
#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/treectrl.h> 
#include "EnvironmentUtil.h"
#include "CfWx.h"



using namespace std;

#define PROJ_TYPE	wxT("ProjectType")
#define PROJ_AUTHOR	wxT("Author")

#define WS_DESCRIPT		wxT("Description")
#define WS_AUTHOR		wxT("Author")
#define WS_ACTIVEPROJ	wxT("ActiveProject")
#define WS_PROJFILE		wxT("ProjectFile")
#define WS_VERSION		wxT("Version")

static const wxChar *ItemTypeStr[] = 
{
	 wxT("WorkSpace")
	,wxT("Project")
	,wxT("Folder")
	,wxT("Classifier")
	,wxT("File")
};

#define USE_FOLDER

class CfProject;
class CfProjectItem;
typedef vector<CfProjectItem*> CProjectItemList;

const enum CfItemType
{

	PIT_WORKSPACE
	,PIT_RPOJECT 
	,PIT_FOLDER	
	,PIT_CLASSIFIER 
	,PIT_FILE	
	,PIT_EXTERN_FILE    // the file opened not from project tree
	,OPEN_DIR			// a state of dir
	,PIT_UNKNOWN
};

class CfiiFileItem
{
public:
	wxString Path;
	CfItemType Type;
	//wxWindow* PageWnd;
	CfiiFileItem(wxString sPath,CfItemType iType):Path(sPath),Type(iType)/*,PageWnd(NULL)*/{};
	const wxString GetItemName(){return Path.AfterLast('\\');};
	const wxString GetDirectory(){return Path.BeforeLast('\\');};
};

class CfProjectItem : public CfiiFileItem, public wxTreeItemData
{
public:
	CfProjectItem(wxString sPath,CfItemType iType,wxString sDepth,CfProject *project);
	CfProjectItem(CfProjectItem *item);
	~CfProjectItem();
	bool AddChild(CfProjectItem *child);
	bool DelChild(CfProjectItem *item);
	void Sort();
	bool ChildExist(CfProjectItem *child);
	void DumpItem(wxTextFile* pro_file,wxString depth);
	void BackupItem(wxTextFile* pro_file,wxString depth,wxString bk_path);//<<MING_20110414
	CProjectItemList* GetChildList();
	CfProjectItem* FindItem(const wxString name,bool full_path=true);
	virtual bool operator ==(const CfProjectItem &item) const
	{
		return ((this->Path == item.Path) && (this->Depth == item.Depth) && (this->Type == item.Type));
	}

	virtual bool operator < (const CfProjectItem &item) const
	{
		if (this->Type == item.Type) 
			return (Path.AfterLast('\\') < item.Path.AfterLast('\\'));
		return (this->Type < item.Type);
	}
	int GetFileCount(); // To get the count of files, not include folders and classifiers

public:
	wxString Depth;
	CfProjectItem *pParent;
	CProjectItemList *pChildList;
	CfProject *ProjectPtr;
};

class CfProject
{
public:
	CfProject();
	//CfProject(wxWindow *parent);
	//CfProject(wxWindow *parent, const wxString& project_file);
	~CfProject(void);

	bool Load(const wxString& project_file);
	void Dump(wxString path=wxEmptyString);
	void Clear();
	void Init();
	void Sort();

	void AskForSave();
	wxString GetProjectName();
	wxString GetDirectory();
	wxString GetProjectPath();
	wxString GetProjectTypeString();
	CProjectItemList* GetProjItemList();
	CfProjectItem *GetRootItem();
	wxTreeItemId GetTreeItemId();
	void UnsetTreeItemId();

	void GetProjectFiles(wxArrayString &files);

	bool IsEmpty();
	int GetFileCount(); // To get the count of files, not include folders and classifiers

	void SetProjectType(wxString project_type);
	void SetProjectPath(wxString project_path);
	void SetWorkspaceExplorerItemId(wxTreeItemId tid);
	void SetChanged(bool changed);
	wxString GetAuthor();
	void SetAuthor(wxString name);

	CfProjectItem* AddProjectItem(CfProjectItem *parent,wxString name,CfItemType type);
	void DelProjectItem(CfProjectItem *item);
	CfProjectItem* FindItem(const wxString name);
	bool RenameFolder(CfProjectItem *item, wxString new_name);
	//<<MING_20110414
	//
	bool Backup(wxString bk_path);
	//>>
	
private:
	wxWindow	*m_pParentWin;
	CfProjectItem *m_pProRootItem;
	CProjectItemList m_lProjItemList;
	wxString	m_sProjectPath;
	wxString	m_sAuthor;
	bool		m_bChanged;

	wxString	m_sProjectTypeString;
	wxTreeItemId m_TreeNodeId;  // the tree item id in project tree

};

#endif
