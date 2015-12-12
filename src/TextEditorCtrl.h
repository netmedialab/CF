/////////////////////////////////////////////////////////////////////////////////
//
// Name        :TextEditorCtrl.h
// Author      :MING
// Created     :2010-04-08 ¿ÀÈÄ 1:51:37
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////


#ifndef _FII_WX_TEXT_EDITOR_H_
#define _FII_WX_TEXT_EDITOR_H_

//----------------------------------------------------------------------------
// informations
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

//! wxWidgets headers

//! wxStyledTextCtrl headers
#include "wx/stc/stc.h"  // styled text control

//! application headers
#include "prefs.h"       // preferences
#include "FileEditorBase.h"
#include "Notification.h"
//============================================================================
// declarations
//============================================================================

class EditPrint;
class EditProperties;

//----------------------------------------------------------------------------
//! CfTextEditorCtrl
class CfTextEditorCtrl: public wxStyledTextCtrl, public CfFileEditorBase, public CfNotificationSender
{
    friend class EditProperties;
    friend class EditPrint;

public:
    //! constructor
	CfTextEditorCtrl (
		const wxChar* file_path,
		wxWindow *parent, wxWindowID id = wxID_ANY,
          const wxPoint &pos = wxDefaultPosition,
          const wxSize &size = wxDefaultSize,
          long style = wxSUNKEN_BORDER|wxVSCROLL
         );

    //! destructor
    ~CfTextEditorCtrl ();

    void DisplayEOL ();
    void ShowIndentGuide ();
    void ShowLineNumber ();
    void ShowLongLineOn ();
    void ShowWhiteSpace ();
    void FoldToggle ();
	void WrapmodeOn();



    // event handlers
    // common
    void OnSize( wxSizeEvent &event );
    // edit
    void OnEditRedo (wxCommandEvent &event);
    void OnEditUndo (wxCommandEvent &event);
    void OnEditClear (wxCommandEvent &event);
    void OnEditCut (wxCommandEvent &event);
    void OnEditCopy (wxCommandEvent &event);
    void OnEditPaste (wxCommandEvent &event);
    // find
    void OnFind (wxCommandEvent &event);
    void OnFindNext (wxCommandEvent &event);
    void OnReplace (wxCommandEvent &event);
    void OnReplaceNext (wxCommandEvent &event);
    void OnBraceMatch (wxCommandEvent &event);
    void OnGoto (wxCommandEvent &event);
    void OnEditIndentInc (wxCommandEvent &event);
    void OnEditIndentRed (wxCommandEvent &event);
    void OnEditSelectAll (wxCommandEvent &event);
    void OnEditSelectLine (wxCommandEvent &event);
    //! view
    void OnHilightLang (wxCommandEvent &event);
    void OnDisplayEOL (wxCommandEvent &event);
    void OnIndentGuide (wxCommandEvent &event);
    void OnLineNumber (wxCommandEvent &event);
    void OnLongLineOn (wxCommandEvent &event);
    void OnWhiteSpace (wxCommandEvent &event);
    void OnFoldToggle (wxCommandEvent &event);
    void OnSetOverType (wxCommandEvent &event);
    void OnSetReadOnly (wxCommandEvent &event);
    void OnWrapmodeOn (wxCommandEvent &event);
    void OnUseCharset (wxCommandEvent &event);
    //! extra
    void OnChangeCase (wxCommandEvent &event);
    void OnConvertEOL (wxCommandEvent &event);
    // stc
    void OnMarginClick (wxStyledTextEvent &event);
    void OnCharAdded  (wxStyledTextEvent &event);

	void OnCaptureMouse(wxMouseCaptureLostEvent& event);

	// context menu
	void OnContextMenu(wxContextMenuEvent &event);

	void OnCaretChange(wxStyledTextEvent& event);
	void OnTextChanged(wxStyledTextEvent& event);
    void CheckIfCanFold(wxUpdateUIEvent &event);


    //! language/lexer
    wxString DeterminePrefs (const wxString &filename);
    bool InitializePrefs (const wxString &filename);
    bool UserSettings (const wxString &filename);
    LanguageInfo const* GetLanguageInfo () {return m_language;};

    //! load/save file
    bool LoadFile ();
    bool LoadFile (const wxString &filename);
    bool SaveFileAs (const wxString &filename);
    //bool SaveFile (bool new_file=false);
    bool SaveFile ();
    bool SaveFile (const wxString &filename);
    bool Modified ();
	void SetCaretAt(long pos);

	bool CloseEditor();

	// find
	bool SearchText(int flags,wxString &text, bool search_up);

	//CfFileEditorBase
	virtual void Activated();
	bool OpenFile(const wxString &file_path);
	void ProcessEvent(wxCommandEvent &event);
	cfReaderID GetReaderId() const;
    wxString GetFilename () {return m_FilePath;};
    void SetFilename (const wxString &filename) {m_FilePath = filename;};

private:
    // lanugage properties
    LanguageInfo const* m_language;

	bool m_bStartAndEndFlag;

    // margin variables
    int m_LineNrID;
    int m_LineNrMargin;
    int m_FoldingID;
    int m_FoldingMargin;
    int m_DividerID;
	int m_iCaretPos;

    DECLARE_EVENT_TABLE()
};

//----------------------------------------------------------------------------
//! EditProperties
class EditProperties: public wxDialog {

public:

    //! constructor
    EditProperties (CfTextEditorCtrl *edit, long style = 0);

private:

};

#if wxUSE_PRINTING_ARCHITECTURE

//----------------------------------------------------------------------------
//! EditPrint
class EditPrint: public wxPrintout {

public:

    //! constructor
    EditPrint (CfTextEditorCtrl *edit, wxChar *title = _T(""));

    //! event handlers
    bool OnPrintPage (int page);
    bool OnBeginDocument (int startPage, int endPage);

    //! print functions
    bool HasPage (int page);
    void GetPageInfo (int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);

private:
    CfTextEditorCtrl *m_edit;
    int m_printed;
    wxRect m_pageRect;
    wxRect m_printRect;

    bool PrintScaling (wxDC *dc);
};

#endif // wxUSE_PRINTING_ARCHITECTURE

#endif // _EDIT_H_
