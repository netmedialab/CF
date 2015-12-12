//////////////////////////////////////////////////////////////////////////////
// File:        prefs.h
// Purpose:     STC test Preferences initialization
// Maintainer:  Wyo
// Created:     2003-09-01
// RCS-ID:      $Id: prefs.h 35523 2005-09-16 18:25:44Z ABX $
// Copyright:   (c) wxGuide
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef _PREFS_H_
#define _PREFS_H_

//----------------------------------------------------------------------------
// informations
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

//! wxWidgets headers

//! wxWidgets/contrib headers
#include "wx/stc/stc.h"  // styled text control
#include "defsext.h"

//! application headers


//============================================================================
// declarations
//============================================================================

//! general style types
#define TOKEN_TYPE_DEFAULT 0

#define TOKEN_TYPE_WORD1 1
#define TOKEN_TYPE_WORD2 2
#define TOKEN_TYPE_WORD3 3
#define TOKEN_TYPE_WORD4 4
#define TOKEN_TYPE_WORD5 5
#define TOKEN_TYPE_WORD6 6

#define TOKEN_TYPE_COMMENT 7
#define TOKEN_TYPE_COMMENT_DOC 8
#define TOKEN_TYPE_COMMENT_LINE 9
#define TOKEN_TYPE_COMMENT_SPECIAL 10

#define TOKEN_TYPE_CHARACTER 11
#define TOKEN_TYPE_CHARACTER_EOL 12
#define TOKEN_TYPE_STRING 13
#define TOKEN_TYPE_STRING_EOL 14

#define TOKEN_TYPE_DELIMITER 15

#define TOKEN_TYPE_PUNCTUATION 16

#define TOKEN_TYPE_OPERATOR 17

#define TOKEN_TYPE_BRACE 18

#define TOKEN_TYPE_COMMAND 19
#define TOKEN_TYPE_IDENTIFIER 20
#define TOKEN_TYPE_LABEL 21
#define TOKEN_TYPE_NUMBER 22
#define TOKEN_TYPE_PARAMETER 23
#define TOKEN_TYPE_REGEX 24
#define TOKEN_TYPE_UUID 25
#define TOKEN_TYPE_VALUE 26

#define TOKEN_TYPE_PREPROCESSOR 27
#define TOKEN_TYPE_SCRIPT 28

#define TOKEN_TYPE_ERROR 29

//----------------------------------------------------------------------------
//! style bits types
#define TOKEN_STYLE_BOLD 1
#define TOKEN_STYLE_ITALIC 2
#define TOKEN_STYLE_UNDERL 4
#define TOKEN_STYLE_HIDDEN 8

//----------------------------------------------------------------------------
//! general folding types
#define TOKEN_FOLD_COMMENT 1
#define TOKEN_FOLD_COMPACT 2
#define TOKEN_FOLD_PREPROC 4

#define TOKEN_FOLD_HTML 16
#define TOKEN_FOLD_HTMLPREP 32

#define TOKEN_FOLD_COMMENTPY 64
#define TOKEN_FOLD_QUOTESPY 128

//----------------------------------------------------------------------------
//! flags
#define TOKEN_FLAG_WRAPMODE 16

//----------------------------------------------------------------------------
// CommonInfo

struct CommonInfo {
    // editor functionality prefs
    bool syntaxEnable;
    bool foldEnable;
    bool indentEnable;
    // display defaults prefs
    bool readOnlyInitial;
    bool overTypeInitial;
    bool wrapModeInitial;
    bool displayEOLEnable;
    bool indentGuideEnable;
    bool lineNumberEnable;
    bool longLineOnEnable;
    bool whiteSpaceEnable;
};
extern CommonInfo g_EditorCommonPrefs;

//----------------------------------------------------------------------------
// LanguageInfo

struct LanguageInfo {
    wxChar *name;
    wxChar *filepattern;
    int lexer;
    struct {
        int type;
        const wxChar *words;
    } styles [STYLE_TYPES_COUNT];
    int folds;
};

extern const LanguageInfo g_LanguagePrefs[];
extern const int g_LanguagePrefsSize;

//----------------------------------------------------------------------------
// StyleInfo
struct StyleInfo {
    wxChar *name;
    wxChar *foreground;
    wxChar *background;
    wxChar *fontname;
    int fontsize;
    int fontstyle;
    int lettercase;
};

extern const StyleInfo g_StylePrefs[];
extern const int g_StylePrefsSize;

#endif // _PREFS_H_
