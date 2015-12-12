#include "ImageEditorCtrl.h"
#include "EnvironmentUtil.h"
#include "DrawingElementFactory.h"
#include "LogMessageCtrl.h"
#include "IDrawable.h"
#include "Port.h"
#include "Wire.h"
#include<cmath>

BEGIN_EVENT_TABLE (CfImageEditorCtrl, wxScrolledWindow)

  EVT_PAINT(CfImageEditorCtrl::OnPaint)
  EVT_MOUSE_EVENTS(CfImageEditorCtrl::OnMouseEvent)

END_EVENT_TABLE()

//int CfImageEditorCtrl::m_DrawingElementID = DRAWING_ELEMENT_NONE;
//CfDrawingElement* CfImageEditorCtrl::m_pDrawingElement = NULL;

CfImageEditorCtrl::CfImageEditorCtrl(const wxString &chip_path, const wxString &schematic_path,
			wxWindow *parent, wxWindowID id,
            const wxPoint &pos,
            const wxSize &size
)
: wxScrolledWindow(parent, id, pos, size), CfFileEditorBase(chip_path), m_Image(this,wxEmptyString,wxEmptyString,chip_path), m_View(0,0,1000,1000,1)
,CfNotificationSender(parent)
{

	//CfRectangle *rect = new CfRectangle(100,100,150,150);
	//CfRectangle *rect2 = new CfRectangle(30,30,100,100);
	//CfRectangle *rect3 = new CfRectangle(200,200,100,100);
	//CfSymbol *sym = new CfSymbol(_("Test symbol"),rect);
	//CfPort *port1 = new CfPort(_("Test port1"),rect2);
	//CfPort *port2 = new CfPort(_("Test port2"),rect3);
	//sym->GetElementArray().Add(port1);
	//m_Panel->GetElementArray().Add(sym);

	//m_Panel->GetElementArray().Add(port1);

	//m_Panel->GetElementArray().Add(port2);

}


CfImageEditorCtrl::~CfImageEditorCtrl(void)
{
}

void CfImageEditorCtrl::Activated()
{
}
bool CfImageEditorCtrl::LoadFile()
{
	SetVirtualSize(m_Image.GetSize());
	SetScrollRate(1,1); // Will be changable later

	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	Refresh();
	return true;
}
void CfImageEditorCtrl::ProcessEvent(wxCommandEvent &event)
{
	int id = event.GetId();
	switch(id)
	{
	case MENU_ID_PORT:
		OnChosePort();
		break;
	case MENU_ID_WIRE:
		OnChoseWire();
		break;
	}
}

cfReaderID CfImageEditorCtrl::GetReaderId() const
{
	return FR_IMAGE_EDITOR;
}

void CfImageEditorCtrl::OnMouseEvent(wxMouseEvent& event)
{
    if (event.LeftDown())
    {
		int x = event.GetX();
		int y = event.GetY();
		wxLogDebug(_("Left Down: %d,%d"),x,y);		
		return;		
    }
	else if (event.Dragging())
	{
		if(event.LeftIsDown())
		{
			return;
		}
	}
	else if (event.LeftUp())
	{
		CfNotification *pNotification = new CfNotification(cfId_PropertyView_ShowElement);
			pNotification->SetElement(&m_Image);
			AddNotification(pNotification);
	}
	else if (event.RightDown())
	{
		wxLogDebug(_("Right Down"));
    }
	event.Skip();
}

void CfImageEditorCtrl::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc( this );
    PrepareDC( dc );
	dc.Clear();

	wxLogDebug(_("Dc Size: %dx%d"),dc.GetSize().x,dc.GetSize().y);
	wxLogDebug(_("Dc Size: %dx%d"),GetVirtualSize().x,GetVirtualSize().y);
   // dc.DrawText( wxT("Loaded image"), 0, 0 );
	m_Image.Draw(dc,CfScreenView(0,0,1000,1000,1));
}

void CfImageEditorCtrl::OnChosePort()
{
}

void CfImageEditorCtrl::OnChoseWire()
{
}