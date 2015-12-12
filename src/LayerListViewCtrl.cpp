#include "LayerListViewCtrl.h"
#include "ResourceUtil.h"
#include "DrawingElementFactory.h"
#include "LogMessageCtrl.h"

#define LAYER_SELECTED_IMG_IDX 5

wxBEGIN_EVENT_TABLE(CfLayerListViewCtrl, wxDataViewListCtrl)
	EVT_DATAVIEW_ITEM_ACTIVATED(wxID_ANY, CfLayerListViewCtrl::OnItemActivated)
	EVT_DATAVIEW_SELECTION_CHANGED(wxID_ANY, CfLayerListViewCtrl::OnItemSelectionChanged)
	EVT_DATAVIEW_ITEM_VALUE_CHANGED(wxID_ANY, CfLayerListViewCtrl::OnItemValueChanged)
wxEND_EVENT_TABLE()

CfLayerListViewCtrl::CfLayerListViewCtrl(wxWindow *parent)
:wxDataViewListCtrl(parent, ID_LayerView,wxPoint(0,0), wxSize(350,300))
,CfNotificationSender(parent)
,m_pElementWithChildren(NULL)
{
	AppendTextColumn("Layer",wxDATAVIEW_CELL_ACTIVATABLE ,150);
	AppendToggleColumn("Active",wxDATAVIEW_CELL_ACTIVATABLE ,60);
	AppendToggleColumn("Image",wxDATAVIEW_CELL_ACTIVATABLE ,60);
	//AppendToggleColumn("Objects",wxDATAVIEW_CELL_INERT,75);
	AppendToggleColumn("Objects");

}


CfLayerListViewCtrl::~CfLayerListViewCtrl(void)
{
}

void CfLayerListViewCtrl::Reload()
{
	LoadElement(m_pElementWithChildren);
}

void CfLayerListViewCtrl::LoadElement(IElementWithChildren *ewc)
{
	this->DeleteAllItems();
	m_pElementWithChildren = ewc;

	// if panel is NULL, just clear 
	if(m_pElementWithChildren == NULL) return;
	AddElementNode(m_pElementWithChildren->GetElementArray());
}

void CfLayerListViewCtrl::AddElementNode(IElementGroup *element_group)
{
	if(element_group == NULL) return;
	CfDrawingElement *element;
	CfLayer *layer;
    wxVector<wxVariant> data;
	for(int i=element_group->Count()-1;i>=0;i--)
	{
		element = (*element_group)[i];
		if(element ->GetTypeId() != ID_ELEMENT_LAYER) continue;
		layer = (CfLayer*)element;
		if((layer->IsDeleted())) continue;

		data.clear();
		data.push_back(layer->GetName());
		data.push_back(layer == ((CfPanel* )m_pElementWithChildren)->GetActiveLayer());
		data.push_back(layer->GetBackground()&&layer->GetBackground()->IsShowing());
		data.push_back(layer->IsElementShowing());

		AppendItem( data, (wxUIntPtr)layer);
	}

}

void CfLayerListViewCtrl::HighlightElement(CfDrawingElement *element)
{
	if(element == NULL) return;
	SetHighlightElement(element);
	
	//Refresh image editor
	CfNotification *pNotification1 = new CfNotification(cfId_ImageEditor_Refresh);
	AddNotification(pNotification1);

	//Update property view
	CfNotification *pNotification2 = new CfNotification(cfId_PropertyView_ShowElement);
	pNotification2->SetElement(element);
	AddNotification(pNotification2);
}

//void CfLayerListViewCtrl::OnItemChecked(wxDataViewEvent& event)
//{
//    wxTreeListItem item = event.GetItem();
//	CfDrawingElement *element_temp = (CfDrawingElement*)this->GetItemData(item);
//	CfDrawingElement *element = m_pElementWithChildren->GetElement(element_temp);
//
//	if(element == NULL) return;
//	element->SetShowing(this->GetCheckedState(item)== wxCHK_CHECKED);
//	//if(element->GetTypeId() == ID_ELEMENT_LAYER)
//		LoadLayers(m_pElementWithChildren);
//
//	//Refresh image editor
//	CfNotification *pNotification = new CfNotification(cfId_ImageEditor_Refresh);
//	AddNotification(pNotification);
//}

void CfLayerListViewCtrl::OnItemActivated(wxDataViewEvent& event)
{
 //   wxTreeListItem item = event.GetItem();
	//CfDrawingElement *element_temp = (CfDrawingElement*)this->GetItemData(item);
	//CfDrawingElement *element = m_pElementWithChildren->GetElement(element_temp);

	//HighlightElement(element);
	//if((element != NULL) && (element->GetTypeId() == ID_ELEMENT_LAYER))
	//{
	//	m_pElementWithChildren->ActivateLayer((CfLayer *)element);
	//	this->SetItemImage(m_ActiveLayerItem,ID_ELEMENT_LAYER);
	//	this->SetItemImage(item,LAYER_SELECTED_IMG_IDX);
	//	m_ActiveLayerItem = item;
	//}
	wxLogDebug(_("OnItemActivated"));
}

void CfLayerListViewCtrl::OnItemValueChanged(wxDataViewEvent& event)
{
	wxDataViewItem item = event.GetItem();
	CfLayer *layer = (CfLayer*)this->GetItemData(item);
	int row = this->GetSelectedRow();

	wxLogDebug(_("OnItemValueChanged row:%d"),row);
	wxLogDebug(layer->GetName());
	wxLogDebug(_("value 1 %d"),this->GetToggleValue(row,1));
	wxLogDebug(_("value 2 %d"),this->GetToggleValue(row,2));
	wxLogDebug(_("value 3 %d"),this->GetToggleValue(row,3));

	layer->ShowBackground(this->GetToggleValue(row,2));  // 2 must be placed before 1
	layer->ShowElements(this->GetToggleValue(row,3));
	if(this->GetToggleValue(row,1))	
		((CfPanel*)m_pElementWithChildren)->SetActiveLayer(layer);

	//LoadLayers(m_pElementWithChildren);

	//Refresh image editor
	CfNotification *pNotification1 = new CfNotification(cfId_ImageEditor_Refresh);
	AddNotification(pNotification1);

	//Update layer view
	CfNotification *pNotification2 = new CfNotification(cfId_LayerView_Refresh);
	AddNotification(pNotification2);
}

void CfLayerListViewCtrl::OnItemSelectionChanged(wxDataViewEvent& event)
{
	wxDataViewItem item = event.GetItem();
	CfLayer *layer = (CfLayer*)this->GetItemData(item);

	//Update property view
	CfNotification *pNotification2 = new CfNotification(cfId_PropertyView_ShowElement);
	pNotification2->SetElement(layer);
	AddNotification(pNotification2);

	wxLogDebug(_("OnItemSelectionChanged"));
}