/////////////////////////////////////////////////////////////////////////////////
//
// Name        : NewFileDialogCtrl.h
// Author      : MING
// Created     : 2015Äê9ÔÂ12ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <wx/treebook.h>
#include <wx/treectrl.h>
#include "CfDialogBaseCtrl.h"
#include "Project.h"
#include "FileTypeManager.h"
#include "Workspace.h"

class CfNewFileDialogCtrl : public CfDialogBaseCtrl, public CfFileTypeManager
{
	enum
    {
		ID_BrowseBtn = MENU_ID_NewProject + 1
	};

public:
	CfNewFileDialogCtrl(wxWindow* pParent);
	~CfNewFileDialogCtrl(void);

	wxString GetFilePath() const;

private:
	wxString m_sFilePath;
	CfFileTypeManager *m_pFileTypeMan;
	wxTreebook *setting_notebook;
	wxTreeCtrl *m_pTypeTree;
	vector<wxPanel*> m_pPanelList;
	vector<wxListBox*> m_pTemplateList;
	wxListBox *m_pCurTemplate;

	//CfiiProject* m_pProject;
	wxString m_sAuthor;
	wxTextCtrl *m_pNameCtrl;
	wxTextCtrl *m_pPathCtrl;
	wxButton   *m_pBrowseBtn;
	wxTextCtrl *m_pTypeDesc;
	wxCheckBox *m_pCreateProjectDirCtrl;		
	wxStaticText *m_pFileExtensionCtrl;

	

	
	DECLARE_EVENT_TABLE()

	void OnPushOKButton(wxCommandEvent &event);
	void OnPushBrowseButton(wxCommandEvent &event);
	void OnChangePage(wxTreeEvent &event);
	void OnClickListBox(wxCommandEvent &event);

	void ChangePathText();
	
};


