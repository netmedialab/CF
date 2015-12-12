/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfSettingDialogCtrl.h
// Author      :WbRen
// Created     :2015-10-29 »ŒŒ∞±Û
// Description :
// Copyright   :
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/listctrl.h>
#include <wx/spinctrl.h>
#include "CfDefines.h"
#include "EnvironmentUtil.h"
#include "CfWx.h"
#include "CfDialogbaseCtrl.h"
#include "IProperty.h"
#include "Parameter.h"


class CfParameterDialogCtrl: public CfDialogBaseCtrl
{
	enum
    {        
		ID_ParameterList= ID_PropertyDialog + 1,
		
		cfID_AddBtn,
		cfID_ModifyBtn,
		cfID_DelBtn,
		cfID_ReferenceBtn,
		cfID_DoneBtn,

		cfID_EndParameterDialog
	};
public:
	CfParameterDialogCtrl(wxWindow* pParent, CfParameter* pParam);
	~CfParameterDialogCtrl(void);



	void GetNameList(wxStringList &list);


private:
	void OnPushButton(wxCommandEvent &event);
	void OnListCtrlItemSelected(wxListEvent &event);
	void OnListCtrlItemDeselected(wxListEvent &event);
	void OnListDoubleClicked(wxListEvent &event);
	void PushAddButton();
	void PushModifyButton();
	void PushDeleteButton();
	void PushDoneButton();
	
private:
	CfParameter* m_pParameter;
	ParameterItemPtrArray *m_pParameterItemPtrArray;

	bool m_bInit;				// to prevent wxEVT_COMMAND_TEXT_UPDATED event at begining.
	int m_iSelected;
	wxListCtrl			*m_pParameterListCtrl;
	wxButton			*m_pAddBtn;
	wxButton			*m_pModifyBtn;
	wxButton			*m_pDelBtn;
	wxButton			*m_pReferenceBtn;
	wxButton			*m_pDoneBtn;
	//void Load();
	void InitPanels();
	
	DECLARE_EVENT_TABLE()
};

class CfParameter_AddDlg: public CfDialogBaseCtrl
{
	enum
    {        
		cfID_NameTextCtrl,
		cfID_ValueSpinCtrl,
		cfID_TypeComboBox,
	};
public:
	//CfParameter_AddDlg(wxWindow* pParent);
	CfParameter_AddDlg(wxWindow* pParent, wxString name = wxEmptyString, int value = 8, wxString type = wxString("int"));

	~CfParameter_AddDlg(void);

	wxString GetTextName();
	int GetSpinValue();
	wxString GetComboType();
private:
	void Init(wxString name, int spin_value, wxString combo_type);

	wxTextCtrl			*m_pNameTextCtrl;
	wxSpinCtrl			*m_pValueSpinCtrl;
	wxComboBox			*m_pTypeComboBox;
	wxButton			*m_pOKBtn;
	wxButton			*m_pCancelBtn;

	//DECLARE_EVENT_TABLE()
};
