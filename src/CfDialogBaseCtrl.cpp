/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfDialogBaseCtrl.cpp
// Author      :James
// Created     :2010-03-18 ¿ÀÈÄ 3:15:41
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

//#include "fiiDefines.h"
#include "ResourceUtil.h"
#include "CfDialogBaseCtrl.h"

CfDialogBaseCtrl::CfDialogBaseCtrl(wxWindow* pParent,const wxChar* title,const wxSize &size,const wxChar* caption)
:wxDialog(pParent,wxID_ANY,title,wxDefaultPosition, size),m_strCaption(caption)
{
	//wxStaticBitmap* header_img = new wxStaticBitmap(this,wxID_ANY, wxBitmap(wxT("IDB_DLG_HEADER")));
		//wxImage image(wxT("res\\Dialog Header.bmp"));
		//m_HeaderImage = wxBitmap(wxT("IDB_DLG_HEADER"));
		m_HeaderImage = ResourceUtil::Get()->GetBitmap(IMG_DLG_HEADER);

		wxImage image = m_HeaderImage.ConvertToImage();
		
		m_HeaderImage = wxBitmap(image.Scale(size.GetWidth(),(image.GetHeight()-10)*size.GetWidth()/image.GetWidth()));
}
void CfDialogBaseCtrl::SetCaption(wxString caption)
{
	m_strCaption = caption;
}


void CfDialogBaseCtrl::OnPaint(wxPaintEvent& event)
{
	wxDialog::OnPaint(event);
	wxPaintDC dc(this);
	dc.DrawBitmap(m_HeaderImage,0,0);
	dc.SetFont( wxFont(10, wxSWISS, wxNORMAL, wxBOLD) );
      //dc.SetFont(*wxNORMAL_FONT);
    dc.SetPen(*wxBLACK_PEN);

	dc.DrawText(m_strCaption,10,10);
	
}

BEGIN_EVENT_TABLE(CfDialogBaseCtrl,wxDialog)
EVT_PAINT(CfDialogBaseCtrl::OnPaint)
END_EVENT_TABLE()
