/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfNewProjectDialogCtrl.cpp
// Author      :James
// Created     :2010-03-13 ¿ÀÈÄ 4:10:29
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#include <wx/sizer.h>
#include <wx/statline.h>
#include "CfDefines.h"
#include "CfUtils.h"
#include "FileUtil.h"
#include "NewProjectDialogCtrl.h"
#include "LogMessageCtrl.h"

BEGIN_EVENT_TABLE(CfNewProjectDialogCtrl,CfDialogBaseCtrl)

EVT_BUTTON(ID_BrowseBtn,CfNewProjectDialogCtrl::OnPushBrowseButton)
EVT_BUTTON(wxID_OK,CfNewProjectDialogCtrl::OnPushOKButton)
//EVT_TREEBOOK_PAGE_CHANGED(wxID_ANY, CfNewProjectDialogCtrl::OnChangePage)
EVT_TREE_SEL_CHANGED(wxID_ANY, CfNewProjectDialogCtrl::OnChangePage)
EVT_RADIOBUTTON(wxID_ANY,CfNewProjectDialogCtrl::OnClickRadio)
END_EVENT_TABLE()


CfNewProjectDialogCtrl::CfNewProjectDialogCtrl(wxWindow* pParent,wxString &proj_path, wxString &ws_path)
:CfDialogBaseCtrl(pParent,_("New Project"),wxSize(560,480),_("Create a new project"))//,m_pProject(pProject)
,CfProjectTypeManager(),m_pCurTemplate(NULL)
,m_ProjectPath(proj_path),m_WSPath(ws_path)
{
        wxBoxSizer* s0 = new wxBoxSizer(wxHORIZONTAL);
		s0->Add(10, 1);
		s0->Add(new wxStaticText(this, wxID_ANY, wxT("Project Types:")));
		//s0->Add(180, 1);
		s0->Add(133, 1);//cheng modify it
		s0->Add(new wxStaticText(this, wxID_ANY, wxT("Templates:")));

		m_pTypeDesc = new wxTextCtrl(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(510,25),wxTE_READONLY);
		m_pTypeDesc->Enable(false);

		m_pTypeTree = new wxTreeCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(215,150),wxTR_HIDE_ROOT|wxTR_LINES_AT_ROOT|wxTR_HAS_BUTTONS);
		wxTreeItemId parent_node= m_pTypeTree->AddRoot(wxEmptyString);

		wxPanel *templ = new wxPanel(this);


		//setting_notebook = new wxTreebook(this,wxID_ANY,wxDefaultPosition,wxSize(400,300));
		wxArrayString *type_list = GetTypeList();
		int num = type_list->GetCount();
		//int w,h,w_ws;
  //      wxClientDC dc(this);
		//dc.GetTextExtent(_(" "),&w_ws,&h);  // get width of a whitespace

		for(int i=0;i<num;i++)
		{
			//m_pPanelList.push_back(new wxPanel(setting_notebook,wxID_ANY,0,0,200,200));
			wxString str(type_list->Item(i));
			//dc.GetTextExtent(str,&w,&h);
			//int num = 30 - w/w_ws; // append whitespace to the end of type name to make it equals to 30 whitespace length
			//str.Append(32,num);  
			//setting_notebook->AddPage(m_pPanelList[i],str);
			//
			wxTreeItemId item = m_pTypeTree->AppendItem(parent_node,str);
			wxArrayString classifiers;
			GetTypeTemplateList(i,&classifiers);
			wxCheckListBox *temp = new wxCheckListBox(templ,wxID_ANY,wxDefaultPosition,wxSize(290,150));
			temp->Append(classifiers);
			for(int ii=0;ii<classifiers.GetCount();ii++)
				temp->Check(ii);
			m_pTemplateList.push_back(temp);
			temp->Show(i==0);
			if(i==0) 
			{
				m_pCurTemplate = temp;
				m_pTypeTree->SelectItem(item);
			}

		}
        wxBoxSizer* s_type = new wxBoxSizer(wxHORIZONTAL);
		s_type->Add(m_pTypeTree,0,wxALL,5);
		s_type->Add(templ,0,wxTOP,5);
//		s_note->Add(setting_notebook,1,wxEXPAND|wxALL,5);


        wxBoxSizer* s1 = new wxBoxSizer(wxHORIZONTAL);
		m_pNameCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,wxDefaultPosition, wxSize(220,20));
		m_pCreateProjectDirCtrl = new wxCheckBox(this, wxID_ANY, wxT("Create directory for project"), wxDefaultPosition);
		m_pCreateProjectDirCtrl->SetValue(false);
        s1->Add(10, 1, 1, wxEXPAND);
        s1->Add(new wxStaticText(this, wxID_ANY, wxT("Project Name:")));
        s1->Add(m_pNameCtrl);
        s1->Add(5, 5, 1, wxEXPAND);
        s1->Add(m_pCreateProjectDirCtrl);
        s1->Add(1, 1, 1, wxEXPAND);
        s1->SetItemMinSize((size_t)1, 120, 20);

  //      wxBoxSizer* s3 = new wxBoxSizer(wxHORIZONTAL);
		//m_pTypeCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(160,20));
  //      s3->Add(10, 1, 1, wxEXPAND);
  //      s3->Add(new wxStaticText(this, wxID_ANY, wxT("Project Type:")));
  //      s3->Add(m_pTypeCtrl);
  //      s3->Add(1, 1, 1, wxEXPAND);
  //      s3->SetItemMinSize((size_t)1, 100, 20);


        wxBoxSizer* s2 = new wxBoxSizer(wxHORIZONTAL);
		m_pPathCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(390,20));
		m_pBrowseBtn = new wxButton(this, ID_BrowseBtn, wxT("..."),wxDefaultPosition, wxSize(20,20));
        s2->Add(10, 1, 1, wxEXPAND);
        s2->Add(new wxStaticText(this, wxID_ANY, wxT("Project Path:")));
        s2->Add(m_pPathCtrl);
        s2->Add(m_pBrowseBtn);
        s2->Add(1, 1, 1, wxEXPAND);
        s2->SetItemMinSize((size_t)1, 120, 20);

		m_pAddToWSRd = new wxRadioButton( this, wxID_ANY, _("Add to current Workspace"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);;
		m_pCreateNewWSRd = new wxRadioButton( this, wxID_ANY, _("Create new Workspace"), wxDefaultPosition, wxDefaultSize, 0);
		wxBoxSizer* s3 = new wxBoxSizer(wxHORIZONTAL);
        s3->Add(10, 1, 1, wxEXPAND);
        s3->Add(new wxStaticText(this, wxID_ANY, wxT("Workspace:")));
        s3->Add(m_pAddToWSRd);
        s3->Add(5, 5, 1, wxEXPAND);
        s3->Add(m_pCreateNewWSRd);
        s3->Add(1, 1, 1, wxEXPAND);
        s3->SetItemMinSize((size_t)1, 120, 20);



		m_pWorkspaceNameCtrl = new wxTextCtrl(this,wxID_ANY, wxEmptyString,wxDefaultPosition, wxSize(210,20));
		m_pCreateWorkspaceDirCtrl = new wxCheckBox(this, wxID_ANY, wxT("Create directory for workspace"), wxDefaultPosition);
		m_pCreateWorkspaceDirCtrl->SetValue(false);
		wxBoxSizer* s4 = new wxBoxSizer(wxHORIZONTAL);
        s4->Add(10, 1, 1, wxEXPAND);
        s4->Add(new wxStaticText(this, wxID_ANY, wxT("Workspace Name:")));
        s4->Add(m_pWorkspaceNameCtrl);
        s4->Add(5, 5, 1, wxEXPAND);
        s4->Add(m_pCreateWorkspaceDirCtrl);
        s4->Add(1, 1, 1, wxEXPAND);
        s4->SetItemMinSize((size_t)1, 120, 20);


        wxBoxSizer* s_btn = new wxBoxSizer(wxHORIZONTAL);
		s_btn->Add(new wxButton(this, wxID_OK, wxT("OK"),wxDefaultPosition, wxSize(80,25)));
        s_btn->Add(10, 1, 1, wxEXPAND);
		s_btn->Add(new wxButton(this, wxID_CANCEL, wxT("Cancel"),wxDefaultPosition, wxSize(80,25)));


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
		s_all->Add(10, 15, 0);
		s_all->Add(s3, 0, wxLEFT, 5);
		s_all->Add(10, 10, 0);
		s_all->Add(s4, 0, wxLEFT, 5);
		s_all->Add(10, 10, 0);
		s_all->Add(new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxSize(540,2), wxLI_HORIZONTAL),0,wxLEFT,5);
		s_all->Add(10, 10, 0);
		s_all->Add(s_btn, 0,wxCENTER); 
		s_all->Add(10, 10, 0);

		SetSizer(s_all);
		//s_all->Fit(this);

		ChangePathText();
}

CfNewProjectDialogCtrl::~CfNewProjectDialogCtrl(void)
{
}

void CfNewProjectDialogCtrl::OnPushOKButton(wxCommandEvent &event)// cheng modify it 20120320

/////////////////////////////////////////////////////////
////wxMkdir: when we use it we should take care that the  parent directory must be exist.
//////////////////////////////////////////////////////////////////
{
	wxString ws_name;wxString ws_dir1;
	wxString name = m_pNameCtrl->GetValue();
	if(name.IsEmpty()) {m_pNameCtrl->SetFocus();return;}
	wxString ws_dir = m_pPathCtrl->GetValue();
	if(ws_dir.IsEmpty()) {m_pPathCtrl->SetFocus();return;}

	if(ws_dir.EndsWith(_("\\"))) ws_dir = ws_dir.BeforeLast('\\');

	// Create dir for workspace if required
	if(m_pCreateNewWSRd->GetValue() ) 
	{
		ws_name = m_pWorkspaceNameCtrl->GetValue();
		if(ws_name.IsEmpty()) {m_pWorkspaceNameCtrl->SetFocus();return;}

		if(m_pCreateWorkspaceDirCtrl->IsChecked())
		{
			wxStringList dir;
			if(dir.IsEmpty()) {dir.clear();}
			ws_dir1 = ws_dir;
			if((ws_dir1.Find('\\')==wxNOT_FOUND) || ws_dir1.Find('\\')<2)
				{
					wxString msg1;
					msg1.Printf(wxT("Wrong directory  Check your directory '%s'"),ws_dir1.c_str());
					wxMessageDialog dlg(this,msg1,PROJECT_CAPTION , wxOK | wxCANCEL | wxICON_QUESTION);
					int iid = dlg.ShowModal();
					if(iid == wxID_CANCEL) return;
				}
			else 
			{
			wxString ws_dir2 = ws_dir1.BeforeLast('\\');
			while(!wxDirExists(ws_dir2.c_str()))
			{
				dir.Add(ws_dir2);
				ws_dir2 = ws_dir2.BeforeLast('\\');
				
			}
			int j = dir.GetCount();
			for(int i=1;i<=j;i++)
			{
				ws_dir2 = dir[j-i];
			    wxMkdir(ws_dir2);
			}
			if(!wxDirExists(ws_dir1.c_str()))// make the parent directory  cheng add it
			{
				wxMkdir(ws_dir1);
			}

			ws_dir = ws_dir + _("\\") + ws_name;
			
			//Sleep(10);
			if(wxDirExists(ws_dir.c_str())) 
			{
				wxString msg;
				msg.Printf(wxT("Directory '%s' already exists, continue?"),ws_dir.c_str());
				wxMessageDialog dlg(this,msg,PROJECT_CAPTION , wxOK | wxCANCEL | wxICON_QUESTION);
				int iid = dlg.ShowModal();
				if(iid == wxID_CANCEL) return;
			}
			else
			{
				wxMkdir(ws_dir.c_str());
			}
			}
		}
	}
	if(!wxDirExists(ws_dir.c_str())) 
	{
		wxString msg;
		msg.Printf(wxT("Directory '%s' doesn't exists!"),ws_dir.c_str());
		ShowWarningDialog(msg.c_str(),this);
		m_pPathCtrl->SetFocus();
		return;
	}

	wxString proj_dir = ws_dir;
	//Create dir for project
	if(m_pCreateProjectDirCtrl->IsChecked())
	{
		proj_dir = proj_dir + _("\\") + name;
		if(wxDirExists(proj_dir.c_str())) 
		{
			wxString msg;
			msg.Printf(wxT("Directory '%s' already exists, continue?"),proj_dir.c_str());
			wxMessageDialog dlg(this,msg,PROJECT_CAPTION , wxOK | wxCANCEL | wxICON_QUESTION);
			int iid = dlg.ShowModal();
			if(iid == wxID_CANCEL) return;
		}
		else
		{
			wxMkdir(proj_dir);
		}
	}

	if(m_pCreateNewWSRd->GetValue())
	{
		wxString ws_path;
		ws_path.Printf(_("%s\\%s.flw"),ws_dir, ws_name);	
		NewWorkspaceFile(ws_path);
	}

	wxString path;
	path.Printf(_("%s\\%s.flp"),proj_dir, name);
	if(!NewProjectFile(path)) return;
	event.Skip();
}

/*void CfNewProjectDialogCtrl::OnPushOKButton(wxCommandEvent &event)
{
	wxString ws_name;
	wxString name = m_pNameCtrl->GetValue();
	if(name.IsEmpty()) {m_pNameCtrl->SetFocus();return;}
	wxString ws_dir = m_pPathCtrl->GetValue();
	if(ws_dir.IsEmpty()) {m_pPathCtrl->SetFocus();return;}
	if(!wxDirExists(ws_dir.c_str())) 
	{
		wxString msg;
		msg.Printf(wxT("Directory '%s' doesn't exists!"),ws_dir.c_str());
		ShowWarningDialog(msg.c_str(),this);
		m_pPathCtrl->SetFocus();
		return;
	}

	if(ws_dir.EndsWith(_("\\"))) ws_dir = ws_dir.BeforeLast('\\');

	// Create dir for workspace if required
	if(m_pCreateNewWSRd->GetValue()) 
	{
		ws_name = m_pWorkspaceNameCtrl->GetValue();
		if(ws_name.IsEmpty()) {m_pWorkspaceNameCtrl->SetFocus();return;}

		if(m_pCreateWorkspaceDirCtrl->IsChecked())
		{
			ws_dir = ws_dir + _("\\") + ws_name;
			if(wxDirExists(ws_dir.c_str())) 
			{
				wxString msg;
				msg.Printf(wxT("Directory '%s' already exists, continue?"),ws_dir.c_str());
				wxMessageDialog dlg(this,msg,PROJECT_CAPTION , wxOK | wxCANCEL | wxICON_QUESTION);
				int iid = dlg.ShowModal();
				if(iid == wxID_CANCEL) return;
			}
			else
			{
				wxMkdir(ws_dir);
			}
		}
	}

	wxString proj_dir = ws_dir;
	//Create dir for project
	if(m_pCreateProjectDirCtrl->IsChecked())
	{
		proj_dir = proj_dir + _("\\") + name;
		if(wxDirExists(proj_dir.c_str())) 
		{
			wxString msg;
			msg.Printf(wxT("Directory '%s' already exists, continue?"),proj_dir.c_str());
			wxMessageDialog dlg(this,msg,PROJECT_CAPTION , wxOK | wxCANCEL | wxICON_QUESTION);
			int iid = dlg.ShowModal();
			if(iid == wxID_CANCEL) return;
		}
		else
		{
			wxMkdir(proj_dir);
		}
	}

	if(m_pCreateNewWSRd->GetValue())
	{
		wxString ws_path;
		ws_path.Printf(_("%s\\%s.flw"),ws_dir, ws_name);	
		NewWorkspaceFile(ws_path);
	}

	wxString path;
	path.Printf(_("%s\\%s.flp"),proj_dir, name);
	if(!NewProjectFile(path)) return;
	event.Skip();
}*/

void CfNewProjectDialogCtrl::OnPushBrowseButton(wxCommandEvent &event)
{
	wxDirDialog dialog(this, _T("Specify a directory"),wxGetCwd(),wxDD_DEFAULT_STYLE | wxDD_CHANGE_DIR);

    //dialog.SetFilterIndex(1);

    if (dialog.ShowModal() == wxID_OK)
    {		
		m_pPathCtrl->SetValue(dialog.GetPath());
	}
}


void CfNewProjectDialogCtrl::OnChangePage(wxTreeEvent &event)
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


void CfNewProjectDialogCtrl::OnClickRadio(wxCommandEvent &event)
{
	ChangePathText();
}

void CfNewProjectDialogCtrl::ChangePathText()
{
	CfWorkspace *ws = CfWorkspace::Get();
	if(!ws || ws->GetWorkspacePath().IsEmpty())
	{
		FileUtil fu;
		wxString proj_dir = fu.GetDirectory(ID_PROJDIR);
		m_pPathCtrl->SetValue(proj_dir.BeforeLast('\\'));
		m_pWorkspaceNameCtrl->Enable(true);
		m_pCreateWorkspaceDirCtrl->Enable(true);
		m_pAddToWSRd->Enable(false);
		m_pCreateNewWSRd->SetValue(true);

	}
	else
	{
		m_pAddToWSRd->Enable(true);
		int v = m_pAddToWSRd->GetValue();
		if(v>0) 
		{
			wxString proj_dir(ws->GetWorkspacePath().c_str());
			m_pPathCtrl->SetValue(proj_dir.BeforeLast('\\'));
			m_pWorkspaceNameCtrl->Enable(false);
			m_pCreateWorkspaceDirCtrl->Enable(false);

		}
		else
		{
			FileUtil fu;
			wxString proj_dir = fu.GetDirectory(ID_PROJDIR);
			m_pPathCtrl->SetValue(proj_dir.BeforeLast('\\'));
			m_pWorkspaceNameCtrl->Enable(true);
			m_pCreateWorkspaceDirCtrl->Enable(true);
		}
		
	}

}

bool CfNewProjectDialogCtrl::NewProjectFile(wxString path)
{
	wxTextFile fProjectFile(path);
	if(fProjectFile.Exists()) 
	{
		wxString msg;
		msg.Printf(wxT("Project file %s already exists"),m_pNameCtrl->GetValue());
		ShowErrorDialog(msg);
		return false;
	}
	else fProjectFile.Create();
	wxLogMessage("[File] New project "+m_pNameCtrl->GetValue());

	wxString strLine;
	
	// get selected project type
	wxTreeItemId id = m_pTypeTree->GetSelection();
	wxString text = m_pTypeTree->GetItemText(id);
	strLine.Printf(wxT("%s=%s"),PROJ_TYPE,text);
	fProjectFile.AddLine(strLine);

	strLine.Printf(wxT("%s=%s"),PROJ_AUTHOR,m_sAuthor);
	fProjectFile.AddLine(strLine);

	// Add classifiers to project
	wxArrayString *type_list = GetTypeList();
	wxArrayString classi_list;
	int idx = type_list->Index(text.c_str());
	if(idx < 0) 
	{		
		wxLogWarning(wxT("Unknown project type %s"),text.c_str());
		fProjectFile.Close();
		return true;
	}
	GetTypeTemplateList(idx,&classi_list);
	
	for(int i=0;i<classi_list.GetCount();i++)
	{
		if(!m_pCurTemplate->IsChecked(i)) continue;
		strLine.Printf(wxT("%s=0.%d=%s"),ItemTypeStr[PIT_CLASSIFIER],i,classi_list[i]);
		fProjectFile.AddLine(strLine);
	}
	fProjectFile.Write();
	fProjectFile.Close();

	m_ProjectPath = path;
	return true;
}
void CfNewProjectDialogCtrl::NewWorkspaceFile(wxString path)
{
	wxTextFile ws_file(path);
	if(ws_file.Exists()) 
	{
		wxString msg;
		msg.Printf(wxT("Workspace file %s already exists"),m_pWorkspaceNameCtrl->GetValue());
		ShowErrorDialog(msg);
		return;
	}
	else ws_file.Create();
	m_WSPath = path;

	wxString strLine;
	//strLine.Printf(wxT("%s=%s"),WS_AUTHOR,m_pAuthorCtrl->GetValue());
	//ws_file.AddLine(strLine);
	//strLine.Printf(wxT("%s=%s"),WS_DESCRIPT,m_pDescriptCtrl->GetValue());
	//ws_file.AddLine(strLine);
	strLine.Printf(wxT("%s=%s"),WS_AUTHOR,m_sAuthor);
	ws_file.AddLine(strLine);

	wxDateTime dt = wxDateTime::Now();
	strLine.Printf(wxT("%s=%s"),WS_VERSION,dt.Format(_("%Y%m%d%H%M%S")));
	ws_file.AddLine(strLine);

	ws_file.Write();
	ws_file.Close();
	wxLogMessage("[File] New workspace  "+m_pWorkspaceNameCtrl->GetValue());

}
