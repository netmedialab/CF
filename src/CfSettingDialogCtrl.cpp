/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfSettingDialogCtrl.h
// Author      :WbRen
// Created     :2015-09-15 »ŒŒ∞±Û
// Description :
// Copyright   :
//
/////////////////////////////////////////////////////////////////////////////////

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/filename.h>
#include <wx/statline.h>
#include "CfUtils.h"
#include "CfSettingDialogCtrl.h"

DEFINE_EVENT_TYPE(wxEVT_CLOSEPANE_EVENT)


BEGIN_EVENT_TABLE(CfSettingDialogCtrl,CfDialogBaseCtrl)

	EVT_TEXT(wxID_ANY, CfSettingDialogCtrl::OnSettingChanged)
	EVT_TREEBOOK_PAGE_CHANGED(ID_TreeBook, CfSettingDialogCtrl::OnChangePage)
	EVT_BUTTON(wxID_ANY,CfSettingDialogCtrl::OnPushButton)
	EVT_CHECKBOX(wxID_ANY, CfSettingDialogCtrl::On0ClickCheckbox)

END_EVENT_TABLE()


CfSettingDialogCtrl::CfSettingDialogCtrl(wxWindow* pParent, CfEnvironmentUtil *pEnvironment, int &panel_choice)
:CfDialogBaseCtrl(pParent,_("Settings"),wxSize(630,490),_("Environment Options")),m_pEnvironment(pEnvironment),m_iPanelChoice(panel_choice)
,m_bInit(true)
,m_bGeneralPortPanelShow(false)
,m_bGeneralWirePanelShow(false)
,m_bGeneralSymbolPanelShow(false)
,m_bGeneralGridPanelShow(false)
,m_bLayerPanelShow(false)
{
	setting_notebook = new wxTreebook(this,ID_TreeBook,wxDefaultPosition,wxSize(600,150));

	// ------- initialize panels ------------ //
	general_panel = new wxPanel(setting_notebook,wxID_ANY);
		general_port_panel = new wxPanel(setting_notebook,wxID_ANY);
		general_wire_panel = new wxPanel(setting_notebook,wxID_ANY);
		general_symbol_panel = new wxPanel(setting_notebook,wxID_ANY);
		general_grid_panel = new wxPanel(setting_notebook,wxID_ANY);
	default_layer_panel = new wxPanel(setting_notebook,wxID_ANY);
	

	InitPanels(m_iPanelChoice);
	
	//------------------------------------------------------------------------------------------------------

		setting_notebook->AddPage(general_panel,wxT("General"),true);
			setting_notebook->AddSubPage(general_port_panel,_T("Port"),true);
			setting_notebook->AddSubPage(general_wire_panel,_T("Wire"),true);
			setting_notebook->AddSubPage(general_symbol_panel,_T("Symbol"),true);
			setting_notebook->AddSubPage(general_grid_panel,_T("Grid"),true);

		setting_notebook->AddPage(default_layer_panel,wxT("Layer"),true);
		

		wxBoxSizer* s_btn = new wxBoxSizer(wxHORIZONTAL);
		m_pApplyBtn = new wxButton(this, ID_ApplyBtn, wxT("Apply"),wxDefaultPosition, wxSize(80,25));
		s_btn->Add(20, 5, 0);
		s_btn->Add(m_pApplyBtn, 0);
		s_btn->Add(5, 5, 0);
		s_btn->Add(new wxButton(this, wxID_CANCEL, wxT("Close"),wxDefaultPosition, wxSize(80,25)), 0);
		
		wxBoxSizer* s_all = new wxBoxSizer(wxVERTICAL);
		s_all->Add(10, 60, 0, wxEXPAND);

		s_all->Add(setting_notebook, 1, wxCENTER|wxALL, 20);
		s_all->Add(new wxStaticLine(this), 0, wxEXPAND|wxALL, 5);
		s_all->Add(s_btn,0,wxALIGN_RIGHT|wxRIGHT,20);
		s_all->Add(20, 5, 0);

		SetSizer(s_all);

		m_bInit = false;
		m_pApplyBtn->Enable(false);
		setting_notebook->SetSelection(m_iPanelChoice);
}


CfSettingDialogCtrl::~CfSettingDialogCtrl(void)
{
}

void CfSettingDialogCtrl::InitPanels(int panel_id)
{
	if(!m_bGeneralPortPanelShow)
	{
		//--------------- General --------------------
		wxString LineColor;
		wxString FilledColor;
		wxString LineWidth;
		wxString GridSize;
		bool Shown;
		int sel = 0;

		//Port

		wxBoxSizer* gs_port1 = new wxBoxSizer(wxVERTICAL);
		gs_port1->Add(10,5);
		gs_port1->Add(new wxStaticText(general_port_panel,wxID_ANY,_("Default Line Color:"),wxDefaultPosition,wxSize(150,30),wxALIGN_LEFT),0,wxTOP);
		gs_port1->Add(new wxStaticText(general_port_panel,wxID_ANY,_("Default Line Width:"),wxDefaultPosition,wxSize(150,30),wxALIGN_LEFT),0,wxTOP);
		gs_port1->Add(new wxStaticText(general_port_panel,wxID_ANY,_("Default Filled Color:"),wxDefaultPosition,wxSize(150,30),wxALIGN_LEFT),0,wxTOP);
		gs_port1->Add(new wxStaticText(general_port_panel,wxID_ANY,_("Default show or not:"),wxDefaultPosition,wxSize(150,30),wxALIGN_LEFT),0,wxTOP);
		
		wxBoxSizer* gs_port2 = new wxBoxSizer(wxVERTICAL);
		LineColor = CfEnvironmentUtil::Get()->GetString(PROP_SETTING_PORT_DEFAULTLINECOLOR,_T("Blue"));
		LineWidth = CfEnvironmentUtil::Get()->GetString(PROP_SETTING_PORT_DEFAULTLINEWIDTH,_T("3"));
		FilledColor = CfEnvironmentUtil::Get()->GetString(PROP_SETTING_PORT_DEFAULTFILLEDCOLOR,_T("None"));
		Shown = CfEnvironmentUtil::Get()->GetBool(PROP_SETTING_PORT_SHOWN,true);

		m_p0PortDefaultLineColorCtrl = new wxComboBox(general_port_panel,cfID_0PortDefaultLineColor,wxEmptyString,wxDefaultPosition,wxSize(70,20),6,cfColorStringList,wxCB_READONLY);
		for(int i = 0;i <= cfColorStringList->size();i++)
			if(LineColor.IsSameAs(cfColorStringList[i]))
			{
				m_p0PortDefaultLineColorCtrl->SetSelection(i);
				break;
			}
		m_p0PortDefaultLineWidth = new wxSpinCtrl(general_port_panel, cfID_0PortDefaultLineWidth, LineWidth, wxDefaultPosition, wxSize(70,20));
		m_p0PortDefaultFilledColorCtrl = new wxComboBox(general_port_panel,cfID_0PortDefaultFilledColor,wxEmptyString,wxDefaultPosition,wxSize(70,20),6,cfColorStringList,wxCB_READONLY);
		for(int i = 0;i <= cfColorStringList->size();i++)
			if(FilledColor.IsSameAs(cfColorStringList[i]))
			{
				m_p0PortDefaultFilledColorCtrl->SetSelection(i);
				break;
			}
		m_p0PortShownCheck = new wxCheckBox(general_port_panel,wxID_ANY,_("To show"));
		m_p0PortShownCheck->SetValue(Shown);
		gs_port2->Add(40,5);
		gs_port2->Add(m_p0PortDefaultLineColorCtrl);
		gs_port2->Add(10,10);
		gs_port2->Add(m_p0PortDefaultLineWidth);
		gs_port2->Add(10,10);
		gs_port2->Add(m_p0PortDefaultFilledColorCtrl);
		gs_port2->Add(10,10);
		gs_port2->Add(m_p0PortShownCheck, 0, wxEXPAND|wxALL);

		wxBoxSizer* s_general_port= new wxBoxSizer(wxHORIZONTAL);
		s_general_port->Add(gs_port1, 0, wxEXPAND|wxALL);
		s_general_port->Add(gs_port2, 0, wxEXPAND|wxALL);

		general_port_panel->SetSizer(s_general_port);

		//Wire

		wxBoxSizer* gs_wire1 = new wxBoxSizer(wxVERTICAL);
		gs_wire1->Add(10,5);
		gs_wire1->Add(new wxStaticText(general_wire_panel,wxID_ANY,_("Default Line Color:"),wxDefaultPosition,wxSize(150,30),wxALIGN_LEFT),0,wxTOP);
		gs_wire1->Add(new wxStaticText(general_wire_panel,wxID_ANY,_("Default Line Width:"),wxDefaultPosition,wxSize(150,30),wxALIGN_LEFT),0,wxTOP);
		gs_wire1->Add(new wxStaticText(general_wire_panel,wxID_ANY,_("Default Filled Color:"),wxDefaultPosition,wxSize(150,30),wxALIGN_LEFT),0,wxTOP);
		gs_wire1->Add(new wxStaticText(general_wire_panel,wxID_ANY,_("Default show or not:"),wxDefaultPosition,wxSize(150,30),wxALIGN_LEFT),0,wxTOP);
		
		wxBoxSizer* gs_wire2 = new wxBoxSizer(wxVERTICAL);

		LineColor = CfEnvironmentUtil::Get()->GetString(PROP_SETTING_WIRE_DEFAULTLINECOLOR,_T("Blue"));
		LineWidth = CfEnvironmentUtil::Get()->GetString(PROP_SETTING_WIRE_DEFAULTLINEWIDTH,_T("2"));
		FilledColor = CfEnvironmentUtil::Get()->GetString(PROP_SETTING_WIRE_DEFAULTFILLEDCOLOR,_T("None"));
		Shown = CfEnvironmentUtil::Get()->GetBool(PROP_SETTING_WIRE_SHOWN,true);

		m_p0WireDefaultLineColorCtrl = new wxComboBox(general_wire_panel,cfID_0WireDefaultLineColor,wxEmptyString,wxDefaultPosition,wxSize(70,20),6,cfColorStringList,wxCB_READONLY);
		for(int i = 0;i <= cfColorStringList->size();i++)
			if(LineColor.IsSameAs(cfColorStringList[i]))
			{
				m_p0WireDefaultLineColorCtrl->SetSelection(i);
				break;
			}
		m_p0WireDefaultLineWidth = new wxSpinCtrl(general_wire_panel, cfID_0WireDefaultLineWidth, LineWidth, wxDefaultPosition, wxSize(70,20));
		m_p0WireDefaultFilledColorCtrl = new wxComboBox(general_wire_panel,cfID_0WireDefaultFilledColor,wxEmptyString,wxDefaultPosition,wxSize(70,20),6,cfColorStringList,wxCB_READONLY);
		for(int i = 0;i <= cfColorStringList->size();i++)
			if(FilledColor.IsSameAs(cfColorStringList[i]))
			{
				m_p0WireDefaultFilledColorCtrl->SetSelection(i);
				break;
			}
		m_p0WireShownCheck = new wxCheckBox(general_wire_panel,wxID_ANY,_("To show"));
		m_p0WireShownCheck->SetValue(Shown);
		gs_wire2->Add(40,5);
		gs_wire2->Add(m_p0WireDefaultLineColorCtrl, 0, wxEXPAND|wxALL);
		gs_wire2->Add(10,10);
		gs_wire2->Add(m_p0WireDefaultLineWidth, 0, wxEXPAND|wxALL);
		gs_wire2->Add(10,10);
		gs_wire2->Add(m_p0WireDefaultFilledColorCtrl, 0, wxEXPAND|wxALL);
		gs_wire2->Add(10,10);
		gs_wire2->Add(m_p0WireShownCheck, 0, wxEXPAND|wxALL);

		wxBoxSizer* s_general_wire= new wxBoxSizer(wxHORIZONTAL);
		s_general_wire->Add(gs_wire1, 0, wxEXPAND|wxALL);
		s_general_wire->Add(gs_wire2, 0, wxEXPAND|wxALL);

		general_wire_panel->SetSizer(s_general_wire);

		//Symbol
		wxBoxSizer* gs_symbol1 = new wxBoxSizer(wxVERTICAL);
		gs_symbol1->Add(10,5);
		gs_symbol1->Add(new wxStaticText(general_symbol_panel,wxID_ANY,_("Default Line Color:"),wxDefaultPosition,wxSize(150,30),wxALIGN_LEFT),0,wxTOP);
		gs_symbol1->Add(new wxStaticText(general_symbol_panel,wxID_ANY,_("Default Line Width:"),wxDefaultPosition,wxSize(150,30),wxALIGN_LEFT),0,wxTOP);
		gs_symbol1->Add(new wxStaticText(general_symbol_panel,wxID_ANY,_("Default Filled Color:"),wxDefaultPosition,wxSize(150,30),wxALIGN_LEFT),0,wxTOP);
		gs_symbol1->Add(new wxStaticText(general_symbol_panel,wxID_ANY,_("Default show or not:"),wxDefaultPosition,wxSize(150,30),wxALIGN_LEFT),0,wxTOP);
		
		wxBoxSizer* gs_symbol2 = new wxBoxSizer(wxVERTICAL);

		LineColor = CfEnvironmentUtil::Get()->GetString(PROP_SETTING_SYMBOL_DEFAULTLINECOLOR,_T("Black"));
		LineWidth = CfEnvironmentUtil::Get()->GetString(PROP_SETTING_SYMBOL_DEFAULTLINEWIDTH,_T("6"));
		FilledColor = CfEnvironmentUtil::Get()->GetString(PROP_SETTING_SYMBOL_DEFAULTFILLEDCOLOR,_T("None"));
		Shown = CfEnvironmentUtil::Get()->GetBool(PROP_SETTING_SYMBOL_SHOWN,true);

		m_p0SymbolDefaultLineColorCtrl = new wxComboBox(general_symbol_panel,cfID_0SymbolDefaultLineColor,wxEmptyString,wxDefaultPosition,wxSize(70,20),6,cfColorStringList,wxCB_READONLY);
		for(int i = 0;i <= cfColorStringList->size();i++)
			if(LineColor.IsSameAs(cfColorStringList[i]))
			{
				m_p0SymbolDefaultLineColorCtrl->SetSelection(i);
				break;
			}
		m_p0SymbolDefaultLineWidth = new wxSpinCtrl(general_symbol_panel, cfID_0SymbolDefaultLineWidth, LineWidth, wxDefaultPosition, wxSize(70,20));
		m_p0SymbolDefaultFilledColorCtrl = new wxComboBox(general_symbol_panel,cfID_0SymbolDefaultFilledColor,wxEmptyString,wxDefaultPosition,wxSize(70,20),6,cfColorStringList,wxCB_READONLY);
		for(int i = 0;i <= cfColorStringList->size();i++)
			if(FilledColor.IsSameAs(cfColorStringList[i]))
			{
				m_p0SymbolDefaultFilledColorCtrl->SetSelection(i);
				break;
			}
		m_p0SymbolShownCheck = new wxCheckBox(general_symbol_panel,wxID_ANY,_("To show"));
		m_p0SymbolShownCheck->SetValue(Shown);
		gs_symbol2->Add(40,5);
		gs_symbol2->Add(m_p0SymbolDefaultLineColorCtrl, 0, wxEXPAND|wxALL);
		gs_symbol2->Add(10,10);
		gs_symbol2->Add(m_p0SymbolDefaultLineWidth, 0, wxEXPAND|wxALL);
		gs_symbol2->Add(10,10);
		gs_symbol2->Add(m_p0SymbolDefaultFilledColorCtrl, 0, wxEXPAND|wxALL);
		gs_symbol2->Add(10,10);
		gs_symbol2->Add(m_p0SymbolShownCheck, 0, wxEXPAND|wxALL);

		wxBoxSizer* s_general_symbol= new wxBoxSizer(wxHORIZONTAL);
		s_general_symbol->Add(gs_symbol1, 0, wxEXPAND|wxALL);
		s_general_symbol->Add(gs_symbol2, 0, wxEXPAND|wxALL);

		general_symbol_panel->SetSizer(s_general_symbol);

		//Grid
		wxBoxSizer* gs_grid = new wxBoxSizer(wxHORIZONTAL);
		gs_grid->Add(10,5);
		gs_grid->Add(new wxStaticText(general_grid_panel,wxID_ANY,_("Default Grid Size:"),wxDefaultPosition,wxSize(150,30),wxALIGN_LEFT),0,wxTOP);

		GridSize = CfEnvironmentUtil::Get()->GetString(PROP_SETTING_GRID_SIZE,_T("1"));
		m_p0GridSizeCtrl = new wxSpinCtrl(general_grid_panel, cfID_0GridSize,GridSize,wxDefaultPosition,wxSize(70,20));

		gs_grid->Add(m_p0GridSizeCtrl);

		general_grid_panel->SetSizer(gs_grid);
		//--------------- Layer --------------------
		m_p1DefaultLayerListBox= new wxListBox(default_layer_panel,cfID_1DefaultLayerList,wxDefaultPosition,wxSize(150,280));
		
		wxArrayString layer_list;
		m_pEnvironment->GetStringList(PROP_SETTING_DEFAULTLAYER,&layer_list);
		if(layer_list.size() == 0)
		{
			layer_list.Add(_T("MET5"));
			layer_list.Add(_T("VIA4"));
			layer_list.Add(_T("MET4"));
			layer_list.Add(_T("VIA3"));
			layer_list.Add(_T("MET3"));
			layer_list.Add(_T("VIA2"));
			layer_list.Add(_T("MET2"));
			layer_list.Add(_T("VIA1"));
			layer_list.Add(_T("MET1"));
			layer_list.Add(_T("CONT"));
			layer_list.Add(_T("POLY"));
			layer_list.Add(_T("DIFF"));
			m_pEnvironment->SetStringList(PROP_SETTING_DEFAULTLAYER,&layer_list);
		}

		wxBoxSizer* ls1 = new wxBoxSizer(wxVERTICAL);
		m_p1AddLayerBtn = new wxButton(default_layer_panel,cfID_1AddLayerBtn,_("Add"));
		m_p1DelLayerBtn = new wxButton(default_layer_panel,cfID_1DelLayerBtn,_("Delete"));
		m_p1MoveUpBtn = new wxButton(default_layer_panel,cfID_1MoveUpBtn,_("Up"));
		m_p1MoveDownBtn = new wxButton(default_layer_panel,cfID_1MoveDownBtn,_("Down"));
		ls1->Add(m_p1AddLayerBtn);
		ls1->Add(m_p1DelLayerBtn);
		ls1->Add(m_p1MoveUpBtn);
		ls1->Add(m_p1MoveDownBtn);

		wxBoxSizer* ls2 = new wxBoxSizer(wxHORIZONTAL);
		ls2->Add(m_p1DefaultLayerListBox);
		ls2->Add(5, 0, 0);
		ls2->Add(ls1);

    	wxBoxSizer* ls = new wxBoxSizer(wxVERTICAL);
		ls->Add(new wxStaticText(default_layer_panel, wxID_ANY, wxT("Default Layer List:")));	
		ls->Add(2, 5, 0);
		ls->Add(ls2);

		default_layer_panel->SetSizer(ls);
	}
}

void CfSettingDialogCtrl::LoadSetting(int panel_id)
{
	m_bInit = true;
	if((panel_id == PID_GENERAL_PORT) && !m_bGeneralPortPanelShow)
	{
		m_bGeneralPortPanelShow = true;
	}
	if((panel_id == PID_GENERAL_WIRE) && !m_bGeneralWirePanelShow)
	{
		m_bGeneralWirePanelShow = true;
	}
	if((panel_id == PID_GENERAL_SYMBOL) && !m_bGeneralSymbolPanelShow)
	{
		m_bGeneralSymbolPanelShow = true;
	}
	if((panel_id == PID_GENERAL_GRID) && !m_bGeneralGridPanelShow)
	{
		m_bGeneralGridPanelShow = true;
	}
	if((panel_id == PID_LAYER) && !m_bLayerPanelShow)
	{
		m_bLayerPanelShow = true;
		wxArrayString layer_list;
		m_pEnvironment->GetStringList(PROP_SETTING_DEFAULTLAYER,&layer_list);
		m_p1DefaultLayerListBox->Append(layer_list);
		m_p1DefaultLayerListBox->SetSelection(0);
	}
	m_bInit = false;
}

void CfSettingDialogCtrl::On0ClickCheckbox(wxCommandEvent &event)
{
	m_pApplyBtn->Enable(true);
}

void CfSettingDialogCtrl::OnChangePage(wxTreebookEvent &event)
{
	int old = event.GetOldSelection();
	if(old < 0) return;
	int sel = event.GetSelection();
	//InitPanels(sel);
	LoadSetting(sel);
}

void CfSettingDialogCtrl::OnSettingChanged(wxCommandEvent &event )
{
	wxEventType et = event.GetEventType();
	if(((et != wxEVT_COMMAND_TEXT_UPDATED) && (et!= wxEVT_COMMAND_RADIOBOX_SELECTED))|| m_bInit) return;
	int id = event.GetId();
	if((id > cfID_EndSettingDialog) || (id < ID_SettingDialog)) return;

	m_pApplyBtn->Enable(true);
}	

void CfSettingDialogCtrl::OnPushButton(wxCommandEvent &event )
{
	int id = event.GetId();
	switch(id)
	{
	case ID_ApplyBtn:
		PushApplyButton();
		break;
	case wxID_CANCEL:
		PushCloseButton();
		event.Skip();
		break;
	case cfID_1AddLayerBtn:
		{
			wxTextEntryDialog input_dialog(this,
										_T("Please enter the new default layer name:\n"),
										_T("Add default layer"),
										wxEmptyString,
										wxOK | wxCANCEL);

			if (input_dialog.ShowModal() == wxID_OK)
			{
				wxString layer = input_dialog.GetValue().c_str();
				m_p1DefaultLayerListBox->Append(layer);
				m_pApplyBtn->Enable(true);
			}
			break;
		}
	case cfID_1DelLayerBtn:
		{
			int sel = m_p1DefaultLayerListBox->GetSelection();
			m_p1DefaultLayerListBox->Delete(sel);	
			m_pApplyBtn->Enable(true);
			break;
		}
	case cfID_1MoveUpBtn:
		{
			int sel = m_p1DefaultLayerListBox->GetSelection();
			if(sel < 0) return;
			wxArrayString layer_list;
			layer_list = m_p1DefaultLayerListBox->GetStrings();
			wxString exchange;
			exchange = layer_list[sel];
			layer_list[sel] = layer_list[sel - 1];
			layer_list[sel - 1] = exchange;
			m_p1DefaultLayerListBox->Clear();
			m_p1DefaultLayerListBox->Append(layer_list);
			m_pApplyBtn->Enable(true);
			break;
		}
	case cfID_1MoveDownBtn:
		{
			int sel = m_p1DefaultLayerListBox->GetSelection();
			if(sel < 0) return;
			wxArrayString layer_list;
			layer_list = m_p1DefaultLayerListBox->GetStrings();			
			wxString exchange;
			exchange = layer_list[sel];
			layer_list[sel] = layer_list[sel + 1];
			layer_list[sel + 1] = exchange;
			m_p1DefaultLayerListBox->Clear();
			m_p1DefaultLayerListBox->Append(layer_list);
			m_pApplyBtn->Enable(true);
			break;
		}
	default:
		event.Skip();
	}
}

void CfSettingDialogCtrl::PushApplyButton()
{
	if(m_bGeneralPortPanelShow)
	{
		//if(!m_p0PortDefaultLineWidth->GetValue().IsNumber())
		//{
		//	wxString warning_msg;
		//	warning_msg = _T("Please input the number for default line width");
		//	wxMessageBox(warning_msg, PROJECT_CAPTION, wxOK | wxICON_WARNING, this);
		//	return;
		//}
		m_pEnvironment->SetString(PROP_SETTING_PORT_DEFAULTLINECOLOR,m_p0PortDefaultLineColorCtrl->GetValue());
		//m_pEnvironment->SetString(PROP_SETTING_PORT_DEFAULTLINEWIDTH,wxString::Format("%i", m_p0PortDefaultLineWidth->GetValue()));
		m_pEnvironment->SetInt(PROP_SETTING_PORT_DEFAULTLINEWIDTH,m_p0PortDefaultLineWidth->GetValue());
		m_pEnvironment->SetString(PROP_SETTING_PORT_DEFAULTFILLEDCOLOR,m_p0PortDefaultFilledColorCtrl->GetValue());
		m_pEnvironment->SetBool(PROP_SETTING_PORT_SHOWN,m_p0PortShownCheck->GetValue());
	}
	if(m_bGeneralWirePanelShow)
	{
		//if(!m_p0WireDefaultLineWidth->GetValue().IsNumber())
		//{
		//	wxString warning_msg;
		//	warning_msg = _T("Please input the number for default line width");
		//	wxMessageBox(warning_msg, PROJECT_CAPTION, wxOK | wxICON_WARNING, this);
		//	return;
		//}
		m_pEnvironment->SetString(PROP_SETTING_WIRE_DEFAULTLINECOLOR,m_p0WireDefaultLineColorCtrl->GetValue());
		//m_pEnvironment->SetString(PROP_SETTING_WIRE_DEFAULTLINEWIDTH,wxString::Format("%i",m_p0WireDefaultLineWidth->GetValue()));
		m_pEnvironment->SetInt(PROP_SETTING_WIRE_DEFAULTLINEWIDTH,m_p0WireDefaultLineWidth->GetValue());
		m_pEnvironment->SetString(PROP_SETTING_WIRE_DEFAULTFILLEDCOLOR,m_p0WireDefaultFilledColorCtrl->GetValue());
		m_pEnvironment->SetBool(PROP_SETTING_WIRE_SHOWN,m_p0WireShownCheck->GetValue());
	}
	if(m_bGeneralSymbolPanelShow)
	{
		//if(!m_p0SymbolDefaultLineWidth->GetValue().IsNumber())
		//{
		//	wxString warning_msg;
		//	warning_msg = _T("Please input the number for default line width");
		//	wxMessageBox(warning_msg, PROJECT_CAPTION, wxOK | wxICON_WARNING, this);
		//	return;
		//}
		m_pEnvironment->SetString(PROP_SETTING_SYMBOL_DEFAULTLINECOLOR,m_p0SymbolDefaultLineColorCtrl->GetValue());
		//m_pEnvironment->SetString(PROP_SETTING_SYMBOL_DEFAULTLINEWIDTH,wxString::Format("%i", m_p0SymbolDefaultLineWidth->GetValue()));
		m_pEnvironment->SetInt(PROP_SETTING_SYMBOL_DEFAULTLINEWIDTH,m_p0SymbolDefaultLineWidth->GetValue());
		m_pEnvironment->SetString(PROP_SETTING_SYMBOL_DEFAULTFILLEDCOLOR,m_p0SymbolDefaultFilledColorCtrl->GetValue());
		m_pEnvironment->SetBool(PROP_SETTING_SYMBOL_SHOWN,m_p0SymbolShownCheck->GetValue());
	}
	if(m_bGeneralGridPanelShow)
	{
		m_pEnvironment->SetString(PROP_SETTING_GRID_SIZE,wxString::Format("%i", m_p0GridSizeCtrl->GetValue()));
		//cfEDITOR_GRIDSIZE = m_p0GridSizeCtrl->GetValue();
		//m_pEnvironment->SetInt(PROP_SETTING_GRID_SIZE,cfEDITOR_GRIDSIZE);
		
	}
	if(m_bLayerPanelShow)
	{
		m_pEnvironment->RemoveList(PROP_SETTING_DEFAULTLAYER);
		wxArrayString url_list = m_p1DefaultLayerListBox->GetStrings();
		m_pEnvironment->SetStringList(PROP_SETTING_DEFAULTLAYER,&url_list);
	}

	m_pApplyBtn->Enable(false);
	
}

void CfSettingDialogCtrl::PushCloseButton()
{
	m_iPanelChoice = setting_notebook->GetSelection();
}
