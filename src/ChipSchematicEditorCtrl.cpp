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

//CfDrawingElementGroup CfChipSchematicEditorCtrl::ms_Clipboard;

BEGIN_EVENT_TABLE (CfChipSchematicEditorCtrl, wxScrolledWindow)
	EVT_CF_NOTIFICATION(wxID_ANY, CfChipSchematicEditorCtrl::OnDrawingElementEvent)
	EVT_PAINT(CfChipSchematicEditorCtrl::OnPaint)
	EVT_MOUSE_EVENTS(CfChipSchematicEditorCtrl::OnMouseEvent)
	EVT_SIZE(CfChipSchematicEditorCtrl::OnSize)
	EVT_CHAR(CfChipSchematicEditorCtrl::OnPressKey)
	EVT_SCROLLWIN(CfChipSchematicEditorCtrl::OnScrollWin)
	//draw
	EVT_MENU (MENU_ID_ADDIMAGE,         CfChipSchematicEditorCtrl::OnAddImage)
	EVT_MENU (MENU_ID_ADDSYMBOL,        CfChipSchematicEditorCtrl::OnAddSymbol)
	EVT_MENU (MENU_ID_ADDLAYER,         CfChipSchematicEditorCtrl::OnAddLayer)
	EVT_MENU (MENU_ID_PORT,             CfChipSchematicEditorCtrl::OnDrawPort)
	EVT_MENU (MENU_ID_WIRE,             CfChipSchematicEditorCtrl::OnDrawWire)
	EVT_MENU (MENU_ID_SYMBOL,           CfChipSchematicEditorCtrl::OnDrawSymbol)
	EVT_MENU (MENU_ID_SAVESYMBOL,       CfChipSchematicEditorCtrl::OnSaveSymbol)
	EVT_MENU (MENU_ID_ZOOMIN,			CfChipSchematicEditorCtrl::OnZoom)
	EVT_MENU (MENU_ID_ZOOMOUT,			CfChipSchematicEditorCtrl::OnZoom)
	EVT_MENU (MENU_ID_GRID,				CfChipSchematicEditorCtrl::OnShowHideGrid)

	// edit
	EVT_MENU (wxID_DELETE,             CfChipSchematicEditorCtrl::OnEditDelete)
    EVT_MENU (wxID_CLEAR,              CfChipSchematicEditorCtrl::OnEditClear)
    EVT_MENU (wxID_CUT,                CfChipSchematicEditorCtrl::OnEditCut)
    EVT_MENU (wxID_COPY,               CfChipSchematicEditorCtrl::OnEditCopy)
    EVT_MENU (wxID_PASTE,              CfChipSchematicEditorCtrl::OnEditPaste)
    EVT_MENU (wxID_SELECTALL,          CfChipSchematicEditorCtrl::OnEditSelectAll)
    EVT_MENU (wxID_REDO,               CfChipSchematicEditorCtrl::OnEditRedo)
    EVT_MENU (wxID_UNDO,               CfChipSchematicEditorCtrl::OnEditUndo)

	EVT_CONTEXT_MENU(CfChipSchematicEditorCtrl::OnContextMenu)
	EVT_UPDATE_UI(wxID_REDO,	CfChipSchematicEditorCtrl::OnRedoUpdateUI)
	EVT_UPDATE_UI(wxID_UNDO,	CfChipSchematicEditorCtrl::OnUndoUpdateUI)
END_EVENT_TABLE()

//int CfChipSchematicEditorCtrl::m_DrawingElementID = DRAWING_ELEMENT_NONE;
//CfDrawingElement* CfChipSchematicEditorCtrl::m_pDrawingElement = NULL;

CfChipSchematicEditorCtrl::CfChipSchematicEditorCtrl(const wxString &file_path,
			wxWindow *parent, CfProject* project,wxWindowID id,
            const wxPoint &pos,
            const wxSize &size
)
: CfChipEditorBaseCtrl(file_path,parent,project,id,pos,size)
,m_Panel(this)
{
	m_pElementWithChildren = &m_Panel;
}


CfChipSchematicEditorCtrl::~CfChipSchematicEditorCtrl(void)
{
	ClearHighlightElements();
	CloseEditor();
}

//When the file notebook is clicked and showed
void CfChipSchematicEditorCtrl::Activated()
{
	//update property view
	CfDrawingElement *element = NULL;
	if(HighlightElementList.Count() >0) element = HighlightElementList[0];
	//if there is no element highlighted, highlight layer
	else element = m_Panel.GetActiveLayer();
	//update layer view and property view
	SendNotification(cfId_Schematic_LayerChange, &m_Panel);
	SendNotification(cfId_Schematic_ElementChange, element);
	//update status bar
	CfNotification *notif = new CfNotification(cfId_StatusBar_Update);
	notif->SetString(wxString::Format("%g%%",m_ScaleFactor*100));
	AddNotification(notif);	

}
void CfChipSchematicEditorCtrl::AddElement(CfDrawingElement *element)
{
	m_Panel.AddElement(element);
	HighlightElement(element);
	//if(m_pActiveLayer == NULL)
	//{
	//	m_pActiveLayer = (CfLayer* )DoCreateDrawingElement(this,ID_ELEMENT_LAYER,NULL,wxEmptyString);
	//	m_Panel.AddLayer(m_pActiveLayer);
	//}
	//m_pActiveLayer->AddElement(element);
}
void CfChipSchematicEditorCtrl::AddActionToHistory(IAction *action, bool run_now, bool refresh)
{
	m_History.AddAction(action);
	if(run_now) action->Run();
	if(refresh) this->Refresh();
	//update layer view
	SendNotification(cfId_Schematic_LayerChange, &m_Panel);
}
//Addjust the mouse postion to the absolute one, and it can perform the auto alignment
wxPoint CfChipSchematicEditorCtrl::AddjustMousePos(wxPoint pos, bool alignment)
{
	wxPoint offset = GetScrollOffset();
	pos.x += offset.x;
	pos.y += offset.y;
	 
	if(alignment)
	{

		int grid_size = CfEnvironmentUtil::Get()->GetInt(PROP_SETTING_GRID_SIZE,1)*10;
		int precision = grid_size * m_ScaleFactor;
		if(precision == 0) precision =1;
		pos.x = pos.x % precision> precision / 2.0 ? (pos.x / precision + 1) * precision : pos.x / precision  * precision;
		pos.y = pos.y % precision> precision / 2.0 ? (pos.y / precision + 1) * precision : pos.y / precision  * precision;
	}
	return pos;
}

void CfChipSchematicEditorCtrl::OnDrawingElementEvent(wxEvent& event)
{
	CfNotification* pNotification= (CfNotification*)event.GetEventObject();
	wxASSERT(pNotification);
	switch (pNotification->GetID())
	{
	case cfId_DrawingElement_Change:
		{
			CfNotification *pnote= new CfNotification(cfId_Editor_Modified);
			AddNotification(pnote);
			break;
		}
	}
	delete pNotification;
}

void CfChipSchematicEditorCtrl::OnContextMenu(wxContextMenuEvent &event)
{
	wxPoint pt = ScreenToClient(event.GetPosition());
	pt = AddjustMousePos(pt);
    wxLogDebug(wxT("OnContextMenu at screen coords (%i, %i)"), pt.x, pt.y);

	//check if clicked the highlighted elements
	CfDrawingElement * element = HighlightElementList.GetElement(pt);
	int element_count = 0;
	if(element)
	{
		//if the element is one of the highlighted elements, just show common menu
		element_count = HighlightElementList.Count();
	}
	else
	{
		//the element is not highlighted
		element = m_Panel.GetElement(pt,m_ScaleFactor);
		if(element)
		{
			element_count = 1;
			HighlightElement(element);
		}
	}

	if(element_count == 0) return;

	//add common menu items
	wxMenu menu;
	wxMenuItem *item;
	item = menu.Append(wxID_UNDO,_("&Undo\tCtrl+Z"));
	item = menu.Append(wxID_REDO,_("&Redo\tCtrl+Y"));
    menu.AppendSeparator();
	/*
	menu.Append(wxID_FIND,_("&Find and Replace\tCtrl+F"));
	menu.Append(MENU_ID_FINDNEXT,_("Find &Next\tF3"));
	menu.Append(MENU_ID_FindInFiles,_("Find &in Files\tCtrl+Shift+F"));

	*/
    menu.AppendSeparator();
	menu.Append(wxID_CUT,_("&Cut\tCtrl+X"));
	menu.Append(wxID_COPY,_("C&opy\tCtrl+C"));
	menu.Append(wxID_PASTE,_("&Paste\tCtrl+V"));
    menu.AppendSeparator();
	menu.Append(wxID_SELECTALL,_("&Select All\tCtrl+A"));

	if(element_count==1)
	{
		switch (element->GetTypeId())
		{
		case ID_ELEMENT_SYMBOL:
			{
				menu.Append(MENU_ID_SAVESYMBOL,_("&Save symbol..."));
			}

		}
	}
    menu.AppendSeparator();
	menu.Append(wxID_DELETE,_("&Delete\tDel"));
    PopupMenu(&menu, pt);

}
// draw

void CfChipSchematicEditorCtrl::OnDrawPort(wxCommandEvent &event)
{
		ChosePort();
}
void CfChipSchematicEditorCtrl::OnDrawWire(wxCommandEvent &event)
{
		ChoseWire();
}
void CfChipSchematicEditorCtrl::OnAddImage(wxCommandEvent &event)
{
		AddImage();
}
void CfChipSchematicEditorCtrl::OnAddLayer(wxCommandEvent &event)
{
		NewLayer();
}
//void CfChipSchematicEditorCtrl::OnAddElement(wxCommandEvent &event)
//{
//		AddElement();
//}
void CfChipSchematicEditorCtrl::OnAddSymbol(wxCommandEvent &event)
{
		AddSymbol();
}
void CfChipSchematicEditorCtrl::OnDrawSymbol(wxCommandEvent &event)
{
		ChoseSymbol();
}
void CfChipSchematicEditorCtrl::OnSaveSymbol(wxCommandEvent &event)
{
		SaveSymbol();
}
//
////edit
//void CfChipSchematicEditorCtrl::OnEditRedo (wxCommandEvent &event)
//{
//	IAction *action = m_History.Redo();
//	//Keep highlight the select elements
//	if(action) HighlightElementList.Add(action->GetElementGroup());
//	this->Refresh();
//	//update layer view
//	SendNotification(cfId_Schematic_LayerChange, &m_Panel);
//}
//void CfChipSchematicEditorCtrl::OnEditUndo (wxCommandEvent &event)
//{
//	IAction *action = m_History.Undo();
//	//Keep highlight the select elements
//	if(action) HighlightElementList.Add(action->GetElementGroup());
//	this->Refresh();
//	//update layer view
//	SendNotification(cfId_Schematic_LayerChange, &m_Panel);
//}
//void CfChipSchematicEditorCtrl::OnEditClear (wxCommandEvent &event)
//{
//}
//void CfChipSchematicEditorCtrl::OnEditCut (wxCommandEvent &event)
//{
//	OnEditCopy(event);
//	OnEditDelete(event);
//}
//void CfChipSchematicEditorCtrl::OnEditCopy (wxCommandEvent &event)
//{
//	ms_Clipboard.Clear();
//	ms_Clipboard.Add(&HighlightElementList);
//}
//void CfChipSchematicEditorCtrl::OnEditPaste (wxCommandEvent &event)
//{
//	CfDrawingElement* element = NULL;
//	ClearHighlightElements();
//	for(size_t i=0;i<ms_Clipboard.Count();i++)
//	{
//		element = DoCopyDrawingElement(ms_Clipboard[i]);	//Use DoCopyDrawingElement not CopyDrawingElement!!!
//
//		element->Move(10,10);
//		HighlightElementList.Add(element);
//		AddElement(element);
//	}
//	IAction* create = new CfCreate(&HighlightElementList);
//	AddActionToHistory(create,false);
//}
//void CfChipSchematicEditorCtrl::OnEditSelectAll (wxCommandEvent &event)
//{
//}
//void CfChipSchematicEditorCtrl::OnEditDelete(wxCommandEvent &event)
//{
//	if(HighlightElementList.Count()==0) return;
//	IAction *del = new CfDelete(&HighlightElementList);
//	AddActionToHistory(del,true);
//	ClearHighlightElements();
//}

bool CfChipSchematicEditorCtrl::LoadFile()
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	wxFileInputStream fis(m_FilePath);
	m_Panel.Load(this, &fis);

	UpdateScrollBar();
	return true;
}
 

void CfChipSchematicEditorCtrl::RefreshEditor()
{
	UpdateScrollBar();
	this->Refresh();
}

bool CfChipSchematicEditorCtrl::SaveFile()
{
	wxFileOutputStream fos(m_FilePath);
	m_Panel.Dump(&fos);
	fos.Close();
	return true;
}

void CfChipSchematicEditorCtrl::SaveSymbol()
{
	wxASSERT(HighlightElementList.Count()==1);
	CfSymbol *symbol = (CfSymbol*)HighlightElementList[0];

	CfSaveSymbolFileDialogCtrl dlg(this,symbol,m_Panel.GetActiveLayer()->GetElementArray(),m_pProject);
	if (dlg.ShowModal() == wxID_OK)
	{
		//add symbol file to project
		CfNotification *pNotification = new CfNotification(cfId_AddFile);
		pNotification->SetString(dlg.GetSymbolPath());
		pNotification->SetData(m_pProject->GetTreeItemId());
		AddNotification(pNotification);
	}
}

bool CfChipSchematicEditorCtrl::Modified()
{
	return m_Panel.IsChanged();
}

void CfChipSchematicEditorCtrl::ProcessEvent(wxCommandEvent &event)
{
	//int id = event.GetId();
	//switch(id)
	//{
	//case MENU_ID_ADDIMAGE:
	//	ChoseAddImage();
	//	break;
	//case MENU_ID_PORT:
	//	ChosePort();
	//	break;
	//case MENU_ID_WIRE:
	//	ChoseWire();
	//	break;
	//}
}

void CfChipSchematicEditorCtrl::HighlightElement(CfDrawingElement* element)
{	
	//Show it in property window and highlight
	if(element)
	{
		if(element->IsHighlighted()) return;
		m_pDrawingElement = element;
		//update layer view and property view
		SendNotification(cfId_Schematic_LayerChange, &m_Panel);
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
cfReaderID CfChipSchematicEditorCtrl::GetReaderId() const
{
	return FR_CHIP_SCHEMATIC_EDITOR;
}

wxPoint CfChipSchematicEditorCtrl::GetScrollOffset()
{
	int shift_x = this->GetScrollPos(wxHORIZONTAL) * SCROLL_RATE;
	int shift_y = this->GetScrollPos(wxVERTICAL) * SCROLL_RATE;
	return wxPoint(shift_x,shift_y);
}

void CfChipSchematicEditorCtrl::OnMouseEvent(wxMouseEvent& event)
{
	wxPoint mouse_pos = event.GetPosition();
	mouse_pos = AddjustMousePos(mouse_pos);
    if (event.LeftDown())
    {
		wxLogDebug(_("Left Down: %d,%d"),mouse_pos.x,mouse_pos.y);		
		MouseLDown_DrawingElement(mouse_pos);
		if(!m_bSelected)	HighlightElement(m_Panel.GetElement(mouse_pos,m_ScaleFactor));
		//CaptureMouse();
		return;		
    }
	else if (event.Dragging())
	{
		if(event.LeftIsDown())
		{
			MouseDragging_DrawingElement(mouse_pos);
			return;
		}
	}
	else if (event.LeftUp())
	{
		MouseLUp_DrawingElement(mouse_pos);
		wxLogDebug(_("Left Down: %d,%d"),mouse_pos.x,mouse_pos.y);
		SetFocus();
	}
	else if (event.RightDown())
	{
		wxLogDebug(_("Right Down"));
		//m_bSelected = false;
		//HighlightElement(NULL);
    }
	event.Skip();
}

void CfChipSchematicEditorCtrl::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
	//wxPaintDC dc( this );

	//Use double-buffered drawing
	wxBufferedPaintDC dc(this);
    PrepareDC( dc );
	dc.Clear();
	//dc.SetUserScale(m_ScaleFactor,m_ScaleFactor);
	wxSize dc_size = dc.GetSize();
	wxSize client_size = GetClientSize();

	//calculate the offset of scrolled
	int shift_x = this->GetScrollPos(wxHORIZONTAL) * SCROLL_RATE;
	int shift_y = this->GetScrollPos(wxVERTICAL) * SCROLL_RATE;
	wxLogDebug(_("Size :%d,%d; shift :%d,%d"),dc_size.x,dc_size.y,shift_x,shift_y);
	wxLogDebug(_("Client Size :%d,%d; %d,%d"),client_size.x,client_size.y,client_size.GetWidth(),client_size.GetHeight());
	m_Panel.Draw(dc,CfScreenView(shift_x,shift_y,client_size.x,client_size.y,m_ScaleFactor));

	//this->SetFocus();
	//if(m_pActiveLayer)
	//	Draw(dc,CfScreenView(shift_x,shift_y,dc_size.x,dc_size.y,1),m_pActiveLayer->GetElementArray(),&m_HightlightElements);

}

void CfChipSchematicEditorCtrl::OnPressKey(wxKeyEvent& event)
{
	if((event.GetKeyCode() == WXK_DELETE) && m_Panel.DelElement(m_pDrawingElement))
		this->Refresh();
}
//void CfChipSchematicEditorCtrl::DrawElement(IDrawable *element)
//{
//
//}

void CfChipSchematicEditorCtrl::OnRedoUpdateUI(wxUpdateUIEvent &event)
{
	event.Enable(m_History.CanRedo());
}

void CfChipSchematicEditorCtrl::OnUndoUpdateUI(wxUpdateUIEvent &event)
{
	event.Enable(m_History.CanUndo());
}

void CfChipSchematicEditorCtrl::OnSize(wxSizeEvent& event)
{
	this->Refresh();
}

void CfChipSchematicEditorCtrl::OnShowHideGrid(wxCommandEvent& event)
{
	m_Panel.ToggleGrid();
	this->Refresh();
}

void CfChipSchematicEditorCtrl::OnZoom(wxCommandEvent& event)
{
	int id = event.GetId();
	float rate = 2;

	if(id == MENU_ID_ZOOMOUT)
	{
		if(m_ScaleFactor == 0.25) return;
		rate = 0.5;
	}
	m_ScaleFactor *= rate;
	UpdateScrollBar();

	wxSize client_size = GetClientSize();
	//calculate the offset of scrolled
	int shift_x = this->GetScrollPos(wxHORIZONTAL) * SCROLL_RATE;
	int shift_y = this->GetScrollPos(wxVERTICAL) * SCROLL_RATE;
	int x = (shift_x + client_size.x/2)*rate- client_size.x/2;
	int y = (shift_y + client_size.y/2)*rate- client_size.y/2;
	this->Scroll(x/SCROLL_RATE,y/SCROLL_RATE);

	CfNotification *notif = new CfNotification(cfId_StatusBar_Update);
	notif->SetString(wxString::Format("%g%%",m_ScaleFactor*100));
	AddNotification(notif);	
	this->Refresh();
}

//void CfChipSchematicEditorCtrl::AddElement()
//{
//	wxFileDialog dialog
//	 (
//		NULL,
//		_T("Choose Element File"),
//		m_pProject->GetDirectory(),
//		wxEmptyString,
//		_T("Symbol and image files (*.cpsy;*.jpg)|*.cpsy;*.jpg|Symbol files (*.cpsy)|*.cpsy|Image files (*.jpg)|*.jpg")
//	 );
//	if (dialog.ShowModal() == wxID_OK)
//	{
//		wxString fn = dialog.GetFilename();
//		CfDrawingElement* element;
//		if(fn.EndsWith(_(".jpg")))
//			element = CreateDrawingElement(ID_ELEMENT_IMAGE,NULL,dialog.GetPath());
//		else if(fn.EndsWith(_(".cpsy")))
//		{
//			wxFileInputStream fis(dialog.GetPath());
//			CfSymbol temp(this);
//			temp.Load(&fis,GetScrollOffset());
//			element = CopyDrawingElement(&temp);
//			HighlightElement(element);
//		}
//
//		AddElement(element);
//		this->Refresh();
//	}
//}

void CfChipSchematicEditorCtrl::NewLayer()
{
	CfDrawingElement *bgd = new CfSolidColorBackground(this->m_pEventHandler,*wxWHITE);
	CfLayer *layer = (CfLayer* )CreateDrawingElement(ID_ELEMENT_LAYER,NULL,wxEmptyString);
	layer->SetBackground(bgd);
	m_Panel.AddLayer(layer);

	//update layer view and property view
	SendNotification(cfId_Schematic_LayerChange, &m_Panel);
	SendNotification(cfId_Schematic_ElementChange, layer);
}
void CfChipSchematicEditorCtrl::AddImage()
{
	wxFileDialog dialog
	 (
		NULL,
		_T("Choose Image File"),
		m_pProject->GetDirectory(),
		wxEmptyString,
		_T("Jpeg files(*.jpg)|*.jpg")
	 );
	if (dialog.ShowModal() == wxID_OK)
	{
		m_pDrawingElement = CreateDrawingElement(ID_ELEMENT_IMAGE,NULL,dialog.GetPath());
	//	CfImage *image = new CfImage(this,dialog.GetPath());
		AddElement(m_pDrawingElement);
		this->Refresh();
	}
}

void CfChipSchematicEditorCtrl::AddSymbol()
{
	wxFileDialog dialog
	 (
		NULL,
		_T("Choose Symbol File"),
		m_pProject->GetDirectory(),
		wxEmptyString,
		_T("Symbol files(*.cpsy)|*.cpsy")
	 );
	if (dialog.ShowModal() == wxID_OK)
	{
		wxFileInputStream fis(dialog.GetPath());
		CfSymbol temp(this);
		temp.Load(&fis,GetScrollOffset());
		CfDrawingElement *instance = DoCreateDrawingElement2(this,ID_ELEMENT_INSTANCE,&temp);
	
		//add to history
		IAction* create = new CfCreate(instance);
		AddActionToHistory(create,false,false);

	//	CfDrawingElement* symbol = CopyDrawingElement(&temp);
		AddElement(instance);
		HighlightElement(instance);
		this->Refresh();
	}
}
void CfChipSchematicEditorCtrl::ChosePort()
{
	m_DrawingElementID = DRAWING_ELEMENT_PORT;
}

void CfChipSchematicEditorCtrl::ChoseWire()
{
	m_DrawingElementID = DRAWING_ELEMENT_WIRE;
}
void CfChipSchematicEditorCtrl::ChoseSymbol()
{
	m_DrawingElementID = DRAWING_ELEMENT_SYMBOL;
}
  
bool CfChipSchematicEditorCtrl::CloseEditor()
{
	if (m_Panel.IsChanged())
	{
		wxMessageDialog dlg(NULL,wxT("Save the changes to chip schematic '")+m_FilePath.AfterLast('\\')+_("'?"),PROJECT_CAPTION,wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
		int id = dlg.ShowModal();
		if(id == wxID_YES)
			SaveFile();

	}
	return true;
}
CfDrawingElement* CfChipSchematicEditorCtrl::CopyDrawingElement(CfDrawingElement *source_element)
{
	CfDrawingElement* element = DoCopyDrawingElement(source_element);
	IAction* create = new CfCreate(element);
	AddActionToHistory(create,false,false);
	//m_History.AddActionToHistory(create);
	return element;
}

CfDrawingElement* CfChipSchematicEditorCtrl::CreateDrawingElement(int type_id, CfShape *shape,const wxString& path)
{
	CfDrawingElement* element = DoCreateDrawingElement(this,type_id,shape,path);
	//CfDrawingElementGroup group;
	//group.Add(element);
	IAction* create = new CfCreate(element);
	AddActionToHistory(create,false,false);
	//m_History.AddActionToHistory(create);
	return element;
}

void CfChipSchematicEditorCtrl::Draw(wxDC& dc,const CfScreenView &view,CfDrawingElementGroup* element_group,CfDrawingElementGroup* highlight_array)
{
	if(element_group == NULL) return;
	bool highlight = false;
	CfDrawingElement *element;
	for(size_t i=0; i<element_group->Count();i++)
	{
		element = (*element_group)[i];
		for(size_t j=0;j<highlight_array->Count();j++)
		{
			if(element->GetId() == (*highlight_array)[j]->GetId())
			{
				highlight = true;
				break;
			}
		}
		element->Draw(dc,view);
		highlight = false;
	}

}

void CfChipSchematicEditorCtrl::MouseLDown_DrawingElement(wxPoint pt)
{
	wxLogDebug(_("click: %d,%d"),pt.x,pt.y);
	switch(m_DrawingElementID)
	{
	case DRAWING_ELEMENT_PORT:
	case DRAWING_ELEMENT_WIRE:	
	case DRAWING_ELEMENT_SYMBOL:	
		{
			m_DrawStartPos = wxPoint(pt.x, pt.y);
			m_overlay.Reset();
			break;
		}
	default:	//mabye to drag or not
		{
			if(!m_Panel.GetActiveLayer()|| (m_Panel.GetActiveLayer()->GetElementArray()->Count()==0)) return;
			m_pDrawingElement = m_Panel.GetElement(pt,m_ScaleFactor);
			if(m_bSelected)
			{
				if(HighlightElementList.Count() == 1)
				{
					m_bSelected = false;
				}
				if(HighlightElementList.GetElement(pt) && HighlightElementList.GetElement(pt)->GetTypeId() != ID_ELEMENT_IMAGE)
				{
					m_DragStartPos = pt;
					m_DrawingElementID = DRAWING_ELEMENT_DRAG;
					break;
				}
				else 
				{
					m_bSelected = false;
					HighlightElement(NULL);
					break;
				}
			}
			if(m_pDrawingElement && (m_pDrawingElement->GetTypeId() != ID_ELEMENT_IMAGE))
			{
				if(m_pDrawingElement->IsEndPoint(wxPoint(pt.x / m_ScaleFactor, pt.y / m_ScaleFactor)))
				{
					m_DrawingElementID = DRAWING_ELEMENT_WIRE;
					MouseLDown_DrawingElement(pt);
				}
				else
				{
					m_DragStartPos = pt;
					m_DrawingElementID = DRAWING_ELEMENT_DRAG;
				}
			}
			else
			{
				m_SelectStartPos = pt;
				m_overlay.Reset();
				m_DrawingElementID = DRAWING_ELEMENT_SELECT;
				
			}
			break;
		}
	}
}

void CfChipSchematicEditorCtrl::MouseDragging_DrawingElement(wxPoint pt)
{
	switch(m_DrawingElementID)
	{
	case DRAWING_ELEMENT_PORT:
		{
			wxClientDC dc(this);
			this->PrepareDC(dc);
			wxDCOverlay overlaydc( m_overlay, &dc );
			overlaydc.Clear();

			wxPoint motion(pt);
			wxSize size(motion.x - m_DrawStartPos.x, motion.y - m_DrawStartPos.y);
		
			
			wxString line_color = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_LINECOLOR,_("Blue"));
			wxString filled_color = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_FILLEDCOLOR,_("White"));
			int line_width = CfEnvironmentUtil::Get()->GetInt(ENV_DRAWING_LINEWIDTH,3);
			dc.SetPen(wxPen(GetColorFromStr(line_color),line_width));
			dc.SetBrush(wxBrush(GetColorFromStr(filled_color),wxTRANSPARENT));
			dc.DrawRectangle(m_DrawStartPos,size);
			break;
		}
	case DRAWING_ELEMENT_WIRE:
		{
			wxClientDC dc(this);
			this->PrepareDC(dc);
			wxDCOverlay overlaydc( m_overlay, &dc );
			overlaydc.Clear();

			wxString line_color = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_LINECOLOR,_("Blue"));
			int line_width = CfEnvironmentUtil::Get()->GetInt(ENV_DRAWING_LINEWIDTH,2);
			dc.SetPen(wxPen(GetColorFromStr(line_color),line_width));
			
			dc.DrawLine(m_DrawStartPos, pt);
			break;
		}
	case DRAWING_ELEMENT_SYMBOL:
		{
			wxClientDC dc(this);
			this->PrepareDC(dc);
			wxDCOverlay overlaydc( m_overlay, &dc );
			overlaydc.Clear();

			wxPoint motion(pt);
			wxSize size(motion.x - m_DrawStartPos.x, motion.y - m_DrawStartPos.y);
			wxString line_color = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_LINECOLOR,_("Black"));
			wxString filled_color = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_FILLEDCOLOR,_("White"));
			int line_width = CfEnvironmentUtil::Get()->GetInt(ENV_DRAWING_LINEWIDTH,6);
			
			dc.SetPen(wxPen(GetColorFromStr(line_color),line_width));
			dc.SetBrush(wxBrush(GetColorFromStr(filled_color),wxTRANSPARENT));
			dc.DrawRectangle(m_DrawStartPos,size);
			break;
		}
	case DRAWING_ELEMENT_DRAG:
		{
			wxPoint motion(pt);
			int x_move = (motion.x - m_DragStartPos.x) / m_ScaleFactor;
			int y_move = (motion.y - m_DragStartPos.y) / m_ScaleFactor;
			if(((abs(x_move)>=20)||(abs(y_move)>=20)))
			{
				CfMove *move = new CfMove(&HighlightElementList, x_move, y_move);
				AddActionToHistory(move,true);
					//move->Run();
					//m_History.AddActionToHistory(move);
					//this->Refresh();
					//m_pDrawingElement->Move(x_move,y_move);
					m_DragStartPos = motion;
			}	
			break;
		}
	case DRAWING_ELEMENT_SELECT:
		{
			wxClientDC dc(this);
			this->PrepareDC(dc);
			wxDCOverlay overlaydc( m_overlay, &dc );
			overlaydc.Clear();

			wxPoint motion(pt);
			wxSize size(motion.x - m_SelectStartPos.x, motion.y - m_SelectStartPos.y);
		
			
			dc.SetPen(wxPen(*wxLIGHT_GREY,2));
			dc.SetBrush(wxBrush(*wxLIGHT_GREY,wxTRANSPARENT));
			dc.DrawRectangle(m_SelectStartPos,size);
			break;
		}
	}
}

void CfChipSchematicEditorCtrl::MouseLUp_DrawingElement(wxPoint pt)
{
	if(HasCapture())
			ReleaseMouse();
	switch(m_DrawingElementID)
	{
	case DRAWING_ELEMENT_PORT:
		{
			wxPoint motion(pt);
			wxSize size((motion.x - m_DrawStartPos.x) / m_ScaleFactor, (motion.y - m_DrawStartPos.y) / m_ScaleFactor);
			if(size.x < 15 || size.y < 15)
			{
				m_DrawingElementID = DRAWING_ELEMENT_NONE;
				this->Refresh();
				return;
			}
			if(size.x < 0)
			{
				m_DrawStartPos.x += size.x;
				size.x *= -1;
			}
			if(size.y < 0)
			{
				m_DrawStartPos.y += size.y;
				size.y *= -1;
			}
			//CfRectangle *rect = new CfRectangle(m_DrawStartPos.x, m_DrawStartPos.y, size.x, size.y);
			CfPortShape *port = new CfInputPort(m_DrawStartPos.x/ m_ScaleFactor, m_DrawStartPos.y/ m_ScaleFactor, size.x, size.y);
			//m_pDrawingElement = new CfPort(wxEmptyString, rect);
			m_pDrawingElement = CreateDrawingElement(ID_ELEMENT_PORT,port,wxEmptyString);
			m_pDrawingElement->SetShowing(CfEnvironmentUtil::Get()->GetBool(PROP_SETTING_PORT_SHOWN,true));
			AddElement(m_pDrawingElement);
			this->Refresh();
			break;
		}
	case DRAWING_ELEMENT_WIRE:
		{
			CfDrawingElement *element = m_Panel.GetElement(m_DrawStartPos,m_ScaleFactor);
			if(!element) //&& (element->GetTypeId()==ID_ELEMENT_WIRE))
			{
				element = m_Panel.GetElement(pt,m_ScaleFactor);
			}
			if(element && (element->GetTypeId()==ID_ELEMENT_WIRE))
			{
	
				CfWire* wire = (CfWire*)element;
				if(!wire->IsEndPoint(wxPoint(m_DrawStartPos.x / m_ScaleFactor, m_DrawStartPos.y / m_ScaleFactor)))
					wire->IsEndPoint(wxPoint(pt.x / m_ScaleFactor, pt.y / m_ScaleFactor));
				if((int) sqrt(double(pt.x/ m_ScaleFactor - m_DrawStartPos.x/ m_ScaleFactor)* double(pt.x/ m_ScaleFactor - m_DrawStartPos.x/ m_ScaleFactor) + double(pt.y/ m_ScaleFactor - m_DrawStartPos.y/ m_ScaleFactor)*double(pt.y/ m_ScaleFactor - m_DrawStartPos.y/ m_ScaleFactor)) < 40)
				{
					m_DrawingElementID = DRAWING_ELEMENT_NONE;
					this->Refresh();
					return;
				}
				CfLineSegment *line = NULL;
				line = new CfLineSegment(m_DrawStartPos.x/ m_ScaleFactor, m_DrawStartPos.y/ m_ScaleFactor, pt.x / m_ScaleFactor , pt.y / m_ScaleFactor);	
				wire->Merge(line);
				this->Refresh();
				break;
			}
			else
			{
				CfLineSegment *line = NULL;
				if((int) sqrt(double(pt.x/ m_ScaleFactor - m_DrawStartPos.x/ m_ScaleFactor)* double(pt.x/ m_ScaleFactor - m_DrawStartPos.x/ m_ScaleFactor) + double(pt.y/ m_ScaleFactor - m_DrawStartPos.y/ m_ScaleFactor)*double(pt.y/ m_ScaleFactor - m_DrawStartPos.y/ m_ScaleFactor)) < 40)
				{
					m_DrawingElementID = DRAWING_ELEMENT_NONE;
					this->Refresh();
					return;
				}
				line = new CfLineSegment(m_DrawStartPos.x/ m_ScaleFactor, m_DrawStartPos.y/ m_ScaleFactor, pt.x  / m_ScaleFactor, pt.y  / m_ScaleFactor);
				m_pDrawingElement = CreateDrawingElement(ID_ELEMENT_WIRE,line,wxEmptyString);
				m_pDrawingElement->SetShowing(CfEnvironmentUtil::Get()->GetBool(PROP_SETTING_WIRE_SHOWN,true));
				AddElement(m_pDrawingElement);
				//m_Panel.GetElementArray()->Add(m_pDrawingElement);
				this->Refresh();
				break;
			}
			break;
		}
	case DRAWING_ELEMENT_SYMBOL:
		{
			wxPoint motion(pt);
			wxSize size((motion.x - m_DrawStartPos.x) / m_ScaleFactor, (motion.y - m_DrawStartPos.y) / m_ScaleFactor);
			if(size.x < 0)
			{
				m_DrawStartPos.x += size.x;
				size.x *= -1;
			}
			if(size.y < 0)
			{
				m_DrawStartPos.y += size.y;
				size.y *= -1;
			}
			CfRectangle *rect = new CfRectangle(m_DrawStartPos.x/ m_ScaleFactor, m_DrawStartPos.y/ m_ScaleFactor, size.x, size.y);
			m_pDrawingElement = CreateDrawingElement(ID_ELEMENT_SYMBOL,rect,wxEmptyString);
			m_pDrawingElement->SetShowing(CfEnvironmentUtil::Get()->GetBool(PROP_SETTING_SYMBOL_SHOWN,true));
			AddElement(m_pDrawingElement);
			this->Refresh();
			break;
		}
	case DRAWING_ELEMENT_DRAG:
		{
			break;
		}
	case DRAWING_ELEMENT_SELECT:
		{
			wxRect selectRect;
			selectRect = wxRect(wxPoint(wxMin(m_SelectStartPos.x, pt.x),min(m_SelectStartPos.y, pt.y)),  
										  wxPoint(wxMax(m_SelectStartPos.x, pt.x),max(m_SelectStartPos.y, pt.y)));
			CfDrawingElementGroup *allElement = m_Panel.GetActiveLayer()->GetElementArray();
			if(allElement->Count() == 0) 
			{
				this->Refresh();
				break;
			}
			else
			{
				ClearHighlightElements();
				for(size_t i = 0; i < allElement->Count(); i++)
				{
					//wxPoint pt_start = (*allElement)[i]->GetStartPos();
					//wxPoint pt_end = (*allElement)[i]->GetEndPos();
					//if(selectRect.Contains(pt_start) &&selectRect.Contains(pt_end))
					if((*allElement)[i]->Within(selectRect))
						HighlightElementList.Add((*allElement)[i]);
				}
				if(HighlightElementList.Count() > 0) m_bSelected = true;
				else m_bSelected = false;
				this->Refresh();
				break;
			}
		}
	}

	//m_pDrawingElement =	NULL;
	m_DrawingElementID = DRAWING_ELEMENT_NONE;
}

void CfChipSchematicEditorCtrl::UpdateScrollBar()
{
	//Set the scroll bar
	wxSize size = GetVirtualSize();
	wxPoint point = m_Panel.GetEndPos();
	point.x *= m_ScaleFactor;
	point.y *= m_ScaleFactor;
	if((size.x == point.x) && (size.y == point.y)) return;
	SetVirtualSize(wxSize(point.x,point.y));
	SetScrollRate(SCROLL_RATE,SCROLL_RATE); // Will be changable later
	Refresh();

}
void CfChipSchematicEditorCtrl::OnScrollWin(wxScrollWinEvent& event)
{
	this->Refresh();
}