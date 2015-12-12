#include "CfDefines.h"
#include "Port.h"
#include "EnvironmentUtil.h"
// CfPort
CfPort::CfPort(wxEvtHandler *p_event_handler)
:CfDrawingElement(p_event_handler)
{
	m_bDirectionChanged = false;
}

CfPort::CfPort(wxEvtHandler *p_event_handler,const wxString &id, const wxString &name, CfPortShape *prect)
	:CfDrawingElement(p_event_handler,id,name,prect)
	,m_sDirection(_("Input"))/*,m_iWidth(1)*/,m_bDirectionChanged(false)
{
	CfDrawingElement::SetLineColor(CfEnvironmentUtil::Get()->GetString(PROP_SETTING_PORT_DEFAULTLINECOLOR,_T("Blue")));
	CfDrawingElement::SetLineWidth(wxAtoi(CfEnvironmentUtil::Get()->GetString(PROP_SETTING_PORT_DEFAULTLINEWIDTH,_T("3"))));
	CfDrawingElement::SetFilledColor(CfEnvironmentUtil::Get()->GetString(PROP_SETTING_PORT_DEFAULTFILLEDCOLOR,_T("None")));

}

CfPort::CfPort(const wxString &id, const wxString &name, const CfPort& port)
	:CfDrawingElement(port.m_pEventHandler,id,name,port.m_pShape->Clone())
	,m_sDirection(port.m_sDirection),m_bDirectionChanged(port.m_bDirectionChanged)
{
}

wxString CfPort::GetDirection() const
{
	return m_sDirection;
}
//int CfPort::GetWidth() 
//{
//	return m_iWidth;
//}
CfPort::~CfPort()
{

}

void CfPort::Draw(wxDC& dc,const CfScreenView &view)
{
	if(this->m_bDirectionChanged)
		this->ChangeDirection();
	CfDrawingElement::Draw(dc, view);
}
void CfPort::ChangeDirection()
{
	CfShape *shape = m_pShape;
	CfShape *newshape;

	int x, y, width, height,linewidth;
	wxColor linecolor,filledcolor;
	x =shape->GetStartPos().x;
	y = shape->GetStartPos().y;
	width = shape->GetSize().x;
	height = shape->GetSize().y;
	linecolor = shape->GetLineColor();
	linewidth = shape->GetLineWidth();
	filledcolor = shape->GetFilledColor();

	if(this->m_sDirection == _("Input"))
		newshape = new CfInputPort(x, y, width, height);
	else if(this->m_sDirection == _("Output"))
		newshape = new CfOutputPort(x, y, width, height);
	else if(this->m_sDirection == _("Inout"))
		newshape = new CfInoutPort(x, y, width, height);

	
	newshape->SetLineColor(linecolor);
	newshape->SetLineWidth(linewidth);
	newshape->SetFilledColor(filledcolor);
	
	delete shape;
	m_pShape = newshape;
	m_bDirectionChanged = false;

}
//IProperty
void CfPort::GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication)
{
	PropertyUnit *punit;
	wxString group= cfSTRING_PORT;
	punit = new PropertyUnit(cfSTRING_PORT,wxEmptyString,cfSTRING_GROUP);
	properties->Add(punit);
	punit = new PropertyUnit(cfSTRING_ID,m_sId,cfGRID_VALUE_STRING,true);
	properties->Add(punit);
	punit = new PropertyUnit(cfSTRING_NAME,m_sName,cfGRID_VALUE_STRING);
	properties->Add(punit);
	//punit = new PropertyUnit(cfSTRING_DIRECTION,this->m_sDirection,cfGRID_VALUE_CHOICE,false,cfPortStringList,WXSIZEOF(cfPortStringList));
	//properties->Add(punit);
	//punit = new PropertyUnit(cfSTRING_PORT_WIDTH,wxString::Format(_("%d"),this->m_iWidth),cfGRID_VALUE_STRING);
	properties->Add(punit);

	m_pShape->GetPropertyUnitPtrArray(properties,true);

}

void CfPort::SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties)
{
	PropertyUnit *punit;
	for(size_t i=0;i<properties->Count();i++)
	{
		punit = (*properties)[i];
		if(punit->Name == cfSTRING_ID) m_sId = punit->Value;
		else if(punit->Name == cfSTRING_NAME) m_sName = punit->Value;
		else if(punit->Name == cfSTRING_DIRECTION) m_sDirection= punit->Value;
		//else if(punit->Name == cfSTRING_WIDTH) m_iWidth= wxAtoi(punit->Value);
	}
	m_pShape->SetPropertyUnitPtrArray(properties);
	this->SetModified();
}

void CfPort::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_PORT);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[%s]\n"),cfSTRING_COMMON);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_ID,m_sId);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_NAME,m_sName);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_DIRECTION,m_sDirection);
	fos->Write(line.char_str(),line.size());
	//line = wxString::Format(_("%s=%d\n"),cfSTRING_PORT_WIDTH,m_iWidth);
	//fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_COMMON);
	fos->Write(line.char_str(),line.size());
	m_pShape->Dump(fos,offset);
	line = wxString::Format(_("[/%s]\n"),cfSTRING_PORT);
	fos->Write(line.char_str(),line.size());

	this->SetModified(false);
}
void CfPort::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line = tis.ReadLine().Trim().Trim(true);
	if(line == wxString::Format(_("[%s]"),cfSTRING_COMMON))
	{
		while (fis->CanRead())
		{
			line = tis.ReadLine().Trim().Trim(true);
			if(line == wxString::Format(_("[/%s]"),cfSTRING_COMMON))
				break;
			name = line.Before('=');
			value = line.After('=');
			if(name == cfSTRING_ID) m_sId = value;
			else if(name == cfSTRING_NAME) m_sName = value;
			else if(name == cfSTRING_DIRECTION) m_sDirection = value;
			//else if(name == cfSTRING_PORT_WIDTH) m_iWidth = wxAtoi(value);
		}
    }
	//m_pShape->AddShape(shape);
	if(this->m_sDirection == _("Input"))
		m_pShape = new CfInputPort();
	else if(this->m_sDirection == _("Output"))
		m_pShape = new CfOutputPort();
	else if(this->m_sDirection == _("Inout"))
		m_pShape = new CfInoutPort();
	m_pShape->Load(fis,offset);
}
