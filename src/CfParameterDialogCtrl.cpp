/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfSettingDialogCtrl.h
// Author      :WbRen
// Created     :2015-10-29 ÈÎÎ°±ó
// Description :
// Copyright   :
//
/////////////////////////////////////////////////////////////////////////////////

#include "CfParameterDialogCtrl.h"
#include "FileUtil.h"

BEGIN_EVENT_TABLE(CfParameterDialogCtrl,CfDialogBaseCtrl)
	
	EVT_LIST_ITEM_SELECTED(wxID_ANY, CfParameterDialogCtrl::OnListCtrlItemSelected)
	EVT_LIST_ITEM_DESELECTED(wxID_ANY, CfParameterDialogCtrl::OnListCtrlItemDeselected)
	EVT_LIST_ITEM_ACTIVATED(wxID_ANY, CfParameterDialogCtrl::OnListDoubleClicked)

	EVT_BUTTON(wxID_ANY, CfParameterDialogCtrl::OnPushButton)
	
END_EVENT_TABLE()


CfParameterDialogCtrl::CfParameterDialogCtrl(wxWindow* pParent, CfParameter* pParam)
:CfDialogBaseCtrl(pParent,_("Parameter"),wxSize(400,275),_(""))
,m_pParameter(pParam)
,m_pParameterItemPtrArray(pParam->GetParameterList()),m_iSelected(-1)
{
	m_pParameterListCtrl = new wxListCtrl(this,ID_ParameterList,wxDefaultPosition, wxSize(300,200),wxLC_REPORT|wxLC_HRULES|wxLC_VRULES);

	m_pParameterListCtrl->InsertColumn( 0, _("Name"), wxLIST_FORMAT_LEFT, 150);
	m_pParameterListCtrl->InsertColumn( 1, _("Value"), wxLIST_FORMAT_LEFT, 70);
	m_pParameterListCtrl->InsertColumn( 2, _("Type"), wxLIST_FORMAT_LEFT, 80);
	

	for(size_t i = 0;i < m_pParameterItemPtrArray->Count(); i++)
	{
		m_pParameterListCtrl->InsertItem(i,(*m_pParameterItemPtrArray)[i]->Name);
		m_pParameterListCtrl->SetItem(i,1,(*m_pParameterItemPtrArray)[i]->Value);
		m_pParameterListCtrl->SetItem(i,2,(*m_pParameterItemPtrArray)[i]->Type);
	}

	//m_pParameterListCtrl->InsertItem(0,_T("haha"));
	//m_pParameterListCtrl->SetItem(0,1,_T("8"));
	//m_pParameterListCtrl->SetItem(0,2,_T("int"));

	//m_pParameterListCtrl->InsertItem(1,_T("aaa"));
	//m_pParameterListCtrl->SetItem(1,1,_T("1"));
	//m_pParameterListCtrl->SetItem(1,2,_T("unsigned"));

	m_pAddBtn = new wxButton(this,cfID_AddBtn, _T("Add"), wxDefaultPosition, wxSize(70,25));
	m_pModifyBtn = new wxButton(this,cfID_ModifyBtn,_T("Modify"), wxDefaultPosition, wxSize(70,25));
	m_pModifyBtn->Enable(false);
	m_pDelBtn = new wxButton(this,cfID_DelBtn,_T("Delete"), wxDefaultPosition, wxSize(70,25));
	m_pDelBtn->Enable(false);
	
	wxBoxSizer *side_btn = new wxBoxSizer(wxVERTICAL);
	side_btn->Add(m_pAddBtn);
	side_btn->Add(70,10);
	side_btn->Add(m_pDelBtn);
	side_btn->Add(70,10);
	side_btn->Add(m_pModifyBtn);

	wxBoxSizer * top_all = new wxBoxSizer(wxHORIZONTAL);
	top_all->Add(10,10);
	top_all->Add(m_pParameterListCtrl);
	top_all->Add(5,200);
	top_all->Add(side_btn);

	m_pReferenceBtn = new wxButton(this,cfID_ReferenceBtn,_T("Reference"), wxDefaultPosition, wxSize(70,25));
	m_pDoneBtn = new wxButton(this,wxID_OK,_T("Done"), wxDefaultPosition, wxSize(70,25));
	wxBoxSizer *buttom_btn = new wxBoxSizer(wxHORIZONTAL);
	buttom_btn->Add(10,10);
	buttom_btn->Add(m_pReferenceBtn);
	buttom_btn->Add(235,10);
	buttom_btn->Add(m_pDoneBtn);
	
	wxBoxSizer* all = new wxBoxSizer(wxVERTICAL);
	
	all->Add(10,10);
	all->Add(top_all);
	all->Add(200,5);
	all->Add(buttom_btn);

	SetSizer(all);
	
}

CfParameterDialogCtrl::~CfParameterDialogCtrl(void)
{
}

void CfParameterDialogCtrl::OnListCtrlItemSelected(wxListEvent &event)
{
	int id = event.GetId();

	switch(id)
	{
		case ID_ParameterList: 
			{
				m_iSelected = event.GetIndex();
				m_pModifyBtn->Enable(true);
				m_pDelBtn->Enable(true);
				break;
			}
		default:
			event.Skip();
	}
}
void CfParameterDialogCtrl::OnListCtrlItemDeselected(wxListEvent &event)
{
	int id = event.GetId();
	switch(id)
	{
		case ID_ParameterList:
			{
				m_pModifyBtn->Enable(false);
				m_pDelBtn->Enable(false);
				break;
			}
		default:
			event.Skip();
	}
}

void CfParameterDialogCtrl::OnListDoubleClicked(wxListEvent &event)
{
	PushModifyButton();
}


void CfParameterDialogCtrl::OnPushButton(wxCommandEvent &event)
{
	int id = event.GetId();
	switch(id)
	{
		case cfID_AddBtn:
			PushAddButton();
			break;
		case cfID_ModifyBtn:
			PushModifyButton();
			break;
		case cfID_DelBtn:
			PushDeleteButton();
			break;
		case cfID_ReferenceBtn:
			break;
		//case cfID_DoneBtn:
		case wxID_OK:
			PushDoneButton();
			event.Skip();
			break;
	}
}


void CfParameterDialogCtrl::InitPanels()
{
}
void CfParameterDialogCtrl::PushAddButton()
{
	CfParameter_AddDlg *dlg = NULL;
	wxString name_head(_("Parameter"));
	wxString name;
	
	for(size_t i = 0; i <= m_pParameterItemPtrArray->Count(); i++)
	{
		int not_match = 0;
		name = name_head + wxString::Format("%i", i);
		for(size_t j = 0; j < m_pParameterItemPtrArray->Count(); j++)
		{
			if((*m_pParameterItemPtrArray)[j]->Name.compare(name))
			{
				not_match++;
			}
		}
		if(not_match == m_pParameterItemPtrArray->Count())
		{
			dlg = new CfParameter_AddDlg(this,name);
			break;
		}
	}
	if(dlg == NULL)
		dlg = new CfParameter_AddDlg(this);
	if(dlg->ShowModal() == wxID_OK)
	{
		ParameterItem *item;
		item = new ParameterItem();
		item->Name = dlg->GetTextName();
		item->Value = wxString::Format(wxT("%i"),dlg->GetSpinValue());
		item->Type = dlg->GetComboType();
		m_pParameterListCtrl->InsertItem(m_pParameterListCtrl->GetItemCount(), item->Name);
		m_pParameterListCtrl->SetItem(m_pParameterListCtrl->GetItemCount() - 1, 1, item->Value);
		m_pParameterListCtrl->SetItem(m_pParameterListCtrl->GetItemCount() - 1, 2, item->Type);
		this->m_pParameterItemPtrArray->Add(item);
	}
	delete dlg;
}

void CfParameterDialogCtrl::PushModifyButton()
{
	CfParameter_AddDlg dlg(this, m_pParameterListCtrl->GetItemText(m_iSelected,0), wxAtoi(m_pParameterListCtrl->GetItemText(m_iSelected,1)), m_pParameterListCtrl->GetItemText(m_iSelected,2));
	if(dlg.ShowModal() == wxID_OK)
	{
		ParameterItem *item;
		item = (*m_pParameterItemPtrArray)[m_iSelected];

		item->Name = dlg.GetTextName();
		item->Value = wxString::Format(wxT("%i"), dlg.GetSpinValue());
		item->Type = dlg.GetComboType();

		m_pParameterListCtrl->SetItem(m_iSelected, 0,item->Name);
		m_pParameterListCtrl->SetItem(m_iSelected, 1, item->Value);
		m_pParameterListCtrl->SetItem(m_iSelected, 2, item->Type);

	}
}

void CfParameterDialogCtrl::PushDeleteButton()
{
	if(m_iSelected != -1)
	{
		m_pParameterListCtrl->DeleteItem(m_iSelected);
		m_pParameterItemPtrArray->RemoveAt(m_iSelected);
	}
}

void CfParameterDialogCtrl::PushDoneButton()
{
//	this->Close();
}


//----------------------------add_dlg---------------------------------
//CfParameter_AddDlg::CfParameter_AddDlg(wxWindow* pParent)
//:CfDialogBaseCtrl(pParent,_("Add"),wxSize(185,170),_(""))
//{
//	Init();
//}

CfParameter_AddDlg::CfParameter_AddDlg(wxWindow* pParent, wxString name, int value, wxString type)
:CfDialogBaseCtrl(pParent,_("Add"),wxSize(195,170),_(""))
{
	Init(name, value, type);

}

void CfParameter_AddDlg::Init(wxString TextName, int SpinValue, wxString ComboType)
{
	m_pNameTextCtrl = new wxTextCtrl(this,cfID_NameTextCtrl, TextName, wxDefaultPosition, wxSize(80,25));
	m_pValueSpinCtrl = new wxSpinCtrl(this, cfID_ValueSpinCtrl,wxString::Format(wxT("%i"),SpinValue), wxDefaultPosition, wxSize(80,25));
	
	wxString combo_text[2];
	combo_text[0] = _T("int");
	combo_text[1] = _T("unsigned");
	m_pTypeComboBox = new wxComboBox(this, cfID_TypeComboBox, wxEmptyString, wxDefaultPosition, wxSize(80,25),2,combo_text,wxCB_READONLY);
	for(int i = 0; i < 2; i++)
	{
		if(!ComboType.compare(combo_text[i]))
		{
			m_pTypeComboBox->SetSelection(i);
			break;
		}
	}
	//m_pTypeComboBox->SetSelection(0);

	m_pOKBtn = new wxButton(this, wxID_OK, _T("OK"), wxDefaultPosition, wxSize(70,25));
	m_pCancelBtn = new wxButton(this, wxID_CANCEL, _T("CANCEL"), wxDefaultPosition, wxSize(70,25));

	wxBoxSizer *l1 = new wxBoxSizer(wxHORIZONTAL);
	l1->Add(10,25);
	l1->Add(new wxStaticText(this,wxID_ANY,_("Name:"),wxDefaultPosition,wxSize(70,25),wxALIGN_LEFT),0,wxTOP);
	l1->Add(20,25);
	l1->Add(m_pNameTextCtrl);

	wxBoxSizer *l2 = new wxBoxSizer(wxHORIZONTAL);
	l2->Add(10,25);
	l2->Add(new wxStaticText(this,wxID_ANY,_("Value:"),wxDefaultPosition,wxSize(70,25),wxALIGN_LEFT),0,wxTOP);
	l2->Add(20,25);
	l2->Add(m_pValueSpinCtrl);

	wxBoxSizer *l3 = new wxBoxSizer(wxHORIZONTAL);
	l3->Add(10,25);
	l3->Add(new wxStaticText(this,wxID_ANY,_("Type:"),wxDefaultPosition,wxSize(70,25),wxALIGN_LEFT),0,wxTOP);
	l3->Add(20,25);
	l3->Add(m_pTypeComboBox);

	wxBoxSizer *l4 = new wxBoxSizer(wxHORIZONTAL);
	l4->Add(10,25);
	l4->Add(m_pOKBtn);
	l4->Add(20,25);
	l4->Add(m_pCancelBtn);

	wxBoxSizer *l = new wxBoxSizer(wxVERTICAL);
	l->Add(10,5);
	l->Add(l1);
	l->Add(10,5);
	l->Add(l2);
	l->Add(10,5);
	l->Add(l3);
	l->Add(10,15);
	l->Add(l4);


	SetSizer(l);
}
CfParameter_AddDlg::~CfParameter_AddDlg(void)
{
}

wxString CfParameter_AddDlg::GetTextName()
{
	return m_pNameTextCtrl->GetValue();
}
int CfParameter_AddDlg::GetSpinValue()
{
	return m_pValueSpinCtrl->GetValue();
}
wxString CfParameter_AddDlg::GetComboType()
{
	return m_pTypeComboBox->GetStringSelection();
}