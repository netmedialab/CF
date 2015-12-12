#include "CfDefines.h"
#include "Symbol.h"
#include "Port.h"
#include "Wire.h"
#include "DrawingElementFactory.h"
#include "EnvironmentUtil.h"
 //CfSymbol
CfSymbol::CfSymbol(wxEvtHandler *p_event_handler)
:CfDrawingElement(p_event_handler)
{
}
CfSymbol::CfSymbol(wxEvtHandler *p_event_handler,const wxString &id, const wxString &name, CfRectangle *prect)
	:CfDrawingElement(p_event_handler,id,id,prect)
{
	CfDrawingElement::SetLineColor(CfEnvironmentUtil::Get()->GetString(PROP_SETTING_SYMBOL_DEFAULTLINECOLOR,_T("Black")));
	CfDrawingElement::SetLineWidth(wxAtoi(CfEnvironmentUtil::Get()->GetString(PROP_SETTING_SYMBOL_DEFAULTLINEWIDTH,_T("6"))));
	CfDrawingElement::SetFilledColor(CfEnvironmentUtil::Get()->GetString(PROP_SETTING_SYMBOL_DEFAULTFILLEDCOLOR,_T("None")));
	//CfDrawingElement::SetLineColor(_T("Black"));
	//CfDrawingElement::SetLineWidth(6);
	//enumMap["Name"]=name;
	//enumMap["RectangleWidth"]=wxString::Format(wxT("%i"),prect->AbsoluteRect.GetSize().GetWidth());
	//enumMap["RectangleHeight"]=wxString::Format(wxT("%i"),prect->AbsoluteRect.GetSize().GetHeight());
	//enumMap["FilledColor"]=wxT("WHITE");
	//enumMap["BorderColor"]=wxT("BLACK");
	//enumMap["LineWidth"]=wxString::Format(wxT("%i"),prect->LineWidth);
	//enumMap["m_TypeName"]=m_TypeName;
}

CfSymbol::CfSymbol(const wxString &id, const wxString &name, CfSymbol& symbol)
	:CfDrawingElement(symbol.m_pEventHandler,id,name,new CfRectangle((CfRectangle*) symbol.GetShape()))
{
	CfDrawingElementGroup *elements = symbol.GetElementArray();
	for(size_t i=0;i<elements->Count();i++)
	{
		m_ElementArray.Add(DoCopyDrawingElement((*elements)[i]));
	}

}
CfSymbol::~CfSymbol()
{
	//for(size_t i=0; i<m_ElementArray.Count();i++)
	//{
	//	delete m_ElementArray[i];
	//}
}

CfDrawingElement* CfSymbol::GetElement(wxPoint &point)
{
	CfDrawingElement *element = m_ElementArray.GetElement(point);
	if(!element) element = this->Contains(point)?this:NULL;

	return element;
}

bool CfSymbol::Contains(wxPoint &point)
{
	return m_pShape->Contains(point);
}

void CfSymbol::Copy(CfDrawingElement* element,bool change_name)
{
	CfDrawingElement::Copy(element,change_name);
	this->m_ElementArray.Copy(((CfSymbol*)element)->GetElementArray());
}

void CfSymbol::Draw(wxDC& dc,const CfScreenView &view)
{
	if(m_bDeleted || !m_bShowing) return;
	CfDrawingElement::Draw(dc,view);
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		m_ElementArray[i]->Draw(dc,view);
	}
}

//IProperty
//void CfSymbol::GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication)
//{
//	PropertyUnit *punit;
//	wxString group= cfSTRING_SYMBOL;
//	punit = new PropertyUnit(cfSTRING_SYMBOL,wxEmptyString,cfSTRING_GROUP);
//	properties->Add(punit);
//	punit = new PropertyUnit(cfSTRING_ID,m_sId,cfGRID_VALUE_STRING,true);
//	properties->Add(punit);
//	punit = new PropertyUnit(cfSTRING_NAME,m_sName,cfGRID_VALUE_STRING);
//	properties->Add(punit);
//
//	//Get Parameters
//
//	//Get Shape
//	m_pShape->GetPropertyUnitPtrArray(properties);
//
//}
//
//void CfSymbol::SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties)
//{
//	PropertyUnit *punit;
//	for(size_t i=0;i<properties->Count();i++)
//	{
//		punit = (*properties)[i];
//		if(punit->Name == cfSTRING_ID) m_sId = punit->Value;
//		else if(punit->Name == cfSTRING_NAME) m_sName = punit->Value;
//	}
//
//	//Set parameters
//
//	//Set shape
//	m_pShape->SetPropertyUnitPtrArray(properties);
//
//}

CfDrawingElement* CfSymbol::Find(CfDrawingElement *element)
{
	return m_ElementArray.Find(element);
}

void CfSymbol::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_SYMBOL);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[%s]\n"),cfSTRING_COMMON);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_ID,m_sId);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_NAME,m_sName);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_COMMON);
	fos->Write(line.char_str(),line.size());
	
	//Dump parameters
	m_Parameter.Dump(fos,offset);

	//Dump shape
	m_pShape->Dump(fos,offset);

	//Dump elements
	line = wxString::Format(_("[%s]\n"),cfSTRING_ELEMENTS);
	fos->Write(line.char_str(),line.size());
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		m_ElementArray[i]->Dump(fos,offset);
	}
	line = wxString::Format(_("[/%s]\n"),cfSTRING_ELEMENTS);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_SYMBOL);
	fos->Write(line.char_str(),line.size());

	this->SetModified(false);
}
void CfSymbol::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line = tis.ReadLine().Trim().Trim(true);
	if(line == wxString::Format(_("[%s]"),cfSTRING_SYMBOL))
			line = tis.ReadLine().Trim().Trim(true);

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
		}
    }

	//Load parameters
	m_Parameter.Load(fis,offset);

	//Load shape
	m_pShape = new CfRectangle();
	m_pShape->Load(fis,offset);

	//Load elements
	CfDrawingElement *element=NULL;
	int count;
	line = tis.ReadLine().Trim().Trim(true);
	if(line == wxString::Format(_("[%s]"),cfSTRING_ELEMENTS))
	{ 
		while (fis->CanRead())
		{
			line = tis.ReadLine().Trim().Trim(true);
			if(line == wxString::Format(_("[%s]"),cfSTRING_PORT))
			{
				element = new CfPort(m_pEventHandler);
				element->Load(fis);
				tis.ReadLine();		// [/Port]
			}
			else if(line == wxString::Format(_("[%s]"),cfSTRING_WIRE))
			{
				element = new CfWire(m_pEventHandler);
				element->Load(fis);
				tis.ReadLine();		// [/Wire]
			}
			else if(line == wxString::Format(_("[/%s]"),cfSTRING_ELEMENTS))
				break;
			if(element)
			{
				m_ElementArray.Add(element);
				//Read ID counter, used in element factory to make sure generating the unique ID
				sscanf(element->GetId(),CfDrawingElement::ElementTypeArray[element->GetTypeId()]+"%d",&count);
				DoSetElementIdCount(element->GetTypeId(),count+1);
				element = NULL;
			}
		}
	}

	
}

bool CfSymbol::Modified()
{
	if(m_bModified) return m_bModified;
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		m_bModified |= m_ElementArray[i]->Modified();
	}
	return m_bModified;
}

void CfSymbol::Move(int dx, int dy)
{
	CfDrawingElement::Move(dx,dy);
	m_ElementArray.Move(dx,dy);
}