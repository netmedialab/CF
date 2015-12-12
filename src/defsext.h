/////////////////////////////////////////////////////////////////////////////
// Name:        defsext.h extensions
// Purpose:     STC test declarations
// Maintainer:  Wyo
// Created:     2003-09-01
// RCS-ID:      $Id: defsext.h 35523 2005-09-16 18:25:44Z ABX $
// Copyright:   (c) wxGuide
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DEFSEXT_H_
#define _WX_DEFSEXT_H_

//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

//! wxWidgets headers
#include "wx/print.h"    // printing support
#include "wx/printdlg.h" // printing dialog

#include "CfDefines.h"


//============================================================================
// declarations
//============================================================================

#define DEFAULT_LANGUAGE _T("<default>")

#define PAGE_COMMON _("Common")
#define PAGE_LANGUAGES _("Languages")
#define PAGE_STYLE_TYPES _("Style types")

#define STYLE_TYPES_COUNT 32


// ----------------------------------------------------------------------------
// global items
// ----------------------------------------------------------------------------

//! global application name
extern wxString *g_appname;

#if wxUSE_PRINTING_ARCHITECTURE

//! global print data, to remember settings during the session
extern wxPrintData *g_printData;
extern wxPageSetupData *g_pageSetupData;

#endif // wxUSE_PRINTING_ARCHITECTURE

#endif // _WX_DEFSEXT_H_
