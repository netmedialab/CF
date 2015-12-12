#include "LayerViewCtrl.h"
#include "ResourceUtil.h"
#include "DrawingElementFactory.h"
#include "Panel.h"
#include "Symbol.h"

#define LAYER_SELECTED_IMG_IDX 6 

wxBEGIN_EVENT_TABLE(CfLayerViewCtrl, wxTreeListCtrl)
	EVT_TREELIST_ITEM_CHECKED(wxID_ANY, CfLayerViewCtrl::OnItemChecked)
	EVT_TREELIST_ITEM_ACTIVATED(wxID_ANY, CfLayerViewCtrl::OnItemActivated)
	EVT_TREELIST_SELECTION_CHANGED(wxID_ANY, CfLayerViewCtrl::OnItemSelectionChanged)
wxEND_EVENT_TABLE()

CfLayerViewCtrl::CfLayerViewCtrl(wxWindow *parent)
:wxTreeListCtrl(parent, ID_LayerView,wxPoint(0,0), wxSize(200,300),wxTL_DEFAULT_STYLE |wxTL_CHECKBOX| wxNO_BORDER | wxTR_MULTIPLE | wxTR_EDIT_LABELS)
,CfNotificationSender(parent)
,m_pElementWithChildren(NULL)
{
	wxLog::SetLogLevel(0);
	//wxLogNull logNo;
	wxImageList *icon_list = new wxImageList(16,16);
	//icon_list->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_MESSAGE_BOX, wxSize(16,16)));
	icon_list->Add(ResourceUtil::Get()->GetBitmap(IMG_IMAGE));
	icon_list->Add(ResourceUtil::Get()->GetBitmap(IMG_PORT));
	icon_list->Add(ResourceUtil::Get()->GetBitmap(IMG_WIRE));
	icon_list->Add(ResourceUtil::Get()->GetBitmap(IMG_SYMBOL));
	icon_list->Add(ResourceUtil::Get()->GetBitmap(IMG_LAYER));
	icon_list->Add(ResourceUtil::Get()->GetBitmap(IMG_INSTANCE));
	icon_list->Add(ResourceUtil::Get()->GetBitmap(IMG_LAYER_SELECTED));
	SetImageList(icon_list);


}


CfLayerViewCtrl::~CfLayerViewCtrl(void)
{
	delete this->GetImageList();
}


void CfLayerViewCtrl::Reload()
{
	LoadElement(m_pElementWithChildren);
}

void CfLayerViewCtrl::LoadElement(IElementWithChildren *ewc)
{
	this->DeleteAllItems();
	this->ClearColumns();

	m_pElementWithChildren = ewc;
	// if panel is NULL, just clear
	if(m_pElementWithChildren== NULL) return;

	//wxString filename = m_pPanel->GetFileName();

	//this->AppendColumn(filename.AfterLast('\\'),
	this->AppendColumn(_("Layer View"),
                       wxCOL_WIDTH_AUTOSIZE,
                       wxALIGN_LEFT,
                       wxCOL_RESIZABLE | wxCOL_SORTABLE);
	wxTreeListItem root = this->GetRootItem();
	
	AddElementNode(m_pElementWithChildren->GetElementArray(),root);
	this->Expand(root);
}

void CfLayerViewCtrl::AddElementNode(IElementGroup *element_group, wxTreeListItem parent)
{
	if(element_group == NULL) return;
	CfDrawingElement *element;
	wxTreeListItem item;
	for(int i=element_group->Count()-1;i>=0;i--)
	{
		element = (*element_group)[i];
		//if((element->IsDeleted()) || (element->GetTypeId()== ID_ELEMENT_IMAGE)) continue;
		if((element->IsDeleted())) continue;
		//element_temp = CfDrawingElementFactory::GetDrawingElementFactory()->DuplicateDrawingElement(element);

		//wxString *element_id = new wxString(element->GetId());
		wxString element_id = element->GetId();
		item = this->AppendItem(parent,element->GetName(),element->GetTypeId(),-1,new wxStringClientData(element_id));
		this->CheckItem(item,element->IsShowing()?wxCHK_CHECKED:wxCHK_UNCHECKED);
		if(element->GetTypeId() == ID_ELEMENT_LAYER)
		{
			CfLayer *layer = (CfLayer*)element;
			if(layer == ((CfPanel*)m_pElementWithChildren)->GetActiveLayer()) 
			{
				this->SetItemImage(item,LAYER_SELECTED_IMG_IDX);
				m_ActiveLayerItem = item;
	//			this->Select(item);
			}
			AddElementNode(layer->GetElementArray(),item);
			this->Expand(item);
		}
		else
		{
			if(element->IsHighlighted()) 
				this->Select(item);
		}

	}

}

void CfLayerViewCtrl::HighlightElement(CfDrawingElement *element)
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

void CfLayerViewCtrl::OnItemChecked(wxTreeListEvent& event)
{
    wxTreeListItem item = event.GetItem();
	wxString element_id = ((wxStringClientData*)this->GetItemData(item))->GetData();
	CfDrawingElement *element = m_pElementWithChildren->FindElement(element_id);
	if(element == NULL) return;
	element->SetShowing(this->GetCheckedState(item)== wxCHK_CHECKED);
	//if(element->GetTypeId() == ID_ELEMENT_LAYER)
		//LoadLayers(m_pPanel);
	CfNotification *pNotification2 = new CfNotification(cfId_LayerView_Refresh);
//	pNotification2->SetElement((IElement*)m_pElementWithChildren);
	AddNotification(pNotification2);
	//Refresh image editor
	CfNotification *pNotification = new CfNotification(cfId_ImageEditor_Refresh);
	AddNotification(pNotification);
}

void CfLayerViewCtrl::OnItemActivated(wxTreeListEvent& event)
{
    wxTreeListItem item = event.GetItem();
	wxString element_id = ((wxStringClientData*)this->GetItemData(item))->GetData();
	CfDrawingElement *element = m_pElementWithChildren->FindElement(element_id);

	HighlightElement(element);
	if((element != NULL) && (element->GetTypeId() == ID_ELEMENT_LAYER))
	{
		CfLayer *layer = (CfLayer*)element;
		((CfPanel*)m_pElementWithChildren)->SetActiveLayer(layer);
		//Update property view
		CfNotification *pNotification2 = new CfNotification(cfId_LayerView_Refresh);
		AddNotification(pNotification2);
	}
	wxLogDebug(_("OnItemActivated"));
}

void CfLayerViewCtrl::OnItemSelectionChanged(wxTreeListEvent& event)
{
    wxTreeListItem item = event.GetItem();
	if(!item.IsOk()) return;
	wxStringClientData* data = (wxStringClientData*)this->GetItemData(item);
	if(data == NULL) return;
	wxString element_id = ((wxStringClientData*)this->GetItemData(item))->GetData();
	CfDrawingElement *element = m_pElementWithChildren->FindElement(element_id);
	if(element == NULL) return;

	HighlightElement(element);
	wxLogDebug(_("OnItemSelectionChanged"));
}