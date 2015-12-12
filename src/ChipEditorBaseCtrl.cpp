#include "ChipSchematicEditorCtrl.h"
#include "EnvironmentUtil.h"
#include "DrawingElementFactory.h"
#include "LogMessageCtrl.h"
#include "IDrawable.h"
#include "Image.h"
#include "Port.h"
#include "Wire.h"
#include "Symbol.h"
#include "Instance.h"
#include "SaveSymbolFileDialogCtrl.h"
#include "SolidColorBackground.h"
#include <cmath>
#include <wx/dcbuffer.h>

CfDrawingElementGroup CfChipEditorBaseCtrl::ms_Clipboard;

CfChipEditorBaseCtrl::CfChipEditorBaseCtrl(const wxString &file_path,
			wxWindow *parent, CfProject* project,wxWindowID id,
            const wxPoint &pos,
            const wxSize &size
)
: wxScrolledWindow(parent, id, pos, size), CfFileEditorBase(file_path), m_View(0,0,1000,1000,1)
,CfNotificationSender(parent)
,m_pDrawingElement(NULL)
,m_pProject(project)
,m_bSelected(false)
,m_ScaleFactor(1)
,m_DrawingElementID(DRAWING_ELEMENT_NONE)
{


}


CfChipEditorBaseCtrl::~CfChipEditorBaseCtrl(void)
{
}

void CfChipEditorBaseCtrl::HighlightElement(CfDrawingElement* element)
{	
	//Show it in property window and highlight
	if(element)
	{
		if(element->IsHighlighted()) return;
		m_pDrawingElement = element;
		//update layer view and property view
		SendNotification(cfId_Schematic_LayerChange, dynamic_cast<IElement*>(m_pElementWithChildren));
		SendNotification(cfId_Schematic_ElementChange, element);
		SetHighlightElement(element);
		this->Refresh();
	}
	else if(HighlightElementList.Count()>0)
	{
		ClearHighlightElements();
		m_bSelected = false;
		this->Refresh();
	}
}
//edit
void CfChipEditorBaseCtrl::OnEditRedo (wxCommandEvent &event)
{
	IAction *action = m_History.Redo();
	//Keep highlight the select elements
	if(action) HighlightElementList.Add(action->GetElementGroup());
	this->Refresh();
	SendNotification(cfId_LayerView_Refresh, NULL);
}
void CfChipEditorBaseCtrl::OnEditUndo (wxCommandEvent &event)
{
	IAction *action = m_History.Undo();
	//Keep highlight the select elements
	if(action) HighlightElementList.Add(action->GetElementGroup());
	this->Refresh();
	SendNotification(cfId_LayerView_Refresh, NULL);
}
void CfChipEditorBaseCtrl::OnEditClear (wxCommandEvent &event)
{
}
void CfChipEditorBaseCtrl::OnEditCut (wxCommandEvent &event)
{
	OnEditCopy(event);
	OnEditDelete(event);
}
void CfChipEditorBaseCtrl::OnEditCopy (wxCommandEvent &event)
{
	ms_Clipboard.Clear();
	ms_Clipboard.Add(&HighlightElementList);
}
void CfChipEditorBaseCtrl::OnEditPaste (wxCommandEvent &event)
{
	CfDrawingElement* element = NULL;
	ClearHighlightElements();
	for(size_t i=0;i<ms_Clipboard.Count();i++)
	{
		element = DoCopyDrawingElement(ms_Clipboard[i]);	//Use DoCopyDrawingElement not CopyDrawingElement!!!

		element->Move(10,10);
		///HighlightElementList.Add(element);
		AddElement(element);
	}
	IAction* create = new CfCreate(&HighlightElementList);
	AddActionToHistory(create,false);
}
void CfChipEditorBaseCtrl::OnEditSelectAll (wxCommandEvent &event)
{
}
void CfChipEditorBaseCtrl::OnEditDelete(wxCommandEvent &event)
{
	if(HighlightElementList.Count()==0) return;
	IAction *del = new CfDelete(&HighlightElementList);
	AddActionToHistory(del,true);
	HighlightElementList.Clear();
}

void CfChipEditorBaseCtrl::SendNotification(CfNotificationId notify_id, IElement *element, void* data)
{
	CfNotification *pNotification = new CfNotification(notify_id);
	pNotification->SetElement(element);
	pNotification->SetData(data);
	AddNotification(pNotification);
}
void CfChipEditorBaseCtrl::AddActionToHistory(IAction *action, bool run_now, bool refresh)
{
	m_History.AddAction(action);
	if(run_now) action->Run();
	if(refresh) this->Refresh();
	SendNotification(cfId_LayerView_Refresh, NULL);
}