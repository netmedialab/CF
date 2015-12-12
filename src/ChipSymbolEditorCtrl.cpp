#include "ChipSymbolEditorCtrl.h"
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

CfDrawingElementGroup CfChipSymbolEditorCtrl::ms_Clipboard;

BEGIN_EVENT_TABLE (CfChipSymbolEditorCtrl, wxScrolledWindow)
	EVT_CF_NOTIFICATION(wxID_ANY, CfChipSymbolEditorCtrl::OnDrawingElementEvent)
	EVT_PAINT(CfChipSymbolEditorCtrl::OnPaint)
	EVT_MOUSE_EVENTS(CfChipSymbolEditorCtrl::OnMouseEvent)
	EVT_SIZE(CfChipSymbolEditorCtrl::OnSize)
	EVT_CHAR(CfChipSymbolEditorCtrl::OnPressKey)
	//draw
	EVT_MENU (MENU_ID_ADDSYMBOL,        CfChipSymbolEditorCtrl::OnNewSymbol)
	EVT_MENU (MENU_ID_PORT,             CfChipSymbolEditorCtrl::OnDrawPort)
	EVT_MENU (MENU_ID_WIRE,             CfChipSymbolEditorCtrl::OnDrawWire)
	EVT_MENU (MENU_ID_SYMBOL,           CfChipSymbolEditorCtrl::OnDrawSymbol)
	// edit
	EVT_MENU (wxID_DELETE,             CfChipEditorBaseCtrl::OnEditDelete)
    EVT_MENU (wxID_CLEAR,              CfChipEditorBaseCtrl::OnEditClear)
    EVT_MENU (wxID_CUT,                CfChipEditorBaseCtrl::OnEditCut)
    EVT_MENU (wxID_COPY,               CfChipEditorBaseCtrl::OnEditCopy)
    EVT_MENU (wxID_PASTE,              CfChipEditorBaseCtrl::OnEditPaste)
    EVT_MENU (wxID_SELECTALL,          CfChipEditorBaseCtrl::OnEditSelectAll)
    EVT_MENU (wxID_REDO,               CfChipEditorBaseCtrl::OnEditRedo)
    EVT_MENU (wxID_UNDO,               CfChipEditorBaseCtrl::OnEditUndo)

	EVT_CONTEXT_MENU(CfChipSymbolEditorCtrl::OnContextMenu)
	EVT_UPDATE_UI(wxID_REDO,	CfChipSymbolEditorCtrl::OnRedoUpdateUI)
	EVT_UPDATE_UI(wxID_UNDO,	CfChipSymbolEditorCtrl::OnUndoUpdateUI)
END_EVENT_TABLE()

//int CfChipSymbolEditorCtrl::m_DrawingElementID = DRAWING_ELEMENT_NONE;
//CfDrawingElement* CfChipSymbolEditorCtrl::m_pDrawingElement = NULL;

CfChipSymbolEditorCtrl::CfChipSymbolEditorCtrl(const wxString &file_path,
			wxWindow *parent, CfProject* project,wxWindowID id,
            const wxPoint &pos,
            const wxSize &size
)
//: wxScrolledWindow(parent, id, pos, size), CfFileEditorBase(file_path)
//,CfNotificationSender(parent)
: CfChipEditorBaseCtrl(file_path,parent,project,id,pos,size)
//, m_View(0,0,1000,1000,1)
, m_pSymbol(NULL)
{

	m_pElementWithChildren = m_pSymbol;
}


CfChipSymbolEditorCtrl::~CfChipSymbolEditorCtrl(void)
{
	delete m_pSymbol;
}

//When the file notebook is clicked and showed
void CfChipSymbolEditorCtrl::Activated()
{
	//update property view
	CfDrawingElement *element = NULL;
	if(HighlightElementList.Count() >0) element = HighlightElementList[0];
	//if there is no element highlighted, highlight layer
	else element = m_pSymbol;
	CfNotification *pNotification = new CfNotification(cfId_PropertyView_ShowElement);
	pNotification->SetElement(element);
	AddNotification(pNotification);

	//update layer view
	UpdateLayerView();
}
void CfChipSymbolEditorCtrl::AddElement(CfDrawingElement *element)
{
	m_pSymbol->AddElement(element);
}
//void CfChipSymbolEditorCtrl::AddActionToHistory(IAction *action, bool run_now, bool refresh)
//{
//	m_History.AddAction(action);
//	if(run_now) action->Run();
//	if(refresh) this->Refresh();
//	UpdateLayerView();
//}
//Addjust the mouse postion to the absolute one, and it can perform the auto alignment
wxPoint CfChipSymbolEditorCtrl::AddjustMousePos(wxPoint pos, bool alignment)
{
	wxPoint offset = GetScrollOffset();
	pos.x += offset.x;
	pos.y += offset.y;
	if(alignment)
	{
		int precision = CfEnvironmentUtil::Get()->GetInt(PROP_SETTING_GRID_SIZE,1)*10;
		pos.x = pos.x % precision> precision / 2.0 ? (pos.x / precision + 1) * precision : pos.x / precision  * precision;
		pos.y = pos.y % precision> precision / 2.0 ? (pos.y / precision + 1) * precision : pos.y / precision  * precision;
	}
	return pos;
}

void CfChipSymbolEditorCtrl::OnDrawingElementEvent(wxEvent& event)
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

void CfChipSymbolEditorCtrl::OnContextMenu(wxContextMenuEvent &event)
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
		element = m_pSymbol->GetElement(pt);
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

void CfChipSymbolEditorCtrl::OnDrawPort(wxCommandEvent &event)
{
		ChosePort();
}
void CfChipSymbolEditorCtrl::OnDrawWire(wxCommandEvent &event)
{
		ChoseWire();
}

void CfChipSymbolEditorCtrl::OnNewSymbol(wxCommandEvent &event)
{
		NewSymbol();
}

void CfChipSymbolEditorCtrl::OnDrawSymbol(wxCommandEvent &event)
{
		ChoseSymbol();
}

////edit
//void CfChipSymbolEditorCtrl::OnEditRedo (wxCommandEvent &event)
//{
//	IAction *action = m_History.Redo();
//	//Keep highlight the select elements
//	if(action) HighlightElementList.Add(action->GetElementGroup());
//	this->Refresh();
//	UpdateLayerView();
//}
//void CfChipSymbolEditorCtrl::OnEditUndo (wxCommandEvent &event)
//{
//	IAction *action = m_History.Undo();
//	//Keep highlight the select elements
//	if(action) HighlightElementList.Add(action->GetElementGroup());
//	this->Refresh();
//	UpdateLayerView();
//}
//void CfChipSymbolEditorCtrl::OnEditClear (wxCommandEvent &event)
//{
//}
//void CfChipSymbolEditorCtrl::OnEditCut (wxCommandEvent &event)
//{
//	OnEditCopy(event);
//	OnEditDelete(event);
//}
//void CfChipSymbolEditorCtrl::OnEditCopy (wxCommandEvent &event)
//{
//	ms_Clipboard.Clear();
//	ms_Clipboard.Add(&HighlightElementList);
//}
//void CfChipSymbolEditorCtrl::OnEditPaste (wxCommandEvent &event)
//{
//	CfDrawingElement* element = NULL;
//	HighlightElementList.Clear();
//	for(size_t i=0;i<ms_Clipboard.Count();i++)
//	{
//		element = DoCopyDrawingElement(ms_Clipboard[i]);	//Use DoCopyDrawingElement not CopyDrawingElement!!!
//
//		element->Move(10,10);
//		HighlightElementList.Add(element);
//		AddElement(element);
//	}
//	IAction* create = new CfCreate(m_pHightlightElements);
//	AddActionToHistory(create,false);
//}
//void CfChipSymbolEditorCtrl::OnEditSelectAll (wxCommandEvent &event)
//{
//}
//void CfChipSymbolEditorCtrl::OnEditDelete(wxCommandEvent &event)
//{
//	if(HighlightElementList.Count()==0) return;
//	IAction *del = new CfDelete(m_pHightlightElements);
//	AddActionToHistory(del,true);
//	HighlightElementList.Clear();
//}

bool CfChipSymbolEditorCtrl::LoadFile()
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	m_pSymbol = new CfSymbol(this);
	m_pElementWithChildren = m_pSymbol;
	wxFileInputStream fis(m_FilePath);
	m_pSymbol->Load(&fis);

	UpdateScrollBar();
	UpdateLayerView();
	return true;
}
 

bool CfChipSymbolEditorCtrl::SaveFile()
{
	wxFileOutputStream fos(m_FilePath);
	m_pSymbol->Dump(&fos);
	fos.Close();
	return true;
}

bool CfChipSymbolEditorCtrl::Modified()
{
	return m_pSymbol->Modified();
}

void CfChipSymbolEditorCtrl::ProcessEvent(wxCommandEvent &event)
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

//void CfChipSymbolEditorCtrl::HighlightElement(CfDrawingElement* element)
//{	
//	//Show it in property window and highlight
//		if(element)
//		{
//			m_pDrawingElement = element;
//			CfNotification *pNotification = new CfNotification(cfId_PropertyView_ShowElement);
//			pNotification->SetElement(element);
//			AddNotification(pNotification);
//			//update layer view
//			UpdateLayerView();
//			//if(element->GetTypeId()!= ID_ELEMENT_IMAGE)
//			{
//				HighlightElementList.Clear();
//				HighlightElementList.Add(element);
//				this->Refresh();
//			}
//			//else if(HighlightElementList.Count()>0)
//			//{
//			//	HighlightElementList.Clear();
//			//	this->Refresh();
//			//}
//		}
//		else if(HighlightElementList.Count()>0)
//		{
//			HighlightElementList.Clear();
//			m_bSelected = false;
//			this->Refresh();
//		}
//}
cfReaderID CfChipSymbolEditorCtrl::GetReaderId() const
{
	return FR_CHIP_SYMBOL_EDITOR;
}

wxPoint CfChipSymbolEditorCtrl::GetScrollOffset()
{
	int shift_x = this->GetScrollPos(wxHORIZONTAL) * SCROLL_RATE;
	int shift_y = this->GetScrollPos(wxVERTICAL) * SCROLL_RATE;
	return wxPoint(shift_x,shift_y);
}

void CfChipSymbolEditorCtrl::OnMouseEvent(wxMouseEvent& event)
{
	wxPoint mouse_pos = event.GetPosition();
	mouse_pos = AddjustMousePos(mouse_pos);
    if (event.LeftDown())
    {
		wxLogDebug(_("Left Down: %d,%d"),mouse_pos.x,mouse_pos.y);		
		MouseLDown_DrawingElement(mouse_pos);
		if(!m_bSelected)	HighlightElement(m_pSymbol->GetElement(mouse_pos));
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
	}
	else if (event.RightDown())
	{
		wxLogDebug(_("Right Down"));
		//m_bSelected = false;
		//HighlightElement(NULL);
    }
	event.Skip();
}

void CfChipSymbolEditorCtrl::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
	//wxPaintDC dc( this );

	//Use double-buffered drawing
	wxBufferedPaintDC dc(this);
    PrepareDC( dc );
	dc.Clear();
	wxSize dc_size = dc.GetSize();
	wxSize client_size = GetClientSize();

	//calculate the offset of scrolled
	int shift_x = this->GetScrollPos(wxHORIZONTAL) * SCROLL_RATE;
	int shift_y = this->GetScrollPos(wxVERTICAL) * SCROLL_RATE;
	wxLogDebug(_("Size :%d,%d; shift :%d,%d"),dc_size.x,dc_size.y,shift_x,shift_y);
	wxLogDebug(_("Client Size :%d,%d; %d,%d"),client_size.x,client_size.y,client_size.GetWidth(),client_size.GetHeight());
	m_pSymbol->Draw(dc,CfScreenView(shift_x,shift_y,client_size.x,client_size.y,1));

	this->SetFocus();
}

void CfChipSymbolEditorCtrl::OnPressKey(wxKeyEvent& event)
{
	if((event.GetKeyCode() == WXK_DELETE) && m_pSymbol->GetElementArray()->Delete(m_pDrawingElement,false))
		this->Refresh();
}
//void CfChipSymbolEditorCtrl::DrawElement(IDrawable *element)
//{
//
//}

void CfChipSymbolEditorCtrl::OnRedoUpdateUI(wxUpdateUIEvent &event)
{
	event.Enable(m_History.CanRedo());
}

void CfChipSymbolEditorCtrl::OnUndoUpdateUI(wxUpdateUIEvent &event)
{
	event.Enable(m_History.CanUndo());
}

void CfChipSymbolEditorCtrl::OnSize(wxSizeEvent& event)
{
	this->Refresh();
}

//void CfChipSymbolEditorCtrl::AddElement()
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
//			HighlightElement(element)
//		}
//
//		AddElement(element);
//		this->Refresh();
//	}
//}

void CfChipSymbolEditorCtrl::NewSymbol()
{
	wxMessageDialog dlg(NULL,wxT("Delete the current symbol?"),PROJECT_CAPTION,wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
	int id = dlg.ShowModal();
	if (id == wxID_NO) return;

	delete m_pSymbol;
	m_pSymbol = (CfSymbol* )CreateDrawingElement(ID_ELEMENT_SYMBOL,NULL,wxEmptyString);

	UpdateLayerView();

	//Update property view
	CfNotification *pNotification2 = new CfNotification(cfId_PropertyView_ShowElement);
	pNotification2->SetElement(m_pSymbol);
	AddNotification(pNotification2);
}


void CfChipSymbolEditorCtrl::ChosePort()
{
	m_DrawingElementID = DRAWING_ELEMENT_PORT;
}

void CfChipSymbolEditorCtrl::ChoseWire()
{
	m_DrawingElementID = DRAWING_ELEMENT_WIRE;
}
void CfChipSymbolEditorCtrl::ChoseSymbol()
{
	m_DrawingElementID = DRAWING_ELEMENT_SYMBOL;
}

bool CfChipSymbolEditorCtrl::CloseEditor()
{
	if (m_pSymbol->Modified())
	{
		wxMessageDialog dlg(NULL,wxT("Save the changes to chip symbol'")+m_FilePath.AfterLast('\\')+_("'?"),PROJECT_CAPTION,wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
		int id = dlg.ShowModal();
		if(id == wxID_YES)
			SaveFile();

	}
	return true;
}
CfDrawingElement* CfChipSymbolEditorCtrl::CopyDrawingElement(CfDrawingElement *source_element)
{
	CfDrawingElement* element = DoCopyDrawingElement(source_element);
	IAction* create = new CfCreate(element);
	AddActionToHistory(create,false,false);
	//m_History.AddActionToHistory(create);
	return element;
}

CfDrawingElement* CfChipSymbolEditorCtrl::CreateDrawingElement(int type_id, CfShape *shape,const wxString& path)
{
	CfDrawingElement* element = DoCreateDrawingElement(this,type_id,shape,path);
	//CfDrawingElementGroup group;
	//group.Add(element);
	IAction* create = new CfCreate(element);
	AddActionToHistory(create,false,false);
	//m_History.AddActionToHistory(create);
	return element;
}

void CfChipSymbolEditorCtrl::Draw(wxDC& dc,const CfScreenView &view,CfDrawingElementGroup* element_group,CfDrawingElementGroup* highlight_array)
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

void CfChipSymbolEditorCtrl::MouseLDown_DrawingElement(wxPoint pt)
{
	switch(m_DrawingElementID)
	{
	case DRAWING_ELEMENT_PORT:
	case DRAWING_ELEMENT_WIRE:	
	case DRAWING_ELEMENT_SYMBOL:	
		{
			m_DrawStartPos = pt;
			m_overlay.Reset();
			break;
		}
	default:	//mabye to drag or not
		{
			if(!m_pSymbol) return;
			m_pDrawingElement = m_pSymbol->GetElement(pt);
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
				if(m_pDrawingElement->IsEndPoint(pt))
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

void CfChipSymbolEditorCtrl::MouseDragging_DrawingElement(wxPoint pt)
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
			int x_move = motion.x - m_DragStartPos.x;
			int y_move = motion.y - m_DragStartPos.y;
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

void CfChipSymbolEditorCtrl::MouseLUp_DrawingElement(wxPoint pt)
{
	if(HasCapture())
			ReleaseMouse();
	switch(m_DrawingElementID)
	{
	case DRAWING_ELEMENT_PORT:
		{
			wxPoint motion(pt);
			wxSize size(motion.x - m_DrawStartPos.x, motion.y - m_DrawStartPos.y);
			if(size.x < 15 || size.y < 15) return;
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
			CfPortShape *port = new CfInputPort(m_DrawStartPos.x, m_DrawStartPos.y, size.x, size.y);
			//m_pDrawingElement = new CfPort(wxEmptyString, rect);
			m_pDrawingElement = CreateDrawingElement(ID_ELEMENT_PORT,port,wxEmptyString);
			m_pDrawingElement->SetShowing(CfEnvironmentUtil::Get()->GetBool(PROP_SETTING_PORT_SHOWN,true));
			AddElement(m_pDrawingElement);
			this->Refresh();
			break;
		}
	case DRAWING_ELEMENT_WIRE:
		{
			CfDrawingElement *element = m_pSymbol->GetElement(m_DrawStartPos);
			if(!element) //&& (element->GetTypeId()==ID_ELEMENT_WIRE))
			{
				element = m_pSymbol->GetElement(pt);
			}
			if(element && (element->GetTypeId()==ID_ELEMENT_WIRE))
			{
				CfWire* wire = (CfWire*)element;
				if(!wire->IsEndPoint(m_DrawStartPos))
					wire->IsEndPoint(pt);
				CfLineSegment *line = NULL;
				line = new CfLineSegment(m_DrawStartPos.x, m_DrawStartPos.y, pt.x , pt.y);	
				wire->Merge(line);
				this->Refresh();
				break;
			}
			else
			{
				CfLineSegment *line = NULL;
				line = new CfLineSegment(m_DrawStartPos.x, m_DrawStartPos.y, pt.x , pt.y);
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
			wxSize size(motion.x - m_DrawStartPos.x, motion.y - m_DrawStartPos.y);
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
			CfRectangle *rect = new CfRectangle(m_DrawStartPos.x, m_DrawStartPos.y, size.x, size.y);
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
			CfDrawingElementGroup *allElement = m_pSymbol->GetElementArray();
			if(allElement->Count() == 0) 
			{
				this->Refresh();
				break;
			}
			else
			{
				HighlightElementList.Clear();
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

void CfChipSymbolEditorCtrl::UpdateLayerView()
{	
	CfNotification *notify= new CfNotification(cfId_LayerView_Update);
	notify->SetElement(m_pSymbol);
	AddNotification(notify);
}

void CfChipSymbolEditorCtrl::UpdateScrollBar()
{
	//Set the scroll bar
	wxPoint point = m_pSymbol->GetEndPos();
	SetVirtualSize(wxSize(point.x,point.y));
	SetScrollRate(SCROLL_RATE,SCROLL_RATE); // Will be changable later
	Refresh();

}