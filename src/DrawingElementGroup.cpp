#include "CfDefines.h"
#include "DrawingElement.h"
#include "Port.h"
#include "Wire.h"
#include "Symbol.h"
#include "Image.h"
#include "Layer.h"
#include "Instance.h"
#include "SolidColorBackground.h"
#include "Project.h"
#include "DrawingElementFactory.h"



//DrawingElementGroup

CfDrawingElementGroup::CfDrawingElementGroup(bool detached)
	:m_bDetached(detached)
{

}

CfDrawingElementGroup::~CfDrawingElementGroup()
{
	if(m_bDetached) return;
	CfDrawingElement *element = NULL;
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		element = m_ElementArray[i];
		delete m_ElementArray[i];
		m_ElementArray[i] = NULL;
		element = NULL;
	}
	m_ElementArray.Clear();
}

void CfDrawingElementGroup::Add(CfDrawingElement * element)
{
	if(Find(element) == NULL)
		m_ElementArray.Add(element);
}

void CfDrawingElementGroup::Add(CfDrawingElementGroup *element_group)
{
	for(size_t i=0;i<element_group->Count();i++)
	{
		this->Add((*element_group)[i]);
	}
}

void CfDrawingElementGroup::Insert(CfDrawingElement * element, size_t pos)
{
	m_ElementArray.Insert(element,pos);
}

void CfDrawingElementGroup::GetContainedElements(wxRect &rect, CfDrawingElementGroup *element_group)
{
	for(size_t i=0;i<m_ElementArray.Count();i++)
	{
		if(m_ElementArray[i]->IsDeleted()) continue;
		if(m_ElementArray[i]->Within(rect))
			element_group->Add(m_ElementArray[i]);
	}
	
}

CfDrawingElement* CfDrawingElementGroup::GetElement(wxPoint &point)
{
	CfDrawingElement* element = NULL;
	for(int i=m_ElementArray.Count()-1;i>=0;i--)
	{
		if(!m_ElementArray[i]->IsDeleted() && m_ElementArray[i]->IsShowing())
		{
			if(m_ElementArray[i]->Contains(point))	element = m_ElementArray[i];
			if((m_ElementArray[i]->GetTypeId() == ID_ELEMENT_INSTANCE) ||
			 (m_ElementArray[i]->GetTypeId() == ID_ELEMENT_SYMBOL))
			{
				CfDrawingElement *child = ((CfSymbol *)m_ElementArray[i])->GetElement(point);
				if(child) element = child;
			}
		}
	}
	return element;
}


//IProperty
//void CfDrawingElementGroup::GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication)
//{
//	for(size_t i=0;i<m_ElementArray.Count();i++)
//	{
//		m_ElementArray[i]->GetPropertyUnitPtrArray(properties,check_name_duplication);
//	}
//}
//void CfDrawingElementGroup::SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties)
//{
//	for(size_t i=0;i<m_ElementArray.Count();i++)
//	{
//		m_ElementArray[i]->SetPropertyUnitPtrArray(properties);
//	}
//}

size_t CfDrawingElementGroup::Count()
{
	return m_ElementArray.Count();
}

void CfDrawingElementGroup::Clear()
{
	m_ElementArray.Clear();
}

void CfDrawingElementGroup::Copy(CfDrawingElementGroup* element_group)
{
	for(size_t i=0;i<element_group->Count();i++)
	{
		m_ElementArray.Add((*element_group)[i]);
	}
}

void CfDrawingElementGroup::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	for(size_t i=0;i<m_ElementArray.Count();i++)
	{
		if(m_ElementArray[i]->IsDeleted()) m_ElementArray[i]->SetModified(false);
		else
			m_ElementArray[i]->Dump(fos,offset);
	}

}

bool CfDrawingElementGroup::Delete(CfDrawingElement *element, bool delete_element)
{
	if(!element) return false;
	int i=0;
	for(;i<m_ElementArray.Count();i++)
	{
		if((*m_ElementArray[i]) == (*element))
		{
			break;
		}
	}
	if(i>=m_ElementArray.Count()) return false;
	//if(m_bDetached) 
	if(!delete_element) 
	{
		m_ElementArray.RemoveAt(i);
		return true;
	}
	else
	{
		wxMessageDialog dlg(NULL,wxT("Delete element '")+element->GetId()+_("'?"),PROJECT_CAPTION,wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
		int id = dlg.ShowModal();
		if(id == wxID_YES)
		{
			m_ElementArray.RemoveAt(i);
			delete element;
			return true;
		}
		return false;
	}
}


CfDrawingElement* CfDrawingElementGroup::Find(CfDrawingElement *element) const
{
	if(element == NULL) return NULL;
	CfDrawingElement *res_element = NULL;
	for(size_t i=0;i<m_ElementArray.Count();i++)
	{
		res_element = m_ElementArray[i];
		if(*res_element == *element) break;
		if((res_element->GetTypeId() == ID_ELEMENT_INSTANCE)
		|| (res_element->GetTypeId() == ID_ELEMENT_SYMBOL)
		|| (res_element->GetTypeId() == ID_ELEMENT_LAYER))
		{
			res_element = dynamic_cast<IElementWithChildren*>(res_element)->FindElement(element);
			if(res_element != NULL) break;
		}
		res_element = NULL;
	}
	return res_element;
}

CfDrawingElement* CfDrawingElementGroup::Find(wxString &element_id) const
{
	CfDrawingElement *element = NULL, *src_element;
	for(size_t i=0;i<m_ElementArray.Count();i++)
	{
		src_element = m_ElementArray[i];
		if(src_element->GetId() == element_id)
		{
			element = m_ElementArray[i];
			break;
		}
		if((src_element->GetTypeId() == ID_ELEMENT_INSTANCE)
		|| (src_element->GetTypeId() == ID_ELEMENT_SYMBOL)
		|| (src_element->GetTypeId() == ID_ELEMENT_LAYER))
		{
			element = dynamic_cast<IElementWithChildren*>(src_element)->FindElement(element_id);
			if(element != NULL)
				break;
		}
	}
	return element;
}
CfDrawingElement *g_element;
void CfDrawingElementGroup::Load(wxEvtHandler *p_event_handler,wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString line;
	int count;
	CfDrawingElement *element=NULL;
    while (fis->CanRead())
    {
		line = tis.ReadLine().Trim().Trim(true);
		if(line == wxString::Format(_("[%s]"),cfSTRING_PORT))
		{
			element = new CfPort(p_event_handler);
			element->Load(fis);
			tis.ReadLine();		// [/Port]
		}
		else if(line == wxString::Format(_("[%s]"),cfSTRING_WIRE))
		{
			element = new CfWire(p_event_handler);
			element->Load(fis);
			tis.ReadLine();		// [/Wire]
		}
		else if(line == wxString::Format(_("[%s]"),cfSTRING_IMAGE))
		{
			element = new CfImage(p_event_handler);
			element->Load(fis);
			g_element = element;
			//tis.ReadLine();		// [/Image] ???
		}
		else if(line == wxString::Format(_("[%s]"),cfSTRING_SYMBOL))
		{
			element = new CfSymbol(p_event_handler);
			element->Load(fis);
			tis.ReadLine();		// [/Symbol]
		}
		else if(line == wxString::Format(_("[%s]"),cfSTRING_INSTANCE))
		{
			element = new CfInstance(p_event_handler);
			element->Load(fis);
			tis.ReadLine();		// [/Instance]
		}
		else if(line == wxString::Format(_("[%s]"),cfSTRING_LAYER))
		{
			element = new CfLayer(p_event_handler);
			element->Load(fis);
			tis.ReadLine();		// [/Symbol]
		}
		else if(line == wxString::Format(_("[/%s]"),cfSTRING_ELEMENTS))
			break;

		if(element)
		{
			//if shape is empty, drop the element
			CfShape* shape= element->GetShape();
			if((shape==NULL) && (element->GetTypeId() != ID_ELEMENT_IMAGE)
				&& (element->GetTypeId() != ID_ELEMENT_LAYER))
			{
				delete element;
				element = NULL;
				continue;
			}

			m_ElementArray.Add(element);
			sscanf(element->GetId(),CfDrawingElement::ElementTypeArray[element->GetTypeId()]+"%d",&count);
			DoSetElementIdCount(element->GetTypeId(),count+1);
			element = NULL;
		}
    }
}

void CfDrawingElementGroup::Move(int dx, int dy)
{
	for(size_t i=0;i<m_ElementArray.Count();i++)
	{
		m_ElementArray[i]->Move(dx,dy);
	}
}

void CfDrawingElementGroup::SetDeleted(bool del)
{
	for(size_t i=0;i<m_ElementArray.Count();i++)
	{
		m_ElementArray[i]->SetDeleted(del);
	}
}
//
//void CfDrawingElementGroup::RemoveAt(size_t pos)
//{
//	m_ElementArray.RemoveAt(pos);
//}


//CfDrawingElementWithChildren
	
CfDrawingElementWithChildren::CfDrawingElementWithChildren()
:m_ElementArray(false)
{
}

CfDrawingElementWithChildren::~CfDrawingElementWithChildren()
{
}

void CfDrawingElementWithChildren::AddElement(CfDrawingElement* element)
{
	m_ElementArray.Add(element);
}