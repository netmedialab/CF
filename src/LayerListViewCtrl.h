
/////////////////////////////////////////////////////////////////////////////////
//
// Name        : LayerListViewCtrl.h
// Author      : MING
// Created     : 2015Äê9ÔÂ1ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "wx/dataview.h"
#include "CfDefines.h"
#include "Project.h"
#include "Workspace.h"
#include "EnvironmentUtil.h"
#include "Notification.h"
#include "Panel.h"
#include "HighlightElementList.h"

class CfLayerListViewCtrl
	: public wxDataViewListCtrl, public CfNotificationSender
	, public CfHighlightElementList
{
public:
	CfLayerListViewCtrl(wxWindow *parent);
	~CfLayerListViewCtrl(void);

	void Reload();
	void LoadElement(IElementWithChildren *ewc);

private:
	IElementWithChildren *m_pElementWithChildren;
    wxDECLARE_EVENT_TABLE();
    void OnItemActivated(wxDataViewEvent& event);
    void OnItemSelectionChanged(wxDataViewEvent& event);
    void OnItemValueChanged(wxDataViewEvent& event);

	void AddElementNode(IElementGroup *element_group);
	void HighlightElement(CfDrawingElement *element);
};

