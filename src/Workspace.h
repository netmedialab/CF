/////////////////////////////////////////////////////////////////////////////////
//
// Name        :fiiProject.h
// Author      :MING
// Created     :2010-01-28 ¿ÀÈÄ 8:11:51
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////


#ifndef CFWORKSPACE_H
#define CFWORKSPACE_H

#include <list>
#include <vector>
#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/treectrl.h> 
#include "Project.h"

using namespace std;

WX_DEFINE_ARRAY_PTR(CfProject*, CFMProjectPTRArray);

class CfWorkspace  
{
public:
	CfWorkspace();
	~CfWorkspace();

	static CfWorkspace* Get();
	static void Init();
	static void Destroy();
	static void SetWorkspace(CfWorkspace* ws);

	bool Load(const wxString& workspace_file);
	void Dump(wxString path=wxEmptyString);
	void Close();

	void ClearWorkspace();
	void AddProject(CfProject* project);
	void DeleteProject(CfProject* project);
	void SetActiveProject(int index);
	void SetAuthor(wxString name);
	void SetDescription(wxString desc);
	void SetVersion(wxString ver);
	void SaveWorkspace();

	int GetActiveProjectIndex();
	int	GetProjectCount();
	int	GetWorkspaceFiles(wxArrayString &files);
	int	FindProject(const wxChar* proj_path);

	CfProject* GetProject(int index);
	CfProject* GetActiveProject();
	CfProject* LoadProject(const wxChar* project_file);

	wxString GetWorkspaceName() const; 
	wxString GetWorkspacePath() const;
	wxString GetWorkspaceDir() const;
	wxString GetAuthor() const;
	wxString GetDescription() const;
	wxString GetWorkspaceVersion() const;
	//<<MING_20110414
	//
	bool Backup(wxString bk_path);
	//>>

	bool SortProject();		//MING_20110810
	
private:
	static CfWorkspace *m_pWorkspace;
	//wxWindow* m_pParent;

	wxString m_sWorkspacePath;
	wxString m_sDescription;
	wxString m_sAuthor;
	wxString m_sWorksapceName;
	wxString m_sWorkspaceVersion;
	
	bool m_bChanged;

	CFMProjectPTRArray m_ProjectArray;
	long m_iActiveProjectIndex;

	bool AskForSave(); // false: cancel
};

#endif
