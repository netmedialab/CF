#include "DrawingElementFactory.h"
#include "Port.h"
#include "Wire.h"
#include "Image.h"
#include "Symbol.h"
#include "Layer.h"
#include "Instance.h"


CfDrawingElementFactory *CfDrawingElementFactory::m_pDrawingElementFactory = NULL;
CfDrawingElementFactory::CfDrawingElementFactory(void)
{
	for(int i=0;i<ID_ELEMENT_COUNT;i++)
		m_ElementCountArray[i] = 0;
}


CfDrawingElementFactory::~CfDrawingElementFactory(void)
{
}

CfDrawingElementFactory *CfDrawingElementFactory::GetDrawingElementFactory()
{
	if(!m_pDrawingElementFactory)
		m_pDrawingElementFactory = new CfDrawingElementFactory();
	return m_pDrawingElementFactory;

}

void CfDrawingElementFactory::DestroyDrawingElementFactory()
{

	if(m_pDrawingElementFactory)
		delete m_pDrawingElementFactory;
}

CfDrawingElement* CfDrawingElementFactory::CopyDrawingElement(CfDrawingElement *source_element)
{
	const wxString copy = _("_Copy");
	CfDrawingElement *element = NULL;
	int type_id = source_element->GetTypeId();
	wxString id = wxString::Format(_("%s%d"),CfDrawingElement::ElementTypeArray[type_id],m_ElementCountArray[type_id]++);
	switch(type_id)
	{
	case ID_ELEMENT_PORT:
		element = new CfPort(id,source_element->GetName()+copy,*(CfPort*)source_element);
		break;
	case ID_ELEMENT_WIRE:
		element = new CfWire(id,source_element->GetName()+copy,*(CfWire*)source_element);
		break;
	case ID_ELEMENT_SYMBOL:
		element = new CfSymbol(id,source_element->GetName()+copy,*(CfSymbol*)source_element);
		break;
	case ID_ELEMENT_IMAGE:
		//element = new CfImage(source_element);
		break;
	case ID_ELEMENT_LAYER:
		element = new CfLayer(id,source_element->GetName()+copy,*(CfLayer*)source_element);
		break;
	
	}
	return element;

}

CfDrawingElement* CfDrawingElementFactory::DuplicateDrawingElement(CfDrawingElement *source_element)
{
	CfDrawingElement *element = NULL;
	int type_id = source_element->GetTypeId();
	wxString id = source_element->GetId();
	switch(type_id)
	{
	case ID_ELEMENT_PORT:
		element = new CfPort(id,source_element->GetName(),*(CfPort*)source_element);
		break;
	case ID_ELEMENT_WIRE:
		element = new CfWire(id,source_element->GetName(),*(CfWire*)source_element);
		break;
	case ID_ELEMENT_SYMBOL:
		element = new CfSymbol(id,source_element->GetName(),*(CfSymbol*)source_element);
		break;
	case ID_ELEMENT_IMAGE:
		element = new CfImage((CfImage*)source_element);
		break;
	case ID_ELEMENT_LAYER:
		element = new CfLayer(id,source_element->GetName(),*(CfLayer*)source_element);
		break;
	}
	return element;

}

CfDrawingElement* CfDrawingElementFactory::CreateDrawingElement(wxEvtHandler *p_event_handler,int type_id,  void *data)
{
	CfDrawingElement *element = NULL;
	wxString id = wxString::Format(_("%s%d"),CfDrawingElement::ElementTypeArray[type_id],m_ElementCountArray[type_id]++);
	switch(type_id)
	{
	case ID_ELEMENT_INSTANCE:
		element = new CfInstance(p_event_handler,id,(CfSymbol*) data);
		break;
	case ID_ELEMENT_PORT:
		break;
	case ID_ELEMENT_WIRE:
		break;
	case ID_ELEMENT_SYMBOL:
		break;
	case ID_ELEMENT_IMAGE:
		break;
	case ID_ELEMENT_LAYER:
		break;
	}
	return element;

}
CfDrawingElement* CfDrawingElementFactory::CreateDrawingElement(wxEvtHandler *p_event_handler,int type_id, CfShape *shape, const wxString &path)
{
	CfDrawingElement *element = NULL;
	wxString id = wxString::Format(_("%s%d"),CfDrawingElement::ElementTypeArray[type_id],m_ElementCountArray[type_id]++);
	switch(type_id)
	{
	case ID_ELEMENT_PORT:
		element = new CfPort(p_event_handler,id,id,(CfPortShape*)shape);
		break;
	case ID_ELEMENT_WIRE:
		element = new CfWire(p_event_handler,id,id,(CfLineSegment*)shape);
		break;
	case ID_ELEMENT_SYMBOL:
		element = new CfSymbol(p_event_handler,id,id,(CfRectangle*)shape);
		break;
	case ID_ELEMENT_IMAGE:
	{
		wxFileName fn(path);
		wxString name = fn.GetFullName();
		element = new CfImage(p_event_handler,id,name,path);
		break;
	}
	case ID_ELEMENT_LAYER:
		element = new CfLayer(p_event_handler,id,id);
		break;
	}
	return element;

}
void CfDrawingElementFactory::SetElementIdCount(int type_id, int count)
{
	//the count only can be increased 
	if(m_ElementCountArray[type_id] < count)
		m_ElementCountArray[type_id] = count;
}