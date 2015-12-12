/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfAboutDialogCtrl.cpp
// Author      :James
// Created     :2010-04-02 ¿ÀÀü 10:41:07
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#include <wx/sizer.h>
#include "CfDefines.h"
#include "FileUtil.h"
#include "CfAboutDialogCtrl.h"

CfAboutDialogCtrl::CfAboutDialogCtrl(wxWindow* pParent,FileUtil* pFileUtils)
//:wxDialog(pParent,wxID_ANY,wxEmptyString,wxDefaultPosition, wxSize(500,260)),m_pProjectItem(pProjectItem)
:CfDialogBaseCtrl(pParent,_(""),wxSize(600,450),_("About CircuitFinder"))
,m_pFileUtils(pFileUtils)

{
		SetTitle(_("About CircuitFinder"));


		m_pRichTextCtrl = new wxRichTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(570, 280), wxVSCROLL|wxHSCROLL|wxWANTS_CHARS|wxRE_READONLY);
		wxFont font(12, wxROMAN, wxNORMAL, wxNORMAL);
		m_pRichTextCtrl->SetFont(font);
		//m_pRichTextCtrl->SetStyleSheet(wxGetApp().GetStyleSheet());
		
        wxBoxSizer* s1 = new wxBoxSizer(wxHORIZONTAL);
		wxButton* btn = new wxButton(this, wxID_OK, wxT("OK"),wxDefaultPosition, wxSize(80,25));
		FileUtil fu;
		wxString dir = fu.GetDirectory(ID_DOCDIR) + _("\\");
		wxHyperlinkCtrl *lnk = new wxHyperlinkCtrl(this,wxID_ANY,_("Change Log"),dir+_("ChangeLog.txt"));
        s1->Add(15, 1);
		s1->Add(new wxStaticText(this, wxID_ANY, VERSION));
        s1->Add(10, 10);
		s1->Add(lnk);
        s1->Add(290, 10,1, wxEXPAND);
        s1->Add(btn,0,wxALIGN_RIGHT);
        //s1->Add(1, 1, 1, wxEXPAND);

		wxBoxSizer* s_all = new wxBoxSizer(wxVERTICAL);
		s_all->Add(10, 70, 0);
		s_all->Add(m_pRichTextCtrl, 0, wxCENTER, 5);
		s_all->Add(10, 10, 0);
		s_all->Add(s1, 1);
		//s_all->Add(s1, 1, wxLEFT,5);
		s_all->Add(10, 20, 0);
		SetSizer(s_all);
		//GetSizer()->Fit(this);


		WriteInitialText();
		btn->SetFocus();
}


BEGIN_EVENT_TABLE(CfAboutDialogCtrl,CfDialogBaseCtrl)

//EVT_BUTTON(ID_ApplyBtn,CfProjectPropertyDialogCtrl::OnPushApplyButton)
    EVT_TEXT_URL(wxID_ANY, CfAboutDialogCtrl::OnURL)
	EVT_HYPERLINK(wxID_ANY,CfAboutDialogCtrl::OnClickChangeLog)

END_EVENT_TABLE()

CfAboutDialogCtrl::~CfAboutDialogCtrl(void)
{
}

void CfAboutDialogCtrl::WriteInitialText()
{
	wxString str;
    wxRichTextCtrl& r = *m_pRichTextCtrl;

    r.SetDefaultStyle(wxRichTextAttr());
    r.BeginSuppressUndo();
	r.BeginParagraphSpacing(0, 20);

    r.BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
    r.BeginBold();

    r.BeginFontSize(14);

    wxString lineBreak = (wxChar) 29;

    r.WriteText(wxString(wxT("Reverse engineering tool")) + lineBreak);
    r.EndFontSize();
 
    r.BeginItalic();
	str.Printf(wxT("(CircuitFinder %s)\n"),RELEASE);
    r.WriteText(str);
    r.EndItalic();
    r.EndAlignment();

	r.BeginAlignment(wxTEXT_ALIGNMENT_LEFT);

	r.Newline();
    r.WriteText(wxT("Reverse engineering tool extract layout data from image files which are captured by microscope.By decapping the surface of semiconductor chip, the circuit image can be shown.The original layout drawing can be extracted from this image.\n"));


    wxRichTextAttr urlStyle;
    urlStyle.SetTextColour(*wxBLUE);
    urlStyle.SetFontUnderlined(true);

	r.WriteText(wxT("Copyright(c) 2015-2020 All Rights Reserved.\t\t\t\t"));

	wxImage bitmap;
	bitmap.LoadFile(m_pFileUtils->GetDirectory(ID_RESDIR) + wxT("SCLogo.bmp")); 
	if(bitmap.IsOk())
		r.WriteImage(bitmap,wxBITMAP_TYPE_BMP);

	r.Newline();
	r.WriteText(wxT("System Centroid Inc. ("));
    r.BeginStyle(urlStyle);
    r.BeginURL(wxT("https://www.nnfc.re.kr"));
    r.WriteText(wxT("www.nnfc.re.kr"));
    r.EndURL();
    r.EndStyle();
	r.WriteText(wxT(")"));
	r.EndAlignment();


	r.EndParagraphSpacing();
    r.EndSuppressUndo();

}

void CfAboutDialogCtrl::OnURL(wxTextUrlEvent& event)
{
	wxLaunchDefaultBrowser(event.GetString());
    //wxMessageBox(event.GetString());
}

void CfAboutDialogCtrl::OnClickChangeLog(wxHyperlinkEvent& event)
{
	wxLaunchDefaultBrowser(event.GetURL());
	
}