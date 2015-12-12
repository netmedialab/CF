/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfSettingDialogCtrl.h
// Author      :WbRen
// Created     :2015-09-15 »ŒŒ∞±Û
// Description :
// Copyright   :
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/treebook.h>
#include <wx/listctrl.h>
#include "CfDefines.h"
#include "EnvironmentUtil.h"
#include "CfWx.h"
#include "CfDialogbaseCtrl.h"
#include "StringUtil.h"
#include <wx/spinctrl.h>

#define PID_GENERAL_PORT		1
	#define PID_GENERAL_WIRE	2
	#define PID_GENERAL_SYMBOL	3
	#define PID_GENERAL_GRID	4
#define PID_LAYER				5
//#define PID_FTP			2
//#define PID_LOCALTOOL	3
//#define PID_FILEASSOC	4
//#define PID_PROJTYPE	5
//#define PID_ADDRHISTORY 6

//DECLARE_EVENT_TYPE(wxEVT_CLOSEPANE_EVENT, -1)

class CfSettingDialogCtrl: public CfDialogBaseCtrl
{
	enum
    {        
		ID_TreeBook= ID_SettingDialog + 1,
		ID_ApplyBtn,
		
		
		// Property

		// General
		cfID_0PortDefaultLineColor,
		cfID_0PortDefaultLineWidth,
		cfID_0PortDefaultFilledColor,

		cfID_0SymbolDefaultLineColor,
		cfID_0SymbolDefaultLineWidth,
		cfID_0SymbolDefaultFilledColor,
		
		cfID_0WireDefaultLineColor,
		cfID_0WireDefaultLineWidth,
		cfID_0WireDefaultFilledColor,

		cfID_0GridSize,

		// Layer
		cfID_1DefaultLayerList,
		cfID_1AddLayerBtn,
		cfID_1DelLayerBtn,
		cfID_1MoveUpBtn,
		cfID_1MoveDownBtn,

		cfID_EndSettingDialog
	};
public:
	CfSettingDialogCtrl(wxWindow* pParent, CfEnvironmentUtil *pEnvironment, int &panel_choice);
	~CfSettingDialogCtrl(void);

private:
	void OnPushButton(wxCommandEvent &event);
	void OnClickListBox(wxCommandEvent &event);
	void OnClickListCtrl(wxListEvent &event);


	void OnSettingChanged(wxCommandEvent &event);
	void OnChangePage(wxTreebookEvent &event);

	void PushApplyButton();
	void PushCloseButton();

private:
	CfEnvironmentUtil *m_pEnvironment;
	wxTreebook *setting_notebook;
	int &m_iPanelChoice;
	//bool
	bool m_bGeneralPortPanelShow;
	bool m_bGeneralWirePanelShow;
	bool m_bGeneralSymbolPanelShow;
	bool m_bGeneralGridPanelShow;

	bool m_bLayerPanelShow;
	
	//panel
	wxPanel *general_panel;
		wxPanel *general_port_panel;
		wxPanel *general_wire_panel;
		wxPanel *general_symbol_panel;
		wxPanel *general_grid_panel;
	wxPanel *default_layer_panel;


	wxButton *m_pApplyBtn;
	bool m_bInit;				// to prevent wxEVT_COMMAND_TEXT_UPDATED event at begining.

	//------ p0: general ---------------
	wxComboBox			*m_p0PortDefaultLineColorCtrl;
	wxSpinCtrl			*m_p0PortDefaultLineWidth;
	wxComboBox			*m_p0PortDefaultFilledColorCtrl;
	wxCheckBox			*m_p0PortShownCheck;
	
	wxComboBox			*m_p0SymbolDefaultLineColorCtrl;
	wxSpinCtrl			*m_p0SymbolDefaultLineWidth;
	wxComboBox			*m_p0SymbolDefaultFilledColorCtrl;
	wxCheckBox			*m_p0SymbolShownCheck;
	
	wxComboBox			*m_p0WireDefaultLineColorCtrl;
	wxSpinCtrl			*m_p0WireDefaultLineWidth;
	wxComboBox			*m_p0WireDefaultFilledColorCtrl;
	wxCheckBox			*m_p0WireShownCheck;

	wxSpinCtrl			*m_p0GridSizeCtrl;

	void On0ClickCheckbox(wxCommandEvent &event);

	//------ p1: layer ---------------
	wxListBox			*m_p1DefaultLayerListBox;
	wxButton			*m_p1AddLayerBtn;
	wxButton			*m_p1DelLayerBtn;
	wxButton			*m_p1MoveUpBtn;
	wxButton			*m_p1MoveDownBtn;

	void LoadSetting(int panel_id);
	void InitPanels(int panel_id);
	DECLARE_EVENT_TABLE()

};
