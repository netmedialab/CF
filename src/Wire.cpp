#include "CfDefines.h"
#include "Wire.h"
#include "EnvironmentUtil.h"
//CfWire
CfWire::CfWire(wxEvtHandler *p_event_handler)
:CfDrawingElement(p_event_handler)
{
}

CfWire::CfWire(wxEvtHandler *p_event_handler,const wxString &id, const wxString &name, CfLineSegment *prect)
	:CfDrawingElement(p_event_handler,id,id,prect)
	//,m_iWidth(1)
{
	CfDrawingElement::SetLineColor(CfEnvironmentUtil::Get()->GetString(PROP_SETTING_WIRE_DEFAULTLINECOLOR,_T("Blue")));
	CfDrawingElement::SetLineWidth(wxAtoi(CfEnvironmentUtil::Get()->GetString(PROP_SETTING_WIRE_DEFAULTLINEWIDTH,_T("3"))));
	CfDrawingElement::SetFilledColor(CfEnvironmentUtil::Get()->GetString(PROP_SETTING_WIRE_DEFAULTFILLEDCOLOR,_T("None")));
}
CfWire::CfWire(const wxString &id, const wxString &name, CfWire& wire)
	:CfDrawingElement(wire.m_pEventHandler,id,name,NULL)
{
	CfShape* shape= wire.GetShape();
	//for(size_t i=0;i<shape_array->Count();i++)
	//{
	//	m_pShape->AddShape((*shape_array)[i]->Clone());
	//}

}
CfWire::~CfWire()
{

}

//void CfWire::AddShape(CfShape *shape) 
//{
////	if(m_pShape) delete m_pShape;
////	m_pShape = shape;
////	//m_pShape->AddShape(shape);
//}

//IProperty
void CfWire::GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication)
{
	PropertyUnit *punit;
	wxString group= cfSTRING_WIRE;
	punit = new PropertyUnit(cfSTRING_WIRE,wxEmptyString,cfSTRING_GROUP);
	properties->Add(punit);
	punit = new PropertyUnit(cfSTRING_ID,m_sId,wxGRID_VALUE_STRING,true);
	properties->Add(punit);
	punit = new PropertyUnit(cfSTRING_NAME,m_sName,wxGRID_VALUE_STRING);
	properties->Add(punit);
	//punit = new PropertyUnit(cfSTRING_WIDTH,wxString::Format(_("%d"),this->m_iWidth),wxGRID_VALUE_STRING);
	properties->Add(punit);

	m_pShape->GetPropertyUnitPtrArray(properties,true);

}

void CfWire::SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties)
{
	PropertyUnit *punit;
	for(size_t i=0;i<properties->Count();i++)
	{
		punit = (*properties)[i];
		if(punit->Name == cfSTRING_ID) m_sId = punit->Value;
		else if(punit->Name == cfSTRING_NAME) m_sName = punit->Value;
		//else if(punit->Name == cfSTRING_WIDTH) m_iWidth= wxAtoi(punit->Value);
	}
	m_pShape->SetPropertyUnitPtrArray(properties);
	this->SetModified();
}

void CfWire::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_WIRE);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[%s]\n"),cfSTRING_COMMON);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_ID,m_sId);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_NAME,m_sName);
	fos->Write(line.char_str(),line.size());
	//line = wxString::Format(_("%s=%d\n"),cfSTRING_WIDTH,m_iWidth);
	//fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_COMMON);
	fos->Write(line.char_str(),line.size());
	m_pShape->Dump(fos,offset);
	line = wxString::Format(_("[/%s]\n"),cfSTRING_WIRE);
	fos->Write(line.char_str(),line.size());

	this->SetModified(false);
}
void CfWire::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line = tis.ReadLine().Trim().Trim(true);
	if(line != wxString::Format(_("[%s]"),cfSTRING_COMMON))
	{
		wxLogError(_("Parsing error in Wire: %s"),line);
		return;
	}
	while (fis->CanRead())
	{
		line = tis.ReadLine().Trim().Trim(true);
		if(line == wxString::Format(_("[/%s]"),cfSTRING_COMMON))
			break;
		name = line.Before('=');
		value = line.After('=');
		if(name == cfSTRING_ID) m_sId = value;
		else if(name == cfSTRING_NAME) m_sName = value;
		//else if(name == cfSTRING_WIDTH) m_iWidth = wxAtoi(value);
	}

	m_pShape = new CfLineSegment();
	m_pShape->Load(fis,offset);
}

void CfWire::Merge(CfShape *shape)
{
	//return this->m_pShape->Merge(shape);
	m_pShape->Merge(shape);
	//return true;
}