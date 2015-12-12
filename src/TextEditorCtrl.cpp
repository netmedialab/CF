//////////////////////////////////////////////////////////////////////////////
// File:        contrib/samples/stc/edit.cpp
// Purpose:     SCI test module
// Maintainer:  Wyo
// Created:     2003-09-01
// RCS-ID:      $Id: edit.cpp 39402 2006-05-28 23:32:12Z VZ $
// Copyright:   (c) wxGuide
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// informations
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "CfWx.h"
#include "TextEditorCtrl.h"        // edit module
#include "LogMessageCtrl.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all 'standard' wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//! wxWidgets headers
#include "wx/file.h"     // raw file io support
#include "wx/filename.h" // filename support

//! application headers
#include "defsext.h"     // additional definitions

//----------------------------------------------------------------------------
// resources
//----------------------------------------------------------------------------


//============================================================================
// declarations
//============================================================================


//============================================================================
// implementation
//============================================================================

//----------------------------------------------------------------------------
// CfTextEditorCtrl
//----------------------------------------------------------------------------

BEGIN_EVENT_TABLE (CfTextEditorCtrl, wxStyledTextCtrl)
    // common
    EVT_SIZE (                         CfTextEditorCtrl::OnSize)

	// edit
    EVT_MENU (wxID_CLEAR,              CfTextEditorCtrl::OnEditClear)
    EVT_MENU (wxID_CUT,                CfTextEditorCtrl::OnEditCut)
    EVT_MENU (wxID_COPY,               CfTextEditorCtrl::OnEditCopy)
    EVT_MENU (wxID_PASTE,              CfTextEditorCtrl::OnEditPaste)
    EVT_MENU (MENU_ID_INDENTINC,       CfTextEditorCtrl::OnEditIndentInc)
    EVT_MENU (MENU_ID_INDENTRED,       CfTextEditorCtrl::OnEditIndentRed)
    EVT_MENU (wxID_SELECTALL,          CfTextEditorCtrl::OnEditSelectAll)
    EVT_MENU (MENU_ID_SELECTLINE,      CfTextEditorCtrl::OnEditSelectLine)
    EVT_MENU (wxID_REDO,               CfTextEditorCtrl::OnEditRedo)
    EVT_MENU (wxID_UNDO,               CfTextEditorCtrl::OnEditUndo)

    // stc
    EVT_STC_MARGINCLICK (wxID_ANY,     CfTextEditorCtrl::OnMarginClick)
	EVT_STC_CHARADDED (wxID_ANY,       CfTextEditorCtrl::OnCharAdded)
	EVT_STC_MODIFIED(wxID_ANY,			CfTextEditorCtrl::OnTextChanged)
	//EVT_STC_CHANGE(wxID_ANY,			CfTextEditorCtrl::OnTextChanged)
    EVT_UPDATE_UI (MENU_ID_FOLDTOGGLE,   CfTextEditorCtrl::CheckIfCanFold)
	EVT_MOUSE_CAPTURE_LOST(CfTextEditorCtrl::OnCaptureMouse)
	//EVT_SCI_PAINTED(wxID_ANY, CfTextEditorCtrl::OnCaretChange) // MING_20110422

	// MING 101027
	EVT_CONTEXT_MENU(CfTextEditorCtrl::OnContextMenu)

END_EVENT_TABLE()

CfTextEditorCtrl::CfTextEditorCtrl (
			const wxChar* file_path,
			wxWindow *parent, wxWindowID id,
            const wxPoint &pos,
            const wxSize &size,
            long style
			//CfiiFileItem *item
//			const wxChar* file_path
)
: wxStyledTextCtrl(parent, id, pos, size, style), CfFileEditorBase(file_path)
,CfNotificationSender(parent)
,m_bStartAndEndFlag(false)
{
    m_LineNrID = 0;
    m_DividerID = 1;
    m_FoldingID = 2;

    // initialize language
    m_language = NULL;

    // default font for all styles
    SetViewEOL (g_EditorCommonPrefs.displayEOLEnable);
    SetIndentationGuides (g_EditorCommonPrefs.indentGuideEnable);
    SetEdgeMode (g_EditorCommonPrefs.longLineOnEnable?
                 wxSTC_EDGE_LINE: wxSTC_EDGE_NONE);
    SetViewWhiteSpace (g_EditorCommonPrefs.whiteSpaceEnable? wxSTC_WS_VISIBLEALWAYS: wxSTC_WS_INVISIBLE);
    SetOvertype (g_EditorCommonPrefs.overTypeInitial);
    SetReadOnly (g_EditorCommonPrefs.readOnlyInitial);
    SetWrapMode (g_EditorCommonPrefs.wrapModeInitial? wxSTC_WRAP_WORD: wxSTC_WRAP_NONE);

    wxFont font (10, wxMODERN, wxNORMAL, wxNORMAL);
    StyleSetFont (wxSTC_STYLE_DEFAULT, font);
    StyleSetForeground (wxSTC_STYLE_DEFAULT, *wxBLACK);
    StyleSetBackground (wxSTC_STYLE_DEFAULT, *wxWHITE);
    StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColour (_T("DARK GREY")));
    StyleSetBackground (wxSTC_STYLE_LINENUMBER, *wxWHITE);
    StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColour (_T("DARK GREY")));
    InitializePrefs (DEFAULT_LANGUAGE);

    // set visibility
    SetVisiblePolicy (wxSTC_VISIBLE_STRICT|wxSTC_VISIBLE_SLOP, 1);
    SetXCaretPolicy (wxSTC_CARET_EVEN|wxSTC_VISIBLE_STRICT|wxSTC_CARET_SLOP, 1);
    SetYCaretPolicy (wxSTC_CARET_EVEN|wxSTC_VISIBLE_STRICT|wxSTC_CARET_SLOP, 1);

	// markers
	MarkerDefine (wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS);
    MarkerSetBackground (wxSTC_MARKNUM_FOLDER, wxColour (_T("BLACK")));
    MarkerSetForeground (wxSTC_MARKNUM_FOLDER, wxColour (_T("WHITE")));
    MarkerDefine (wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS);
    MarkerSetBackground (wxSTC_MARKNUM_FOLDEROPEN, wxColour (_T("BLACK")));
    MarkerSetForeground (wxSTC_MARKNUM_FOLDEROPEN, wxColour (_T("WHITE")));
    MarkerDefine (wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);
    MarkerDefine (wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_SHORTARROW);
    MarkerDefine (wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN);
    MarkerDefine (wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
    MarkerDefine (wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);

    CmdKeyClear (wxSTC_KEY_TAB, 0);
    CmdKeyClear (wxSTC_KEY_TAB, wxSTC_SCMOD_SHIFT);

	//// markers
 //   MarkerDefine (wxSTC_MARKNUM_FOLDER,        wxSTC_MARK_DOTDOTDOT, _T("BLACK"), _T("BLACK"));
 //   MarkerDefine (wxSTC_MARKNUM_FOLDEROPEN,    wxSTC_MARK_ARROWDOWN, _T("BLACK"), _T("BLACK"));
 //   MarkerDefine (wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_EMPTY,     _T("BLACK"), _T("BLACK"));
 //   MarkerDefine (wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_DOTDOTDOT, _T("BLACK"), _T("WHITE"));
 //   MarkerDefine (wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, _T("BLACK"), _T("WHITE"));
 //   MarkerDefine (wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY,     _T("BLACK"), _T("BLACK"));
 //   MarkerDefine (wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_EMPTY,     _T("BLACK"), _T("BLACK"));

    // miscelaneous
    m_LineNrMargin = TextWidth (wxSTC_STYLE_LINENUMBER, _T("_999999"));
    m_FoldingMargin = 16;
    CmdKeyClear (wxSTC_KEY_TAB, 0); // this is done by the menu accelerator key
    SetLayoutCache (wxSTC_CACHE_PAGE);
	SetModEventMask(wxSTC_MOD_INSERTTEXT|wxSTC_MOD_DELETETEXT);

}

CfTextEditorCtrl::~CfTextEditorCtrl () 
{
	CloseEditor();
}

void CfTextEditorCtrl::Activated()
{
	//clear property view
	CfNotification *pNotification = new CfNotification(cfId_PropertyView_ShowElement);
	pNotification->SetElement(NULL);
	AddNotification(pNotification);

	//clear layer view
	CfNotification *notify= new CfNotification(cfId_LayerView_Update);
	notify->SetElement(NULL);
	AddNotification(notify);
}

void CfTextEditorCtrl::DisplayEOL () {
    SetViewEOL (!GetViewEOL());
}

void CfTextEditorCtrl::ShowIndentGuide () {
    SetIndentationGuides (!GetIndentationGuides());
}

void CfTextEditorCtrl::ShowLineNumber () {
    SetMarginWidth (m_LineNrID, GetMarginWidth (m_LineNrID) == 0? m_LineNrMargin: 0);
}

void CfTextEditorCtrl::ShowLongLineOn () {
    SetEdgeMode (GetEdgeMode() == 0? wxSTC_EDGE_LINE: wxSTC_EDGE_NONE);
}

void CfTextEditorCtrl::ShowWhiteSpace () {
    SetViewWhiteSpace (GetViewWhiteSpace() == 0? wxSTC_WS_VISIBLEALWAYS: wxSTC_WS_INVISIBLE);
}

void CfTextEditorCtrl::FoldToggle () {
    //int n = GetFoldParent(GetCurrentLine()+1);
	ToggleFold (GetFoldParent(GetCurrentLine()+1));
}

void CfTextEditorCtrl::WrapmodeOn () {
    SetWrapMode (GetWrapMode() == 0? wxSTC_WRAP_WORD: wxSTC_WRAP_NONE);

}


void CfTextEditorCtrl::OnCaptureMouse(wxMouseCaptureLostEvent& event)
{
	m_iCaretPos = 0;
	//
}


//----------------------------------------------------------------------------
// common event handlers
void CfTextEditorCtrl::OnSize( wxSizeEvent& event ) {
    int x = GetClientSize().x +
            (g_EditorCommonPrefs.lineNumberEnable? m_LineNrMargin: 0) +
            (g_EditorCommonPrefs.foldEnable? m_FoldingMargin: 0);
    if (x > 0) SetScrollWidth (x);
    event.Skip();
}

// edit event handlers
void CfTextEditorCtrl::OnEditRedo (wxCommandEvent &WXUNUSED(event)) {
    if (!CanRedo()) return;
    Redo ();
}

void CfTextEditorCtrl::OnEditUndo (wxCommandEvent &WXUNUSED(event)) {
    if (!CanUndo()) return;
    Undo ();
}

void CfTextEditorCtrl::OnEditClear (wxCommandEvent &WXUNUSED(event)) {
    if (GetReadOnly()) return;
    Clear ();
}

void CfTextEditorCtrl::OnEditCut (wxCommandEvent &WXUNUSED(event)) {
    if (GetReadOnly() || (GetSelectionEnd()-GetSelectionStart() <= 0)) return;
    Cut ();
}

void CfTextEditorCtrl::OnEditCopy (wxCommandEvent &WXUNUSED(event)) {
    if (GetSelectionEnd()-GetSelectionStart() <= 0) return;
    Copy ();
}

void CfTextEditorCtrl::OnEditPaste (wxCommandEvent &WXUNUSED(event)) {
    if (!CanPaste()) return;
    Paste ();
}

void CfTextEditorCtrl::OnFind (wxCommandEvent &WXUNUSED(event)) {
}

void CfTextEditorCtrl::OnFindNext (wxCommandEvent &WXUNUSED(event)) {
}

void CfTextEditorCtrl::OnReplace (wxCommandEvent &WXUNUSED(event)) {
}

void CfTextEditorCtrl::OnReplaceNext (wxCommandEvent &WXUNUSED(event)) {
}

void CfTextEditorCtrl::OnBraceMatch (wxCommandEvent &WXUNUSED(event)) {
    int min = GetCurrentPos ();
    int max = BraceMatch (min);
    if (max > (min+1)) {
        BraceHighlight (min+1, max);
        SetSelection (min+1, max);
    }else{
        BraceBadLight (min);
    }
}

void CfTextEditorCtrl::OnGoto (wxCommandEvent &WXUNUSED(event)) {
}

void CfTextEditorCtrl::OnEditIndentInc (wxCommandEvent &WXUNUSED(event)) {
    CmdKeyExecute (wxSTC_CMD_TAB);
}

void CfTextEditorCtrl::OnEditIndentRed (wxCommandEvent &WXUNUSED(event)) {
    CmdKeyExecute (wxSTC_CMD_DELETEBACK);
}

void CfTextEditorCtrl::OnEditSelectAll (wxCommandEvent &WXUNUSED(event)) {
    SetSelection (0, GetTextLength ());
}

void CfTextEditorCtrl::OnEditSelectLine (wxCommandEvent &WXUNUSED(event)) {
    int lineStart = PositionFromLine (GetCurrentLine());
    int lineEnd = PositionFromLine (GetCurrentLine() + 1);
    SetSelection (lineStart, lineEnd);
}

void CfTextEditorCtrl::OnHilightLang (wxCommandEvent &event) {
    InitializePrefs (g_LanguagePrefs [event.GetId() - MENU_ID_HILIGHTFIRST].name);
}

void CfTextEditorCtrl::OnDisplayEOL (wxCommandEvent &WXUNUSED(event)) {
    SetViewEOL (!GetViewEOL());
}

void CfTextEditorCtrl::OnIndentGuide (wxCommandEvent &WXUNUSED(event)) {
    SetIndentationGuides (!GetIndentationGuides());
}

void CfTextEditorCtrl::OnLineNumber (wxCommandEvent &WXUNUSED(event)) {
    SetMarginWidth (m_LineNrID,
                    GetMarginWidth (m_LineNrID) == 0? m_LineNrMargin: 0);
}

void CfTextEditorCtrl::OnLongLineOn (wxCommandEvent &WXUNUSED(event)) {
    SetEdgeMode (GetEdgeMode() == 0? wxSTC_EDGE_LINE: wxSTC_EDGE_NONE);
}

void CfTextEditorCtrl::OnWhiteSpace (wxCommandEvent &WXUNUSED(event)) {
    SetViewWhiteSpace (GetViewWhiteSpace() == 0?
                       wxSTC_WS_VISIBLEALWAYS: wxSTC_WS_INVISIBLE);
}

void CfTextEditorCtrl::OnFoldToggle (wxCommandEvent &WXUNUSED(event)) {
    ToggleFold (GetFoldParent(GetCurrentLine()));
}

void CfTextEditorCtrl::OnSetOverType (wxCommandEvent &WXUNUSED(event)) {
    SetOvertype (!GetOvertype());
}

void CfTextEditorCtrl::OnSetReadOnly (wxCommandEvent &WXUNUSED(event)) {
    SetReadOnly (!GetReadOnly());
}

void CfTextEditorCtrl::OnWrapmodeOn (wxCommandEvent &WXUNUSED(event)) {
    SetWrapMode (GetWrapMode() == 0? wxSTC_WRAP_WORD: wxSTC_WRAP_NONE);
}

void CfTextEditorCtrl::OnUseCharset (wxCommandEvent &event) {
    int Nr;
    int charset = GetCodePage();
    switch (event.GetId()) {
        case MENU_ID_CHARSETANSI: {charset = wxSTC_CHARSET_ANSI; break;}
        case MENU_ID_CHARSETMAC: {charset = wxSTC_CHARSET_ANSI; break;}
    }
    for (Nr = 0; Nr < wxSTC_STYLE_LASTPREDEFINED; Nr++) {
        StyleSetCharacterSet (Nr, charset);
    }
    SetCodePage (charset);
}

void CfTextEditorCtrl::OnChangeCase (wxCommandEvent &event) {
    switch (event.GetId()) {
        case MENU_ID_CHANGELOWER: {
            CmdKeyExecute (wxSTC_CMD_LOWERCASE);
            break;
        }
        case MENU_ID_CHANGEUPPER: {
            CmdKeyExecute (wxSTC_CMD_UPPERCASE);
            break;
        }
    }
}

void CfTextEditorCtrl::OnConvertEOL (wxCommandEvent &event) {
    int eolMode = GetEOLMode();
    switch (event.GetId()) {
        case MENU_ID_CONVERTCR: { eolMode = wxSTC_EOL_CR; break;}
        case MENU_ID_CONVERTCRLF: { eolMode = wxSTC_EOL_CRLF; break;}
        case MENU_ID_CONVERTLF: { eolMode = wxSTC_EOL_LF; break;}
    }
    ConvertEOLs (eolMode);
    SetEOLMode (eolMode);
}

//! misc
void CfTextEditorCtrl::OnMarginClick (wxStyledTextEvent &event) {
    if (event.GetMargin() == 2) {
        int lineClick = LineFromPosition (event.GetPosition());
        int levelClick = GetFoldLevel (lineClick);
        if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) {
            ToggleFold (lineClick);
        }
    }
}


void CfTextEditorCtrl::CheckIfCanFold(wxUpdateUIEvent &event)
{
	/*int pos = GetCurrentPos();
	int lineClick = LineFromPosition (pos);
    int levelClick = GetFoldLevel (lineClick);
	if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) */
	int n = GetFoldParent(GetCurrentLine()+1);
	if(n >= 0) event.Enable(true);
	else event.Enable(false);
}


void CfTextEditorCtrl::OnCharAdded (wxStyledTextEvent &event) {
    char chr = (char)event.GetKey();
    int currentLine = GetCurrentLine();
    // Change this if support for mac files with \r is needed
    if (chr == '\n') {
        int lineInd = 0;
        if (currentLine > 0) {
            lineInd = GetLineIndentation(currentLine - 1);
        }
        if (lineInd == 0) return;
        SetLineIndentation (currentLine, lineInd);
        GotoPos(PositionFromLine (currentLine) + lineInd);
    }
}

void CfTextEditorCtrl::OnTextChanged(wxStyledTextEvent& event)
{
	//Do not send notification when loading and closing
	if(m_bStartAndEndFlag)
	{
		m_bStartAndEndFlag = false;
		return;
	}

	CfNotification *pNotification = new CfNotification(cfId_Editor_Modified);
	AddNotification(pNotification);
}

//----------------------------------------------------------------------------
// context menu
void CfTextEditorCtrl::OnContextMenu(wxContextMenuEvent &event)
{
    //wxPoint pt = event.GetPosition();
	wxPoint pt = ScreenToClient(wxGetMousePosition());

    //wxLogMessage(wxT("OnContextMenu at screen coords (%i, %i)"), pt.x, pt.y);
	wxMenu menu;
	menu.Append(wxID_UNDO,_("&Undo\tCtrl+Z"));
	menu.Append(wxID_REDO,_("&Redo\tCtrl+Shift+Z"));
    menu.AppendSeparator();
	menu.Append(wxID_FIND,_("&Find and Replace\tCtrl+F"));
	menu.Append(MENU_ID_FINDNEXT,_("Find &Next\tF3"));
	menu.Append(MENU_ID_FindInFiles,_("Find &in Files\tCtrl+Shift+F"));
	menu.Append(MENU_ID_Goto,_("&Go To...\tCtrl+G"));

    menu.AppendSeparator();
	menu.Append(wxID_CUT,_("&Cut\tCtrl+X"));
	menu.Append(wxID_COPY,_("C&opy\tCtrl+C"));
	menu.Append(wxID_PASTE,_("&Paste\tCtrl+V"));
    menu.AppendSeparator();
	menu.Append(wxID_SELECTALL,_("&Select All\tCtrl+A"));

    PopupMenu(&menu, pt);

}

//<<MING_20110422
//
//void CfTextEditorCtrl::OnCaretChange(wxStyledTextEvent& event)
//{
//	int pos = GetCurrentPos();
//	//if(m_iCaretPos == pos) return;
//	//m_iCaretPos = -m_iCaretPos;
//	int col = GetColumn(pos);
//	int sel = GetSelectionEnd() - GetSelectionStart();
//	wxString msg;
//	msg.Printf(_("Ln: %d  Col: %d  Sel: %d"),GetCurrentLine()+1,col+1,sel);
//	CfiiWxStatusBar::Get()->SetStatusText(msg,SB_Field_LnCol);
//	SendUserInputEvent();
//}

//>>

//----------------------------------------------------------------------------
// private functions
wxString CfTextEditorCtrl::DeterminePrefs (const wxString &filename) {

    LanguageInfo const* curInfo;

    // determine language from filepatterns
    int languageNr;
    for (languageNr = 0; languageNr < g_LanguagePrefsSize; languageNr++) {
        curInfo = &g_LanguagePrefs [languageNr];
        wxString filepattern = curInfo->filepattern;
        filepattern.Lower();
        while (!filepattern.empty()) {
            wxString cur = filepattern.BeforeFirst (';');
            if ((cur == filename) ||
                (cur == (filename.BeforeLast ('.') + _T(".*"))) ||
                (cur == (_T("*.") + filename.AfterLast ('.')))) {
                return curInfo->name;
            }
            filepattern = filepattern.AfterFirst (';');
        }
    }
    return wxEmptyString;

}

bool CfTextEditorCtrl::InitializePrefs (const wxString &name) {

    // initialize styles
    StyleClearAll();
    LanguageInfo const* curInfo = NULL;

	wxString type_name = name;
	if(name.IsEmpty()) type_name = _T("BASH");

    // determine language
    bool found = false;
    int languageNr;
    for (languageNr = 0; languageNr < g_LanguagePrefsSize; languageNr++) {
        curInfo = &g_LanguagePrefs [languageNr];
        if (curInfo->name == type_name) {
            found = true;
            break;
        }
    }
    if (!found) return false;

    // set lexer and language
    SetLexer (curInfo->lexer);
    m_language = curInfo;

    // set margin for line numbers
    SetMarginType (m_LineNrID, wxSTC_MARGIN_NUMBER);
    StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColour (_T("DARK GREY")));
    StyleSetBackground (wxSTC_STYLE_LINENUMBER, *wxWHITE);
    //SetMarginWidth (m_LineNrID, 0); // start out not visible

    // default fonts for all styles!
    int Nr;
    for (Nr = 0; Nr < wxSTC_STYLE_LASTPREDEFINED; Nr++) {
        wxFont font (10, wxMODERN, wxNORMAL, wxNORMAL);
        StyleSetFont (Nr, font);
    }

    // set common styles
    StyleSetForeground (wxSTC_STYLE_DEFAULT, wxColour (_T("DARK GREY")));
    StyleSetForeground (wxSTC_STYLE_INDENTGUIDE, wxColour (_T("DARK GREY")));

    // initialize settings
    if (g_EditorCommonPrefs.syntaxEnable) {
        int keywordnr = 0;
        for (Nr = 0; Nr < STYLE_TYPES_COUNT; Nr++) {
            if (curInfo->styles[Nr].type == -1) continue;
            const StyleInfo &curType = g_StylePrefs [curInfo->styles[Nr].type];
            wxFont font (curType.fontsize, wxMODERN, wxNORMAL, wxNORMAL, false,
                         curType.fontname);
            StyleSetFont (Nr, font);
            if (curType.foreground) {
                StyleSetForeground (Nr, wxColour (curType.foreground));
            }
            if (curType.background) {
                StyleSetBackground (Nr, wxColour (curType.background));
            }
            StyleSetBold (Nr, (curType.fontstyle & TOKEN_STYLE_BOLD) > 0);
            StyleSetItalic (Nr, (curType.fontstyle & TOKEN_STYLE_ITALIC) > 0);
            StyleSetUnderline (Nr, (curType.fontstyle & TOKEN_STYLE_UNDERL) > 0);
            StyleSetVisible (Nr, (curType.fontstyle & TOKEN_STYLE_HIDDEN) == 0);
            StyleSetCase (Nr, curType.lettercase);
            const wxChar *pwords = curInfo->styles[Nr].words;
            if (pwords) {
                SetKeyWords (keywordnr, pwords);
                keywordnr += 1;
            }
        }
    }

    // set margin as unused
    SetMarginType (m_DividerID, wxSTC_MARGIN_SYMBOL);
    SetMarginWidth (m_DividerID, 0);
    SetMarginSensitive (m_DividerID, false);

    // folding
    SetMarginType (m_FoldingID, wxSTC_MARGIN_SYMBOL);
    SetMarginMask (m_FoldingID, wxSTC_MASK_FOLDERS);
    StyleSetBackground (m_FoldingID, *wxWHITE);
    SetMarginWidth (m_FoldingID, 0);
    SetMarginSensitive (m_FoldingID, false);
    if (g_EditorCommonPrefs.foldEnable) {
        SetMarginWidth (m_FoldingID, curInfo->folds != 0? m_FoldingMargin: 0);
        SetMarginSensitive (m_FoldingID, curInfo->folds != 0);
        SetProperty (_T("fold"), curInfo->folds != 0? _T("1"): _T("0"));
        SetProperty (_T("fold.comment"),
                     (curInfo->folds & TOKEN_FOLD_COMMENT) > 0? _T("1"): _T("0"));
        SetProperty (_T("fold.compact"),
                     (curInfo->folds & TOKEN_FOLD_COMPACT) > 0? _T("1"): _T("0"));
        SetProperty (_T("fold.preprocessor"),
                     (curInfo->folds & TOKEN_FOLD_PREPROC) > 0? _T("1"): _T("0"));
        //SetProperty (_T("fold.html"),
        //             (curInfo->folds & TOKEN_FOLD_HTML) > 0? _T("1"): _T("0"));
        //SetProperty (_T("fold.html.preprocessor"),
        //             (curInfo->folds & TOKEN_FOLD_HTMLPREP) > 0? _T("1"): _T("0"));
        //SetProperty (_T("fold.comment.python"),
        //             (curInfo->folds & TOKEN_FOLD_COMMENTPY) > 0? _T("1"): _T("0"));
        //SetProperty (_T("fold.quotes.python"),
        //             (curInfo->folds & TOKEN_FOLD_QUOTESPY) > 0? _T("1"): _T("0"));
    }
    SetFoldFlags (wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED |
                  wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

    // set spaces and indention
    SetTabWidth (4);
    SetUseTabs (false);
    SetTabIndents (true);
    SetBackSpaceUnIndents (true);
    SetIndent (g_EditorCommonPrefs.indentEnable? 4: 0);

    // others
	SetMarginWidth (m_LineNrID, g_EditorCommonPrefs.lineNumberEnable? m_LineNrMargin: 0);
    SetViewEOL (g_EditorCommonPrefs.displayEOLEnable);
    SetIndentationGuides (g_EditorCommonPrefs.indentGuideEnable);
    SetEdgeColumn (80);
    SetEdgeMode (g_EditorCommonPrefs.longLineOnEnable? wxSTC_EDGE_LINE: wxSTC_EDGE_NONE);
    SetViewWhiteSpace (g_EditorCommonPrefs.whiteSpaceEnable?
                       wxSTC_WS_VISIBLEALWAYS: wxSTC_WS_INVISIBLE);
    SetOvertype (g_EditorCommonPrefs.overTypeInitial);
    SetReadOnly (g_EditorCommonPrefs.readOnlyInitial);
    SetWrapMode (g_EditorCommonPrefs.wrapModeInitial?
                 wxSTC_WRAP_WORD: wxSTC_WRAP_NONE);

    return true;
}

bool CfTextEditorCtrl::LoadFile ()
{
#if wxUSE_FILEDLG
    //// get filname
    //if (!m_FilePath) {
    //    wxFileDialog dlg (this, _T("Open file"), wxEmptyString, wxEmptyString,
    //                      _T("Any file (*)|*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
    //    if (dlg.ShowModal() != wxID_OK) return false;
    //    m_FilePath = dlg.GetPath();
    //}

    // load file
    return LoadFile (m_FilePath);
#else
    return false;
#endif // wxUSE_FILEDLG
}

bool CfTextEditorCtrl::LoadFile (const wxString &filename) {

	m_bStartAndEndFlag = true;

    // load file in edit and clear undo
    if (!filename.empty()) m_FilePath = filename;
    ClearAll ();
    wxStyledTextCtrl::LoadFile(m_FilePath);
    EmptyUndoBuffer();

    // determine lexer language
    wxFileName fname (m_FilePath);
    InitializePrefs (DeterminePrefs (fname.GetFullName()));
	SetCaretAt(0);

    return true;
}

//bool CfTextEditorCtrl::SaveFile (bool new_file)
bool CfTextEditorCtrl::SaveFile ()
{
#if wxUSE_FILEDLG
    // return if no change
	//    if ((!new_file)&&(!Modified())) return true;
    if (!Modified()) return true;

    // get filname
    if (!m_FilePath) {
        wxFileDialog dlg (this, _T("Save file"), wxEmptyString, wxEmptyString, _T("Any file (*)|*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (dlg.ShowModal() != wxID_OK) return false;
        m_FilePath = dlg.GetPath();
    }

    // save file
    return SaveFile (m_FilePath);
#else
    return false;
#endif // wxUSE_FILEDLG
}

//<<MING_20110426
// Save as...
bool CfTextEditorCtrl::SaveFileAs (const wxString &filename)
{
    // save file
    wxStyledTextCtrl::SaveFile(filename);
	return LoadFile(filename);
}
//>>



bool CfTextEditorCtrl::SaveFile (const wxString &filename) {

    // return if no change
    if (!Modified()) return true;
    return wxStyledTextCtrl::SaveFile(filename);

}

bool CfTextEditorCtrl::Modified () {

    // return modified state
    return (GetModify() && !GetReadOnly());
}


bool CfTextEditorCtrl::CloseEditor()
{
    if (Modified()) 
	{
		wxString msg;
		msg.Printf(wxT("File '%s' has been changed, save the changes?"),m_FilePath.AfterLast('\\'));
		wxMessageDialog dlg(this,msg,PROJECT_CAPTION,wxYES_NO | wxCANCEL | wxYES_DEFAULT | wxICON_QUESTION);
		int id = dlg.ShowModal();
        if (id == wxID_YES) {
            SaveFile();
            if (Modified()) {
				msg.Printf(wxT("File '%s' could not be saved, close it anyway?"),m_FilePath.AfterLast('\\'));
				wxMessageDialog dlg2(this,msg,PROJECT_CAPTION,wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
				id = dlg2.ShowModal();
				if(id == wxNO)  return false;
            }
        }
		else if(id == wxID_CANCEL) return false;
    }
    SetFilename (wxEmptyString);
	m_bStartAndEndFlag = true;
    ClearAll();
    SetSavePoint();
//	CfiiWxStatusBar::Get()->SetStatusText(wxEmptyString,SB_Field_LnCol);

	return true;
}

bool CfTextEditorCtrl::SearchText(int flags,wxString &text, bool search_up)
{
	int found = 0;
	bool all_covered = false;

Do_Search:
	int pos = GetAnchor();
	//pos = GetCurrentPos();
	if(pos > -1) 
	{ 
		SetCurrentPos(pos);
		SearchAnchor();
	}
	if(search_up)
		found = SearchPrev(flags,text);
	else
		found = SearchNext(flags,text);
	if(found > -1)
	{	
		if(search_up) SetSelection(found,found+text.Length());  // to set the anchor in front of the word.
		EnsureCaretVisible();
	}
	else
	{
		if(!all_covered)
		{
			if(search_up) DocumentEnd();
			else DocumentStart();
			all_covered = true;
			goto Do_Search;
		}
	}

	return (found > 0);
}

void CfTextEditorCtrl::SetCaretAt(long pos)
{
	SetCurrentPos(pos);
	SetSelectionStart(pos);
	SetSelectionEnd(pos);
	EnsureCaretVisible();
}

//CfFileEditorBase
bool CfTextEditorCtrl::OpenFile(const wxString &file_path)
{
	return this->LoadFile(file_path);
}

void CfTextEditorCtrl::ProcessEvent(wxCommandEvent &event)
{

}
cfReaderID CfTextEditorCtrl::GetReaderId() const
{
	return FR_TEXT_EDITOR;
}
//----------------------------------------------------------------------------
// EditProperties
//----------------------------------------------------------------------------

EditProperties::EditProperties (CfTextEditorCtrl *edit,
                                long style)
        : wxDialog (edit, wxID_ANY, wxEmptyString,
                    wxDefaultPosition, wxDefaultSize,
                    style | wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {

    // sets the application title
    SetTitle (_("Properties"));
    wxString text;

    // fullname
    wxBoxSizer *fullname = new wxBoxSizer (wxHORIZONTAL);
    fullname->Add (10, 0);
    fullname->Add (new wxStaticText (this, wxID_ANY, _("Full filename"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
    fullname->Add (new wxStaticText (this, wxID_ANY, edit->GetFilename()),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);

    // text info
    wxGridSizer *textinfo = new wxGridSizer (4, 0, 2);
    textinfo->Add (new wxStaticText (this, wxID_ANY, _("Language"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    textinfo->Add (new wxStaticText (this, wxID_ANY, edit->m_language->name),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);
    textinfo->Add (new wxStaticText (this, wxID_ANY, _("Lexer-ID: "),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    text = wxString::Format (_T("%d"), edit->GetLexer());
    textinfo->Add (new wxStaticText (this, wxID_ANY, text),
                   0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);
    wxString EOLtype = wxEmptyString;
    switch (edit->GetEOLMode()) {
        case wxSTC_EOL_CR: {EOLtype = _T("CR (Unix)"); break; }
        case wxSTC_EOL_CRLF: {EOLtype = _T("CRLF (Windows)"); break; }
        case wxSTC_EOL_LF: {EOLtype = _T("CR (Macintosh)"); break; }
    }
    textinfo->Add (new wxStaticText (this, wxID_ANY, _("Line endings"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    textinfo->Add (new wxStaticText (this, wxID_ANY, EOLtype),
                   0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);

    // text info box
    wxStaticBoxSizer *textinfos = new wxStaticBoxSizer (
                     new wxStaticBox (this, wxID_ANY, _("Informations")),
                     wxVERTICAL);
    textinfos->Add (textinfo, 0, wxEXPAND);
    textinfos->Add (0, 6);

    // statistic
    wxGridSizer *statistic = new wxGridSizer (4, 0, 2);
    statistic->Add (new wxStaticText (this, wxID_ANY, _("Total lines"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                    0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    text = wxString::Format (_T("%d"), edit->GetLineCount());
    statistic->Add (new wxStaticText (this, wxID_ANY, text),
                    0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);
    statistic->Add (new wxStaticText (this, wxID_ANY, _("Total chars"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                    0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    text = wxString::Format (_T("%d"), edit->GetTextLength());
    statistic->Add (new wxStaticText (this, wxID_ANY, text),
                    0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);
    statistic->Add (new wxStaticText (this, wxID_ANY, _("Current line"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                    0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    text = wxString::Format (_T("%d"), edit->GetCurrentLine());
    statistic->Add (new wxStaticText (this, wxID_ANY, text),
                    0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);
    statistic->Add (new wxStaticText (this, wxID_ANY, _("Current pos"),
                                     wxDefaultPosition, wxSize(80, wxDefaultCoord)),
                    0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT, 4);
    text = wxString::Format (_T("%d"), edit->GetCurrentPos());
    statistic->Add (new wxStaticText (this, wxID_ANY, text),
                    0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxRIGHT, 4);

    // char/line statistics
    wxStaticBoxSizer *statistics = new wxStaticBoxSizer (
                     new wxStaticBox (this, wxID_ANY, _("Statistics")),
                     wxVERTICAL);
    statistics->Add (statistic, 0, wxEXPAND);
    statistics->Add (0, 6);

    // total pane
    wxBoxSizer *totalpane = new wxBoxSizer (wxVERTICAL);
    totalpane->Add (fullname, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    totalpane->Add (0, 6);
    totalpane->Add (textinfos, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    totalpane->Add (0, 10);
    totalpane->Add (statistics, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    totalpane->Add (0, 6);
    wxButton *okButton = new wxButton (this, wxID_OK, _("OK"));
    okButton->SetDefault();
    totalpane->Add (okButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit (totalpane);

    ShowModal();
}

#if wxUSE_PRINTING_ARCHITECTURE

//----------------------------------------------------------------------------
// EditPrint
//----------------------------------------------------------------------------

EditPrint::EditPrint (CfTextEditorCtrl *edit, wxChar *title)
              : wxPrintout(title) {
    m_edit = edit;
    m_printed = 0;
wxLogDebug(_("EditPrint"));
}

bool EditPrint::OnPrintPage (int page) {
wxLogDebug(_("OnPrintPage"));
    wxDC *dc = GetDC();
    if (!dc) return false;

    // scale DC
    PrintScaling (dc);

    // print page
    if (page == 1) m_printed = 0;
    m_printed = m_edit->FormatRange (1, m_printed, m_edit->GetLength(),
                                     dc, dc, m_printRect, m_pageRect);

    return true;
}

bool EditPrint::OnBeginDocument (int startPage, int endPage) {

    if (!wxPrintout::OnBeginDocument (startPage, endPage)) {
        return false;
    }
wxLogDebug(_("OnBeginDoc"));
    return true;
}

void EditPrint::GetPageInfo (int *minPage, int *maxPage, int *selPageFrom, int *selPageTo) {

	wxLogDebug(_("GetPageInfo 1"));
    // initialize values
    *minPage = 0;
    *maxPage = 0;
    *selPageFrom = 0;
    *selPageTo = 0;

    // scale DC if possible
    wxDC *dc = GetDC();
    if (!dc) return;
    PrintScaling (dc);

    // get print page informations and convert to printer pixels
    wxSize ppiScr;
    GetPPIScreen (&ppiScr.x, &ppiScr.y);
    wxSize page = g_pageSetupData->GetPaperSize();
    page.x = static_cast<int> (page.x * ppiScr.x / 25.4);
    page.y = static_cast<int> (page.y * ppiScr.y / 25.4);
    m_pageRect = wxRect (0,
                         0,
                         page.x,
                         page.y);

    // get margins informations and convert to printer pixels
    wxPoint pt = g_pageSetupData->GetMarginTopLeft();
    int left = pt.x;
    int top = pt.y;
    pt = g_pageSetupData->GetMarginBottomRight();
    int right = pt.x;
    int bottom = pt.y;

    top = static_cast<int> (top * ppiScr.y / 25.4);
    bottom = static_cast<int> (bottom * ppiScr.y / 25.4);
    left = static_cast<int> (left * ppiScr.x / 25.4);
    right = static_cast<int> (right * ppiScr.x / 25.4);

    m_printRect = wxRect (left,
                          top,
                          page.x - (left + right),
                          page.y - (top + bottom));

	int len = m_edit->GetLength();
	len = len;
    // count pages
    while (HasPage (*maxPage)) {
        m_printed = m_edit->FormatRange (0, m_printed, m_edit->GetLength(),
                                       dc, dc, m_printRect, m_pageRect);
        *maxPage += 1;
    }
    if (*maxPage > 0) *minPage = 1;
    *selPageFrom = *minPage;
    *selPageTo = *maxPage;
	wxLogDebug(_("GetPageInfo 2"));
}

bool EditPrint::HasPage (int WXUNUSED(page)) {
	wxLogDebug(_("HasPage"));
    return (m_printed < m_edit->GetLength());
}

bool EditPrint::PrintScaling (wxDC *dc){
	wxLogDebug(_("PrintScaling 1"));
    // check for dc, return if none
    if (!dc) return false;

    // get printer and screen sizing values
    wxSize ppiScr;
    GetPPIScreen (&ppiScr.x, &ppiScr.y);
    if (ppiScr.x == 0) { // most possible guess 96 dpi
        ppiScr.x = 96;
        ppiScr.y = 96;
    }
    wxSize ppiPrt;
    GetPPIPrinter (&ppiPrt.x, &ppiPrt.y);
    if (ppiPrt.x == 0) { // scaling factor to 1
        ppiPrt.x = ppiScr.x;
        ppiPrt.y = ppiScr.y;
    }
    wxSize dcSize = dc->GetSize();
    wxSize pageSize;
    GetPageSizePixels (&pageSize.x, &pageSize.y);

    // set user scale
    float scale_x = (float)(ppiPrt.x * dcSize.x) /
                    (float)(ppiScr.x * pageSize.x);
    float scale_y = (float)(ppiPrt.y * dcSize.y) /
                    (float)(ppiScr.y * pageSize.y);
    //dc->SetLibraryScale (scale_x, scale_y);
    dc->SetUserScale (scale_x, scale_y);

	wxLogDebug(_("PrintScaling 2"));
    return true;
}

#endif // wxUSE_PRINTING_ARCHITECTURE
