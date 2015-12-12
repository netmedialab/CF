#include "CfDefines.h"
#include "FileUtil.h"
#include "Panel.h"
#include "Port.h"
#include "Wire.h"
#include "Image.h"
#include "Symbol.h"
#include "DrawingElementFactory.h"


CfPanel::CfPanel(wxEvtHandler *p_event_handler)
	:m_bChanged(false)
,CfNotificationSender(p_event_handler)
,m_ElementArray(false)
,m_pActiveElement(NULL)
,m_Grid(true)
{
}

CfPanel::~CfPanel()
{
}

void CfPanel::SetChanged(bool change)
{
	m_bChanged = change;
	if(change)
	{
		CfNotification *pNotification = new CfNotification(cfId_DrawingElement_Change);
		AddNotification(pNotification);
	}
}

bool CfPanel::IsChanged()
{
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		m_bChanged |= m_ElementArray[i]->Modified();
	}
	return m_bChanged;
}

void CfPanel::AddLayer(CfLayer* layer)
{
	m_ElementArray.Add(layer);
	m_pActiveElement = layer;
	SetChanged();
}

void CfPanel::SetActiveLayer(CfLayer* layer)
{
	if(m_pActiveElement == layer) return;
	m_pActiveElement = layer;
	//hide all background images
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		((CfLayer*)m_ElementArray[i])->ShowBackground(false);
	}
	//only showing the background image of active layer
	m_pActiveElement->ShowBackground(true);

}

void CfPanel::AddElement(CfDrawingElement* element)
{
	if(!m_pActiveElement) 
	{
		wxMessageDialog dlg(NULL,wxT("Fail to find the valid layer, add a layer first"),PROJECT_CAPTION,wxOK| wxOK_DEFAULT | wxICON_WARNING);
		dlg.ShowModal();
		return;
		//CfLayer* layer = (CfLayer* )DoCreateDrawingElement(m_pEventHandler,ID_ELEMENT_LAYER,NULL,wxEmptyString);
		//m_ElementArray.Add(layer);
		//m_pActiveElement = layer;
	}

	if(element->GetTypeId()== ID_ELEMENT_LAYER)
		m_ElementArray.Add(element);
	else 
		m_pActiveElement->AddElement(element);

	//if(element->GetTypeId() == ID_ELEMENT_IMAGE)
	//{
	//	int i=0;
	//	for(;i<m_ElementArray.Count();i++)
	//	{
	//		if(m_ElementArray[i]->GetTypeId() != ID_ELEMENT_IMAGE)
	//		{
	//			break;
	//		}
	//	}
	//	m_ElementArray.Insert(element,i);
	//}
	//else
	//	m_ElementArray.Add(element);
	SetChanged();
}

CfLayer *CfPanel::GetActiveLayer() const
{
	return (CfLayer*)m_pActiveElement;
}

CfDrawingElement* CfPanel::GetElement(wxPoint &point,double scale)
{
	CfLayer *layer;
	CfDrawingElement *element = NULL;
	wxPoint scaled_point(point.x/scale,point.y/scale);
	
	for(int i=m_ElementArray.Count()-1; i>=0; i--)
	{
		layer =(CfLayer*) m_ElementArray[i];
		if(!layer->IsShowing()) continue;
		element = layer->GetElementArray()->GetElement(scaled_point);
		point =wxPoint(scaled_point.x*scale,scaled_point.y*scale);
		if(element) break;
	}

	//if no element is clicked, return the layer
	if(!element)
	for(int i=m_ElementArray.Count()-1; i>=0; i--)
	{
		layer =(CfLayer*) m_ElementArray[i];
		if(layer->GetBackground() && layer->GetBackground()->IsShowing())
		//if(layer->IsShowing()) 
		{
			element = layer;
			break;
		}
	}
	return element;
}
CfDrawingElement* CfPanel::FindElement(CfDrawingElement *element) const
{
	return m_ElementArray.Find(element);
}

CfDrawingElement* CfPanel::FindElement(wxString& element_id) const
{
	return m_ElementArray.Find(element_id);
}

CfDrawingElementGroup* CfPanel::GetElementArray() 
{
	return &m_ElementArray;
	//if(m_pActiveElement == NULL) return NULL;
	//return m_pActiveElement->GetElementArray();
}

//CfDrawingElementGroup* CfPanel::GetHighlightElements() 
//{
//	return &m_HightlightElements;
//}

bool CfPanel::DelElement(CfDrawingElement* element)
{
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		((CfDrawingElementWithChildren *)m_ElementArray[i])->GetElementArray()->Delete(element,false);
	}
	//if(m_pActiveElement == NULL) return false;
	//bool res = m_pActiveElement->GetElementArray().Delete(element,true);
	SetChanged(true);
	return true;
}

void CfPanel::Draw(wxDC& dc,const CfScreenView &view)
{
	//1. Draw background first
	//This is to make sure that the background is under all elements of different layers. 
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		((CfLayer*) m_ElementArray[i])->DrawBackground(dc,view);
	}

	//2. Draw grid
	m_Grid.Draw(dc,view);

	//3.Draw elements
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		((CfLayer*) m_ElementArray[i])->Draw(dc,view);
	}
}

void CfPanel::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	m_ElementArray.Dump(fos,offset);
	SetChanged(false);
}

void CfPanel::Load(wxEvtHandler *p_event_handler,wxFileInputStream *fis, const wxPoint offset)
{
	m_ElementArray.Load(m_pEventHandler,fis,offset);
	if(m_ElementArray.Count() >0) m_pActiveElement = (CfLayer* )m_ElementArray[m_ElementArray.Count()-1];
}

wxSize CfPanel::GetSize()
{
	if(m_pActiveElement == NULL) return wxSize(0,0);
	wxSize size;
	int max_x = 0,max_y = 0;
	//CfDrawingElementGroup *element_group = m_pActiveElement->GetElementArray();
	IElementGroup *element_group = m_pActiveElement->GetElementArray();
	CfDrawingElement *element;
	for(size_t i=0; i<element_group->Count();i++)
	{
		element = (*element_group)[i];
		if(element->GetTypeId() == ID_ELEMENT_IMAGE)
			return element->GetSize();
		else
		{
			size = element->GetSize();
			if(size.x>max_x) max_x = size.x;
			if(size.y>max_y) max_y = size.y;
		}
	}
	return wxSize(max_x,max_y);
}

CfDrawingElement* CfPanel::GetShowingBackground()
{
	CfLayer* layer = NULL;
	CfDrawingElement* background = NULL;
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		layer =(CfLayer*) m_ElementArray[i];
		background = layer->GetBackground();
		if(background && background->IsShowing())
			break;
	}
	return background;
}
wxPoint CfPanel::GetEndPos()
{
	int max_x = 0,max_y = 0;
	wxPoint point(0,0);
	CfLayer *layer;

	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		layer = (CfLayer*)m_ElementArray[i];
		point = layer->GetEndPos();
		if(point.x>max_x) max_x = point.x;
		if(point.y>max_y) max_y = point.y;
	}
	return wxPoint(max_x,max_y);
}
//wxPoint CfPanel::GetEndPos()
//{
//
//	int max_x = 0,max_y = 0;
//	wxPoint point(0,0);
//	if(m_pActiveElement == NULL) return point;
//	CfDrawingElementGroup *element_group = m_pActiveElement->GetElementArray2();
//	CfDrawingElement *element;
//
//	for(size_t i=0; i<element_group->Count();i++)
//	{
//		element = (*element_group)[i];
//		if(element->GetTypeId() == ID_ELEMENT_IMAGE)
//			return element->GetEndPos();
//		else
//		{
//			point = element->GetEndPos();
//			if(point.x>max_x) max_x = point.x;
//			if(point.y>max_y) max_y = point.y;
//		}
//	}
//	return wxPoint(max_x,max_y);
//}


void CfPanel::ToggleGrid()
{
	m_Grid.SetShowing(!m_Grid.IsShowing());
}