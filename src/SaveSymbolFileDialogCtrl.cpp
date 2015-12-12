
/////////////////////////////////////////////////////////////////////////////////
//
// Name        : SaveSymbolFileDialogCtrl.cpp
// Author      : MING
// Created     : 2015Äê8ÔÂ13ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////
#include <wx/sizer.h>
#include <wx/statline.h>
#include "CfDefines.h"
#include "CfUtils.h"
#include "FileUtil.h"
#include "SaveSymbolFileDialogCtrl.h"
#include "LogMessageCtrl.h"

BEGIN_EVENT_TABLE(CfSaveSymbolFileDialogCtrl,CfDialogBaseCtrl)

EVT_BUTTON(ID_BrowseBtn,CfSaveSymbolFileDialogCtrl::OnPushBrowseButton)
EVT_BUTTON(wxID_OK,CfSaveSymbolFileDialogCtrl::OnPushOKButton)
END_EVENT_TABLE()


CfSaveSymbolFileDialogCtrl::CfSaveSymbolFileDialogCtrl(wxWindow* pParent,CfSymbol *symbol,CfDrawingElementGroup* all_elements,CfProject *pProject)
:CfDialogBaseCtrl(pParent,_("Save Symbol"),wxSize(560,480),_("Save symbol to file"))
,m_pProject(pProject)
,m_pSymbol(symbol),m_pAllElements(all_elements)
,CHIP_SYMBOL_FODLER(_("ChipSymbol"))
{
	m_pElementListBox = new wxCheckListBox(this,wxID_ANY,wxDefaultPosition,wxSize(400,150));

	// Add elements of symbol to the list
	m_ContainedElements.Add(symbol->GetElementArray());
	size_t i,j;
	for(i=0;i<m_ContainedElements.Count();i++)
	{
		m_pElementListBox->AppendString(m_ContainedElements[i]->GetName());
		m_pElementListBox->Check(i);
	}

	// Add contained element
	wxRect rect(symbol->GetStartPos(),symbol->GetEndPos());
	all_elements->GetContainedElements(rect,&m_ContainedElements);
	for(j=i;j<m_ContainedElements.Count();j++)
		m_pElementListBox->AppendString(m_ContainedElements[j]->GetName());


	wxString name = symbol->GetName()+"."+ F_EXT_CHIP_SYMBOL;
    wxBoxSizer* s1 = new wxBoxSizer(wxHORIZONTAL);
	m_pNameCtrl = new wxTextCtrl(this, wxID_ANY, name,wxDefaultPosition, wxSize(400,30));
    s1->Add(10, 1, 1, wxEXPAND);
    s1->Add(new wxStaticText(this, wxID_ANY, wxT("Name:")));
    s1->Add(m_pNameCtrl);
    s1->SetItemMinSize((size_t)1, 80, 30);

    wxBoxSizer* s2 = new wxBoxSizer(wxHORIZONTAL);
	wxString path = m_pProject->GetProjectPath().BeforeLast('\\');// + CHIP_SYMBOL_FODLER;
	m_pPathCtrl = new wxTextCtrl(this, wxID_ANY, path, wxDefaultPosition, wxSize(400,30));
	m_pBrowseBtn = new wxButton(this, ID_BrowseBtn, wxT("..."),wxDefaultPosition, wxSize(20,30));
    s2->Add(10, 1, 1, wxEXPAND);
    s2->Add(new wxStaticText(this, wxID_ANY, wxT("Path:")));
    s2->Add(m_pPathCtrl);
    s2->Add(m_pBrowseBtn);
    s2->SetItemMinSize((size_t)1, 80, 30);


    wxBoxSizer* s_btn = new wxBoxSizer(wxHORIZONTAL);
	s_btn->Add(new wxButton(this, wxID_OK, wxT("OK"),wxDefaultPosition, wxSize(80,25)));
    s_btn->Add(10, 1, 1, wxEXPAND);
	s_btn->Add(new wxButton(this, wxID_CANCEL, wxT("Cancel"),wxDefaultPosition, wxSize(80,25)));


	wxBoxSizer* s_all = new wxBoxSizer(wxVERTICAL);
	//s_all->Add(header_img,0);
	s_all->Add(10, 70, 0);
	s_all->Add(new wxStaticText(this, wxID_ANY, wxT("Choose included elements:")), 0, wxLEFT,10);
	s_all->Add(10, 10, 0);
	s_all->Add(m_pElementListBox, 0, wxLEFT,95);
	s_all->Add(10, 10, 0);
	s_all->Add(s1, 0, wxLEFT,5);
	s_all->Add(10, 10, 0);
	s_all->Add(s2, 0, wxLEFT,5);
	s_all->Add(10, 10, 0);
	s_all->Add(new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxSize(540,2), wxLI_HORIZONTAL),0,wxLEFT,5);
	s_all->Add(10, 10, 0);
	s_all->Add(s_btn, 0,wxCENTER); 
	s_all->Add(10, 10, 0);

	SetSizer(s_all);
	s_all->Fit(this);
}

CfSaveSymbolFileDialogCtrl::~CfSaveSymbolFileDialogCtrl(void)
{
}

wxString CfSaveSymbolFileDialogCtrl::GetSymbolPath() const
{
	return m_sPath;
}
void CfSaveSymbolFileDialogCtrl::OnPushOKButton(wxCommandEvent &event)

/////////////////////////////////////////////////////////
////wxMkdir: when we use it we should take care that the  parent directory must be exist.
//////////////////////////////////////////////////////////////////
{
	wxString ws_name;wxString ws_dir1;
	wxString name = m_pNameCtrl->GetValue();
	if(name.IsEmpty()) {m_pNameCtrl->SetFocus();return;}
	wxString dir = m_pPathCtrl->GetValue();
	if(dir.IsEmpty()) {m_pPathCtrl->SetFocus();return;}

	if(dir.EndsWith(_("\\"))) dir = dir.BeforeLast('\\');

	// create the dir if not exist
	if(!wxDirExists(dir) && !wxMkdir(dir)) 
	{
		wxString msg;
		msg.Printf(wxT("Failed to create directory '%s'"),dir.c_str());
		wxMessageDialog dlg(this,msg,PROJECT_CAPTION , wxOK);
		wxMessageBox(msg, PROJECT_CAPTION, wxOK | wxICON_WARNING, this); 
		return;
	}
	
	//get selection items
	wxArrayInt selections;
	m_pElementListBox->GetCheckedItems(selections);

	//
	CfDrawingElementGroup* symbol_elements = m_pSymbol->GetElementArray();
	symbol_elements->Clear();
	for(size_t i=0;i<m_ContainedElements.Count();i++)
	{
		int idx = selections.Index(i);
		if(idx == wxNOT_FOUND)
		{
			// if not selected, add to all_elements (it may come from symbol)
			m_pAllElements->Add(m_ContainedElements[i]);
		}
		else
		{
			//if selected, 1) add to symbol; 2) exclude from all_elements
			symbol_elements->Add(m_ContainedElements[i]);	
			m_pAllElements->Delete(m_ContainedElements[i],false);
		}
	}

	//create symbol file, and dump
	m_sPath = dir+_("\\")+name;
	wxFileOutputStream fos(m_sPath);
	wxPoint offset = wxPoint(-m_pSymbol->GetStartPos().x,-m_pSymbol->GetStartPos().y);
	m_pSymbol->Dump(&fos,offset);

	////add symbol file to project
	//CfProjectItem *folder_item = m_pProject->FindItem(CHIP_SYMBOL_FODLER);
	//if(folder_item)
	//{
	//	m_pProject->AddProjectItem(folder_item,m_sPath,PIT_FILE);
	//	m_pProject->Dump();
	//}
	//
	event.Skip();

}

void CfSaveSymbolFileDialogCtrl::OnPushBrowseButton(wxCommandEvent &event)
{
	wxDirDialog dialog(this, _T("Specify a directory"),wxGetCwd(),wxDD_DEFAULT_STYLE | wxDD_CHANGE_DIR);

    if (dialog.ShowModal() == wxID_OK)
    {		
		m_pPathCtrl->SetValue(dialog.GetPath());
	}
}


bool CfSaveSymbolFileDialogCtrl::NewProjectFile(wxString path)
{
	//wxTextFile fProjectFile(path);
	//if(fProjectFile.Exists()) 
	//{
	//	wxString msg;
	//	msg.Printf(wxT("Project file %s already exists"),m_pNameCtrl->GetValue());
	//	ShowErrorDialog(msg);
	//	return false;
	//}
	//else fProjectFile.Create();
	//wxLogMessage("[File] New project "+m_pNameCtrl->GetValue());

	//wxString strLine;
	//
	//// get selected project type
	//wxTreeItemId id = m_pTypeTree->GetSelection();
	//wxString text = m_pTypeTree->GetItemText(id);
	//strLine.Printf(wxT("%s=%s"),PROJ_TYPE,text);
	//fProjectFile.AddLine(strLine);

	//strLine.Printf(wxT("%s=%s"),PROJ_AUTHOR,m_sAuthor);
	//fProjectFile.AddLine(strLine);

	//// Add classifiers to project
	//wxArrayString *type_list = GetTypeList();
	//wxArrayString classi_list;
	//int idx = type_list->Index(text.c_str());
	//if(idx < 0) 
	//{		
	//	wxLogWarning(wxT("Unknown project type %s"),text.c_str());
	//	fProjectFile.Close();
	//	return true;
	//}
	//GetTypeTemplateList(idx,&classi_list);
	//
	//for(int i=0;i<classi_list.GetCount();i++)
	//{
	//	if(!m_pCurTemplate->IsChecked(i)) continue;
	//	strLine.Printf(wxT("%s=0.%d=%s"),ItemTypeStr[PIT_CLASSIFIER],i,classi_list[i]);
	//	fProjectFile.AddLine(strLine);
	//}
	//fProjectFile.Write();
	//fProjectFile.Close();

	//m_ProjectPath = path;
	return true;
}
