/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfAboutDialogCtrl.h
// Author      :James
// Created     :2010-04-02 ¿ÀÀü 10:37:52
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <wx/hyperlink.h>
#include "wx/richtext/richtextctrl.h"
#include "FileUtil.h"
#include "CfDialogBaseCtrl.h"


#define VERSION _("Version 1.0")
#define RELEASE _("R1.0")

class CfAboutDialogCtrl : public CfDialogBaseCtrl
{
public:
	CfAboutDialogCtrl(wxWindow* pParent,FileUtil* pFileUtils);
	~CfAboutDialogCtrl(void);

private:

	FileUtil *m_pFileUtils;
	wxRichTextCtrl	*m_pRichTextCtrl;
	void WriteInitialText();
    void OnURL(wxTextUrlEvent& event);
	void OnClickChangeLog(wxHyperlinkEvent& event);

	DECLARE_EVENT_TABLE()

};