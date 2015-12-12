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

//CfDrawingElement

wxString CfDrawingElement::ElementTypeArray[] = {_("ID_IMAGE"),_("ID_PORT"),_("ID_WIRE"),_("ID_SYMBOL"),_("ID_LAYER"),_("ID_INSTANCE"),_("")};

CfDrawingElement::CfDrawingElement(wxEvtHandler *p_event_handler)
	:m_sName(wxEmptyString), m_bModified(false)
,CfNotificationSender(p_event_handler)
,m_bDeleted(false)
,m_bHighlighted(false)
,m_pShape(NULL)
{
}

CfDrawingElement::CfDrawingElement(wxEvtHandler *p_event_handler,const wxString &id, const wxString &name, CfShape *shape)
	:m_sId(id),m_sName(name),m_bModified(false)
,CfNotificationSender(p_event_handler)
,m_pShape(shape)
,m_bDeleted(false)
,m_bHighlighted(false)
{
}


CfDrawingElement::~CfDrawingElement(void)
{
	if(m_pShape) delete m_pShape;
}


void CfDrawingElement::GetProperty(wxPropertyGridInterface *propertygrid)
{

}

//void CfDrawingElement::AddShape(CfShape *shape) 
//{
//	if(m_pShape) delete m_pShape;
//	m_pShape = shape;
//	//m_pShape->AddShape(shape);
//}

bool CfDrawingElement::Modified()
{
	return m_bModified;
}

void CfDrawingElement::SetDeleted(bool del)
{
	m_bDeleted = del;
	SetModified(del);
}

void CfDrawingElement::SetHighlighted(bool highlight)
{
	m_bHighlighted = highlight;
}

void CfDrawingElement::SetModified(bool modified)
{
	m_bModified = modified;	
	if(modified)
	{
		CfNotification *pNotification = new CfNotification(cfId_DrawingElement_Change);
		AddNotification(pNotification);
	}
}

void CfDrawingElement::SetName(wxString name)
{
	m_sName = name;
}

void CfDrawingElement::Draw(wxDC& dc,const CfScreenView &view)
{
	if(m_bDeleted || !m_bShowing) return;
	wxLogDebug(wxString::Format(_("Drawing %s\n"),m_sName));
	if(!m_pShape) return;
	m_pShape->Draw(dc,view,m_bHighlighted);
}

wxString CfDrawingElement::GetId() const
{
	return m_sId;
}
wxString CfDrawingElement::GetName() const
{
	return m_sName;
}

bool CfDrawingElement::Contains(wxPoint &point)
{
	if(!m_pShape) return false;
	return this->m_pShape->Contains(point);
}

void CfDrawingElement::Copy(CfDrawingElement* element,bool change_name)
{
	if(change_name)
		this->m_sName = element->GetName()+_("_Copy");
	m_pShape = element->GetShape()->Clone();
//	m_pShape->AddShape((*element->GetShapeArray())[0]);
}

bool CfDrawingElement::Within(wxRect &rect)
{
	if(!m_pShape) return false;
	return this->m_pShape->Within(rect);
}

bool CfDrawingElement::IsEndPoint(wxPoint &point)
{
	if(!m_pShape) return false;
	return this->m_pShape->IsEndPoint(point);
}

//bool CfDrawingElement::Merge(CfShape *shape)
//{
//	return this->m_pShape->Merge(shape);
//}
bool CfDrawingElement::IsDeleted()
{
	return m_bDeleted;
}

bool CfDrawingElement::IsHighlighted()
{
	return m_bHighlighted;
}

void CfDrawingElement::Move(int dx, int dy)
{
	if(!m_pShape) return;
	this->m_pShape->Move(dx,dy);
	SetModified();
}

void CfDrawingElement::SetProperty(wxPGProperty *pgproperty)
{

}

CfShape* CfDrawingElement::GetShape() 
{
	return m_pShape;
}
wxSize CfDrawingElement::GetSize()
{
	if(!m_pShape) return wxSize(0,0);
	return this->m_pShape->GetSize();
}

wxPoint CfDrawingElement::GetStartPos()
{
	if(!m_pShape) return wxPoint(0,0);
	return this->m_pShape->GetStartPos();
}

wxPoint CfDrawingElement::GetEndPos()
{
	if(!m_pShape) return wxPoint(0,0);
	return this->m_pShape->GetEndPos();
}

void CfDrawingElement::SetLineColor(wxColor color)
{
	if(!m_pShape) return;
	this->m_pShape->SetLineColor(color);
}

void CfDrawingElement::SetLineWidth(int width)
{
	if(!m_pShape) return;
	this->m_pShape->SetLineWidth(width);
}

void CfDrawingElement::SetFilledColor(wxColor color)
{
	if(!m_pShape) return;
	this->m_pShape->SetFilledColor(color);
}

wxColor CfDrawingElement::GetLineColor()
{
	if(!m_pShape) return *wxWHITE;
	return this->m_pShape->GetLineColor();
}
int CfDrawingElement::GetLineWidth()
{
	if(!m_pShape) return 0;
	return this->m_pShape->GetLineWidth();
}
wxColor CfDrawingElement::GetFilledColor()
{
	if(!m_pShape) return *wxWHITE;
	return this->m_pShape->GetFilledColor();
}

CfDrawingElement* CfDrawingElement::LoadFromFile(wxEvtHandler *p_event_handler,wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString line;
	int count;
	CfDrawingElement *element=NULL;
		
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
		//tis.ReadLine();		// [/Image] ???
	}
	else if(line == wxString::Format(_("[%s]"),cfSTRING_SYMBOL))
	{
		element = new CfSymbol(p_event_handler);
		element->Load(fis);
		tis.ReadLine();		// [/Symbol]
	}
	else if(line == wxString::Format(_("[%s]"),cfSTRING_LAYER))
	{
		element = new CfLayer(p_event_handler);
		element->Load(fis);
		tis.ReadLine();		// [/Layer]
	}
	else if(line == wxString::Format(_("[%s]"),cfSTRING_SOLIDCOLOR_BACKGROUND))
	{
		element = new CfSolidColorBackground(p_event_handler);
		element->Load(fis);
		//tis.ReadLine();		// [/SolidColorBackground]
	}


	if(element)
	{
		//if shape is empty, drop the element
		CfShape* shape= element->GetShape();
		if((shape==NULL) && (element->GetTypeId() != ID_ELEMENT_IMAGE)
			&& (element->GetTypeId() != ID_ELEMENT_LAYER)
			&& (element->GetTypeId() != ID_ELEMENT_SOLIDCOLORBACKGROUND))
		{
			delete element;
			element = NULL;
		}
	}
	return element;

}