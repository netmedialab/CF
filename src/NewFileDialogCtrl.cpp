/////////////////////////////////////////////////////////////////////////////////
//
// Name        : NewFileDialogCtrl.cpp
// Author      : MING
// Created     : 2015Äê9ÔÂ12ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////

#include <wx/sizer.h>
#include <wx/statline.h>
#include "CfDefines.h"
#include "CfUtils.h"
#include "FileUtil.h"
#include "NewFileDialogCtrl.h"
#include "LogMessageCtrl.h"

BEGIN_EVENT_TABLE(CfNewFileDialogCtrl,CfDialogBaseCtrl)

	EVT_BUTTON(ID_BrowseBtn,CfNewFileDialogCtrl::OnPushBrowseButton)
	EVT_BUTTON(wxID_OK,CfNewFileDialogCtrl::OnPushOKButton)
	EVT_LISTBOX(wxID_ANY, CfNewFileDialogCtrl::OnClickListBox)
	//EVT_TREEBOOK_PAGE_CHANGED(wxID_ANY, CfNewFileDialogCtrl::OnChangePage)
	EVT_TREE_SEL_CHANGED(wxID_ANY, CfNewFileDialogCtrl::OnChangePage)
END_EVENT_TABLE()


CfNewFileDialogCtrl::CfNewFileDialogCtrl(wxWindow* pParent)
:CfDialogBaseCtrl(pParent,_("New File"),wxSize(560,480),_("Create a new file"))//,m_pProject(pProject)
,m_pFileTypeMan(),m_pCurTemplate(NULL)
{
        wxBoxSizer* s0 = new wxBoxSizer(wxHORIZONTAL);
		s0->Add(10, 1);
		s0->Add(new wxStaticText(this, wxID_ANY, wxT("File Types:")));
		//s0->Add(180, 1);
		s0->Add(133, 1);//cheng modify it
		s0->Add(new wxStaticText(this, wxID_ANY, wxT("Templates:")));

		m_pTypeDesc = new wxTextCtrl(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(510,30),wxTE_READONLY);
		m_pTypeDesc->Enable(false);

		m_pTypeTree = new wxTreeCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(215,150),wxTR_HIDE_ROOT|wxTR_LINES_AT_ROOT|wxTR_HAS_BUTTONS);
		wxTreeItemId parent_node= m_pTypeTree->AddRoot(wxEmptyString);

		wxPanel *templ = new wxPanel(this);
		wxArrayString *type_list = GetTypeList();
		int num = type_list->GetCount();
		wxString default_extension;


		for(int i=0;i<num;i++)
		{
			wxString str(type_list->Item(i));
			wxTreeItemId item = m_pTypeTree->AppendItem(parent_node,str);
			wxArrayString classifiers;
			GetTypeTemplateList(i,&classifiers);
			wxListBox *temp = new wxListBox(templ,wxID_ANY,wxDefaultPosition,wxSize(290,150));
			temp->Append(classifiers);
			m_pTemplateList.push_back(temp);
			temp->Show(i==0);
			if(i==0) 
			{
				m_pCurTemplate = temp;
				default_extension = temp->GetString(0).AfterLast('*').BeforeLast(')');
				temp->SetSelection(0);
				m_pTypeTree->SelectItem(item);
			}

		}
        wxBoxSizer* s_type = new wxBoxSizer(wxHORIZONTAL);
		s_type->Add(m_pTypeTree,0,wxALL,5);
		s_type->Add(templ,0,wxTOP,5);
//		s_note->Add(setting_notebook,1,wxEXPAND|wxALL,5);


        wxBoxSizer* s1 = new wxBoxSizer(wxHORIZONTAL);
		m_pNameCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,wxDefaultPosition, wxSize(220,25));
		m_pFileExtensionCtrl = new wxStaticText(this,wxID_ANY,default_extension,wxDefaultPosition,wxSize(50,25));
		//m_pCreateProjectDirCtrl = new wxCheckBox(this, wxID_ANY, wxT("Create directory for project"), wxDefaultPosition);
		//m_pCreateProjectDirCtrl->SetValue(false);
        s1->Add(10, 1, 1, wxEXPAND);
        s1->Add(new wxStaticText(this, wxID_ANY, wxT("File Name:")));
        s1->Add(m_pNameCtrl);
        s1->Add(1, 1, 1, wxEXPAND);
		s1->Add(m_pFileExtensionCtrl);
        s1->Add(5, 5, 1, wxEXPAND);
        //s1->Add(m_pCreateProjectDirCtrl);
        s1->Add(1, 1, 1, wxEXPAND);
        s1->SetItemMinSize((size_t)1, 100, 25);

        wxBoxSizer* s2 = new wxBoxSizer(wxHORIZONTAL);
		m_pPathCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(390,30));
		m_pBrowseBtn = new wxButton(this, ID_BrowseBtn, wxT("..."),wxDefaultPosition, wxSize(20,30));
        s2->Add(10, 1, 1, wxEXPAND);
        s2->Add(new wxStaticText(this, wxID_ANY, wxT("File Path:")));
        s2->Add(m_pPathCtrl);
        s2->Add(m_pBrowseBtn);
        s2->Add(1, 1, 1, wxEXPAND);
        s2->SetItemMinSize((size_t)1, 100, 30);

        wxBoxSizer* s_btn = new wxBoxSizer(wxHORIZONTAL);
		s_btn->Add(new wxButton(this, wxID_OK, wxT("OK"),wxDefaultPosition, wxSize(80,30)));
        s_btn->Add(10, 1, 1, wxEXPAND);
		s_btn->Add(new wxButton(this, wxID_CANCEL, wxT("Cancel"),wxDefaultPosition, wxSize(80,30)));


		wxBoxSizer* s_all = new wxBoxSizer(wxVERTICAL);
		//s_all->Add(header_img,0);
		s_all->Add(10, 70, 0);
		s_all->Add(s0, 0, wxLEFT,5);
		s_all->Add(s_type, 0, wxLEFT,10);
		s_all->Add(m_pTypeDesc,0,wxLEFT,15);
		s_all->Add(10, 10, 0);
		s_all->Add(s1, 0, wxLEFT,5);
		s_all->Add(10, 10, 0);
		s_all->Add(s2, 0, wxLEFT, 5); 
		s_all->Add(10, 10, 0);
		s_all->Add(new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxSize(540,2), wxLI_HORIZONTAL),0,wxLEFT,5);
		s_all->Add(10, 10, 0);
		s_all->Add(s_btn, 0,wxCENTER); 
		s_all->Add(10, 10, 0);

		SetSizer(s_all);
		//s_all->Fit(this);

		ChangePathText();
		m_pNameCtrl->SetFocus();
}

CfNewFileDialogCtrl::~CfNewFileDialogCtrl(void)
{
}

wxString CfNewFileDialogCtrl::GetFilePath() const
{
	return m_sFilePath;
}

void CfNewFileDialogCtrl::OnPushOKButton(wxCommandEvent &event)// cheng modify it 20120320

/////////////////////////////////////////////////////////
////wxMkdir: when we use it we should take care that the  parent directory must be exist.
//////////////////////////////////////////////////////////////////
{
	wxString name = m_pNameCtrl->GetValue();
	wxString ext = m_pFileExtensionCtrl->GetLabel();

	if(name.IsEmpty()) {m_pNameCtrl->SetFocus();return;}
	wxString dir = m_pPathCtrl->GetValue();
	if(dir.IsEmpty()) {m_pPathCtrl->SetFocus();return;}

	m_sFilePath = dir+_("\\")+name+ext;
	if(wxFileExists(m_sFilePath))
	{
		wxString msg;
		msg.Printf(wxT("File exists, overwrite it? \n '%s'"),m_sFilePath.c_str());
		wxMessageDialog dlg(this,msg,PROJECT_CAPTION , wxOK | wxCANCEL | wxICON_QUESTION);
		int iid = dlg.ShowModal();
		if(iid == wxID_CANCEL) return;
		wxRemoveFile(m_sFilePath);
	}

	event.Skip();
}

void CfNewFileDialogCtrl::OnPushBrowseButton(wxCommandEvent &event)
{
	wxDirDialog dialog(this, _T("Specify a directory"),wxGetCwd(),wxDD_DEFAULT_STYLE | wxDD_CHANGE_DIR);

    //dialog.SetFilterIndex(1);

    if (dialog.ShowModal() == wxID_OK)
    {		
		m_pPathCtrl->SetValue(dialog.GetPath());
	}
}


void CfNewFileDialogCtrl::OnChangePage(wxTreeEvent &event)
{
	wxTreeItemId id = event.GetItem();
	wxString text = m_pTypeTree->GetItemText(id);
	wxArrayString *list = GetTypeList();
	int idx = list->Index(text.c_str());
	if(idx < 0) return;
	wxASSERT(idx >-1);
	m_pTypeDesc->SetLabel(GetTypeDescriptionList()->Item(idx));
	if(m_pCurTemplate) m_pCurTemplate->Show(false);
	m_pCurTemplate = m_pTemplateList[idx];
	m_pCurTemplate->Show(true);

}


void CfNewFileDialogCtrl::OnClickListBox(wxCommandEvent &event)
{
	int idx = m_pCurTemplate->GetSelection();
	if(idx == wxNOT_FOUND) return;
	wxString ext = m_pCurTemplate->GetString(idx);
	m_pFileExtensionCtrl->SetLabel(ext.AfterLast('*').BeforeLast(')'));
	m_pNameCtrl->SetSelection(-1,-1);
	m_pNameCtrl->SetFocus();
}

void CfNewFileDialogCtrl::ChangePathText()
{
	CfWorkspace *ws = CfWorkspace::Get();
	CfProject *project = NULL;
	if(!ws || (project = ws->GetActiveProject()))
	{
		wxString proj_dir = project->GetProjectPath();
		m_pPathCtrl->SetValue(proj_dir.BeforeLast('\\'));
	}


}

