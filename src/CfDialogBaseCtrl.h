/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfDialogBaseCtrl.h
// Author      :James
// Created     :2010-03-18 ¿ÀÈÄ 2:59:49
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CfWx.h"

class CfDialogBaseCtrl : public wxDialog//, public wxEvtHandler
{

public:
	CfDialogBaseCtrl(wxWindow* pParent,const wxChar* title,const wxSize &size,const wxChar* caption);
	virtual ~CfDialogBaseCtrl(void){};

protected:

	wxBitmap m_HeaderImage;
	wxString m_strCaption;

	DECLARE_EVENT_TABLE()

	void SetCaption(wxString caption);
	void OnPaint(wxPaintEvent& event);
};


