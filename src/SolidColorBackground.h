
/////////////////////////////////////////////////////////////////////////////////
//
// Name        : SolidColorBackground.h
// Author      : MING
// Created     : 2015Äê10ÔÂ5ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "DrawingElement.h"


class CfSolidColorBackground : public CfDrawingElement
{
public:
	CfSolidColorBackground(wxEvtHandler *p_event_handler);
	//CfSolidColorBackground(wxEvtHandler *p_event_handler,const wxString &id, const wxString &name, CfRectangle *prect);
	CfSolidColorBackground(wxEvtHandler *p_event_handler,wxColor filled_color);
	int GetTypeId()const {return ID_ELEMENT_SOLIDCOLORBACKGROUND;}
	~CfSolidColorBackground(void);
	wxColor GetColor() const {return m_FilledColor;};

	//IDrawable
	virtual void Draw(wxDC& dc,const CfScreenView &view);
	virtual wxPoint GetEndPos() {return wxPoint(0,0);};

	//IProperty
	virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication =false);
	virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties);
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset=wxPoint(0,0));
	virtual void Load(wxFileInputStream *fis, const wxPoint offset=wxPoint(0,0));


private:
	wxColor	m_FilledColor;
};


