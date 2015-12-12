/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfNewProjectDialogCtrl.h
// Author      :James
// Created     :2010-03-13 ¿ÀÈÄ 3:00:07
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <wx/treebook.h>
#include <wx/treectrl.h>
#include "CfDialogBaseCtrl.h"
#include "Project.h"
#include "ProjectTypeManager.h"
#include "Workspace.h"

class CfNewProjectDialogCtrl : public CfDialogBaseCtrl, public CfProjectTypeManager
{
	enum
    {
		ID_BrowseBtn = MENU_ID_NewProject + 1
	};

public:
	CfNewProjectDialogCtrl(wxWindow* pParent,wxString &proj_path, wxString &ws_path);
	~CfNewProjectDialogCtrl(void);

private:
	CfProjectTypeManager *m_pProjectTypeMan;
	wxTreebook *setting_notebook;
	wxTreeCtrl *m_pTypeTree;
	vector<wxPanel*> m_pPanelList;
	vector<wxCheckListBox*> m_pTemplateList;
	wxCheckListBox *m_pCurTemplate;

	//CfiiProject* m_pProject;
	wxString &m_ProjectPath;
	wxString &m_WSPath;
	wxString m_sAuthor;
	wxTextCtrl *m_pNameCtrl;
	wxTextCtrl *m_pPathCtrl;
	wxButton   *m_pBrowseBtn;
	wxTextCtrl *m_pTypeDesc;
	wxCheckBox *m_pCreateProjectDirCtrl;		

	//	workspace
	wxTextCtrl *m_pWorkspaceNameCtrl;
	wxTextCtrl *m_pWorkspaceVersion;
	wxCheckBox *m_pCreateWorkspaceDirCtrl;		
	wxRadioButton *m_pCreateNewWSRd, *m_pAddToWSRd;
	

	
	DECLARE_EVENT_TABLE()

	void OnPushOKButton(wxCommandEvent &event);
	void OnPushBrowseButton(wxCommandEvent &event);
	void OnChangePage(wxTreeEvent &event);
	void OnClickRadio(wxCommandEvent &event);

	void ChangePathText();
	bool NewProjectFile(wxString path);
	void NewWorkspaceFile(wxString path);
	

//	void OnPaint(wxPaintEvent& event);
};


