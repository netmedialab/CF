
/////////////////////////////////////////////////////////////////////////////////
//
// Name        : LayerViewCtrl.h
// Author      : MING
// Created     : 2015Äê9ÔÂ1ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "wx/treelist.h"
#include "CfDefines.h"
#include "Project.h"
#include "Workspace.h"
#include "EnvironmentUtil.h"
#include "Notification.h"
#include "DrawingElement.h"
#include "DrawingElementGroup.h"
#include "HighlightElementList.h"

class CfLayerViewCtrl
	: public wxTreeListCtrl, public CfNotificationSender
	, public CfHighlightElementList
{
public:
	CfLayerViewCtrl(wxWindow *parent);
	~CfLayerViewCtrl(void);

	void Reload();
	void LoadElement(IElementWithChildren* ewc);

private:
	IElementWithChildren *m_pElementWithChildren;
	//CfDrawingElementWithChildren *m_pElementWithChildren;
	wxTreeListItem m_ActiveLayerItem;
    wxDECLARE_EVENT_TABLE();
    void OnItemChecked(wxTreeListEvent& event);
    void OnItemActivated(wxTreeListEvent& event);
    void OnItemSelectionChanged(wxTreeListEvent& event);
	void AddElementNode(IElementGroup *element_group, wxTreeListItem parent);

	void HighlightElement(CfDrawingElement *element);
};

