#include "SolidColorBackground.h"
#include "CfDefines.h"
#include "CfLogMessageCtrl.h"

CfSolidColorBackground::CfSolidColorBackground(wxEvtHandler *p_event_handler)
:CfDrawingElement(p_event_handler,wxEmptyString,wxEmptyString,NULL)
{
}


//CfSolidColorBackground::CfSolidColorBackground(wxEvtHandler *p_event_handler,const wxString &id, const wxString &name, CfRectangle *prect)
CfSolidColorBackground::CfSolidColorBackground(wxEvtHandler *p_event_handler,wxColor filled_color)
:CfDrawingElement(p_event_handler,wxEmptyString,wxEmptyString,NULL)
,m_FilledColor(filled_color)
{
}

CfSolidColorBackground::~CfSolidColorBackground(void)
{
}

//IProperty
void CfSolidColorBackground::GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication)
{
	PropertyUnit *punit;
	wxString group= cfSTRING_SOLIDCOLOR_BACKGROUND;
	punit = new PropertyUnit(cfSTRING_SOLIDCOLOR_BACKGROUND,wxEmptyString,cfSTRING_GROUP);
	properties->Add(punit);

	m_pShape->GetPropertyUnitPtrArray(properties,true);
}

void CfSolidColorBackground::SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties)
{
	m_pShape->SetPropertyUnitPtrArray(properties);
	this->SetModified();
}

void CfSolidColorBackground::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_SOLIDCOLOR_BACKGROUND);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_FILLED_COLOR,m_FilledColor.GetAsString());
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_SOLIDCOLOR_BACKGROUND);
	fos->Write(line.char_str(),line.size());

	this->SetModified(false);
}

void CfSolidColorBackground::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line;// = tis.ReadLine().Trim().Trim(true);
	while (fis->CanRead())
	{
		line = tis.ReadLine().Trim().Trim(true);
		if(line == wxString::Format(_("[/%s]"),cfSTRING_SOLIDCOLOR_BACKGROUND))
			break;
		name = line.Before('=');
		value = line.After('=');
		if(name == cfSTRING_FILLED_COLOR) m_FilledColor.Set(value);
	}
}

void CfSolidColorBackground::Draw(wxDC& dc,const CfScreenView &view)
{
	if(m_bDeleted || !m_bShowing) return;
	dc.Clear();
	//dc.SetPen(wxPen(m_FilledColor,3));
	//dc.SetBrush(wxBrush(m_FilledColor));
	//dc.DrawRectangle(view.Rectangle);
}