
/////////////////////////////////////////////////////////////////////////////////
//
// Name        : SaveSymbolFileDialogCtrl.h
// Author      : MING
// Created     : 2015Äê8ÔÂ13ÈÕ
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
#include "Symbol.h"

class CfSaveSymbolFileDialogCtrl : public CfDialogBaseCtrl
{
	enum
    {
		ID_BrowseBtn = MENU_ID_NewProject + 1
	};

public:
	CfSaveSymbolFileDialogCtrl(wxWindow* pParent,CfSymbol *symbol,CfDrawingElementGroup* element_group,CfProject *pProject);
	~CfSaveSymbolFileDialogCtrl(void);

	wxString GetSymbolPath() const;

private:
	const wxString CHIP_SYMBOL_FODLER;
	wxTreebook *setting_notebook;
	wxTreeCtrl *m_pTypeTree;
	vector<wxPanel*> m_pPanelList;
	vector<wxCheckListBox*> m_pTemplateList;
	wxCheckListBox *m_pElementListBox;

	CfProject* m_pProject;
	wxTextCtrl *m_pNameCtrl;
	wxTextCtrl *m_pPathCtrl;
	wxButton   *m_pBrowseBtn;
	wxCheckBox *m_pCreateProjectDirCtrl;		

	CfSymbol* m_pSymbol;
	CfDrawingElementGroup* m_pAllElements;
	CfDrawingElementGroup m_ContainedElements;
	wxString m_sPath;
	
	DECLARE_EVENT_TABLE()

	void OnPushOKButton(wxCommandEvent &event);
	void OnPushBrowseButton(wxCommandEvent &event);
	void OnChangePage(wxTreeEvent &event);
	void OnClickRadio(wxCommandEvent &event);

	void ChangePathText();
	bool NewProjectFile(wxString path);
	void NewWorkspaceFile(wxString path);
};


