#include "CfDefines.h"
#include "Layer.h"
#include "Port.h"
#include "Wire.h"
#include "DrawingElementFactory.h"

 //CfLayer
CfLayer::CfLayer(wxEvtHandler *p_event_handler)
:CfDrawingElement(p_event_handler)
,m_pBackground(NULL)
{
}
CfLayer::CfLayer(wxEvtHandler *p_event_handler,const wxString &id, const wxString &name)
	:CfDrawingElement(p_event_handler,id,name,NULL)
,m_pBackground(NULL)
{
	//enumMap["Name"]=name;
	//enumMap["RectangleWidth"]=wxString::Format(wxT("%i"),prect->AbsoluteRect.GetSize().GetWidth());
	//enumMap["RectangleHeight"]=wxString::Format(wxT("%i"),prect->AbsoluteRect.GetSize().GetHeight());
	//enumMap["FilledColor"]=wxT("WHITE");
	//enumMap["BorderColor"]=wxT("BLACK");
	//enumMap["LineWidth"]=wxString::Format(wxT("%i"),prect->LineWidth);
	//enumMap["m_TypeName"]=m_TypeName;
}

CfLayer::CfLayer(const wxString &id, const wxString &name, CfLayer& layer)
	:CfDrawingElement(layer.m_pEventHandler,id,name,NULL)
,m_pBackground(false) // this is a problem that will be handled later!!! (remove shapearry from element, use shapearray only in wire)
{
	//CfDrawingElementGroup &elements = layer.GetElementArray();
	//for(size_t i=0;i<elements.Count();i++)
	//{
	//	m_ElementArray.Add(DoCopyDrawingElement(elements[i]));
	//}

}

CfLayer::~CfLayer()
{
	if(m_pBackground)
		delete m_pBackground;
	//for(size_t i=0; i<m_ElementArray.Count();i++)
	//{
	//	delete m_ElementArray[i];
	//}
}

void CfLayer::ShowBackground(bool show)
{
	if(m_pBackground) m_pBackground->SetShowing(show);
}

void CfLayer::ShowElements(bool show)
{
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		m_ElementArray[i]->SetShowing(show);
	}
}

void CfLayer::SetBackground(CfDrawingElement* element)
{
	if(m_pBackground) delete m_pBackground;
	m_pBackground = element;
	this->SetModified();
}


void CfLayer::Copy(CfDrawingElement* element,bool change_name)
{
	CfDrawingElement::Copy(element,change_name);
	this->m_ElementArray.Copy(((CfLayer*)element)->GetElementArray());
}

CfDrawingElement* CfLayer::GetBackground() const
{
	return m_pBackground;
}

wxPoint CfLayer::GetEndPos()
{
	int max_x = 0,max_y = 0;
	wxPoint point;
	if(m_pBackground)
	{
		point = m_pBackground->GetEndPos();
		max_x = point.x;
		max_y = point.y;
	}

	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		point = m_ElementArray[i]->GetEndPos();
		if(max_x < point.x) max_x = point.x;
		if(max_y < point.y) max_y = point.y;
	}
	return wxPoint(max_x,max_y);
}

void CfLayer::DrawBackground(wxDC& dc,const CfScreenView &view)
{
	if(m_bDeleted || !IsShowing()) return;
	if(m_pBackground) m_pBackground->Draw(dc,view);
}

void CfLayer::Draw(wxDC& dc,const CfScreenView &view)
{
	if(m_bDeleted || !IsShowing()) return;
	bool highlight = false;

	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		m_ElementArray[i]->Draw(dc,view);
	}
}

bool CfLayer::IsShowing()
{
	m_bShowing = false;
	if(m_pBackground) m_bShowing |= m_pBackground->IsShowing();
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		if(m_ElementArray[i]->IsDeleted()) continue;
		m_bShowing |= m_ElementArray[i]->IsShowing();
	}
	return m_bShowing;
}

bool CfLayer::IsElementShowing()
{
	bool showing = false;
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		if(m_ElementArray[i]->IsDeleted()) continue;
		showing |= m_ElementArray[i]->IsShowing();
	}
	return showing;
}
//IProperty
void CfLayer::GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication)
{
	PropertyUnit *punit;
	wxString group= cfSTRING_SYMBOL;
	punit = new PropertyUnit(cfSTRING_LAYER,wxEmptyString,cfSTRING_GROUP);
	properties->Add(punit);
	punit = new PropertyUnit(cfSTRING_ID,m_sId,cfGRID_VALUE_STRING,true);
	properties->Add(punit);
	punit = new PropertyUnit(cfSTRING_NAME,m_sName,cfGRID_VALUE_STRING);
	properties->Add(punit);

	//Get Parameters

}

void CfLayer::SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties)
{
	PropertyUnit *punit;
	for(size_t i=0;i<properties->Count();i++)
	{
		punit = (*properties)[i];
		if(punit->Name == cfSTRING_ID) m_sId = punit->Value;
		else if(punit->Name == cfSTRING_NAME) m_sName = punit->Value;
	}

	//Set parameters

}

void CfLayer::SetShowing(bool showing)
{
	//m_bShowing = showing;
	if(m_pBackground) m_pBackground->SetShowing(showing);
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		m_ElementArray[i]->SetShowing(showing);
	}
}
void CfLayer::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_LAYER);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[%s]\n"),cfSTRING_COMMON);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_ID,m_sId);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_NAME,m_sName);
	fos->Write(line.char_str(),line.size());

	//Dump background
	line = wxString::Format(_("[%s]\n"),cfSTRING_BACKGROUND);
	fos->Write(line.char_str(),line.size());
	if(m_pBackground) m_pBackground->Dump(fos,offset);
	line = wxString::Format(_("[/%s]\n"),cfSTRING_BACKGROUND);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_COMMON);
	fos->Write(line.char_str(),line.size());
	
	//Dump parameters

	//Dump elements
	line = wxString::Format(_("[%s]\n"),cfSTRING_ELEMENTS);
	fos->Write(line.char_str(),line.size());
	m_ElementArray.Dump(fos,offset);
	line = wxString::Format(_("[/%s]\n"),cfSTRING_ELEMENTS);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_LAYER);
	fos->Write(line.char_str(),line.size());

	this->SetModified(false);
}
void CfLayer::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line = tis.ReadLine().Trim().Trim(true);
	if(line == wxString::Format(_("[%s]"),cfSTRING_LAYER))
			line = tis.ReadLine().Trim().Trim(true);

	if(line == wxString::Format(_("[%s]"),cfSTRING_COMMON))
	{
		while (fis->CanRead())
		{
			line = tis.ReadLine().Trim().Trim(true);
			if(line == wxString::Format(_("[%s]"),cfSTRING_BACKGROUND))
			{
				m_pBackground = CfDrawingElement::LoadFromFile(m_pEventHandler,fis);
				if(m_pBackground)	tis.ReadLine();		// [/Background]
			}
			else if(line == wxString::Format(_("[/%s]"),cfSTRING_COMMON))
				break;
			name = line.Before('=');
			value = line.After('=');
			if(name == cfSTRING_ID) m_sId = value;
			else if(name == cfSTRING_NAME) m_sName = value;
		}
    }

	//Load parameters

	//Load elements
	line = tis.ReadLine().Trim().Trim(true);
	if(line == wxString::Format(_("[%s]"),cfSTRING_ELEMENTS))
		m_ElementArray.Load(m_pEventHandler,fis);

	//CfDrawingElement *element=NULL;
	//int count;
	//line = tis.ReadLine().Trim().Trim(true);
	//if(line == wxString::Format(_("[%s]"),cfSTRING_ELEMENTS))
	//{ 
	//	while (fis->CanRead())
	//	{
	//		line = tis.ReadLine().Trim().Trim(true);
	//		if(line == wxString::Format(_("[%s]"),cfSTRING_PORT))
	//		{
	//			element = new CfPort(m_pEventHandler);
	//			element->Load(fis);
	//			tis.ReadLine();		// [/Port]
	//		}
	//		else if(line == wxString::Format(_("[%s]"),cfSTRING_WIRE))
	//		{
	//			element = new CfWire(m_pEventHandler);
	//			element->Load(fis);
	//			tis.ReadLine();		// [/Wire]
	//		}
	//		else if(line == wxString::Format(_("[/%s]"),cfSTRING_ELEMENTS))
	//			break;
	//		if(element)
	//		{
	//			m_ElementArray.Add(element);
	//			//Read ID counter, used in element factory to make sure generating the unique ID
	//			sscanf(element->GetId(),CfDrawingElement::ElementTypeArray[element->GetTypeId()]+"%d",&count);
	//			DoSetElementIdCount(element->GetTypeId(),count+1);
	//			element = NULL;
	//		}
	//	}
	//}

	
}


bool CfLayer::Modified()
{
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		m_bModified |= m_ElementArray[i]->Modified();
	}
	return m_bModified;
}

void CfLayer::Move(int dx, int dy)
{
	//CfDrawingElement::Move(dx,dy);
	//m_ElementArray.Move(dx,dy);
}