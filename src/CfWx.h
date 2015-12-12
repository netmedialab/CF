/////////////////////////////////////////////////////////////////////////////////
//
// Name        :Wx.h
// Author      :MING
// Created     :2010-02-04 ¿ÀÈÄ 2:02:59
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef FIIWX_H_
#define FIIWX_H_

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

#include <wx/combo.h>
#include <wx/aui/aui.h>
#include <wx/artprov.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/file.h>

//#include "wx/imaglist.h"
//#include "wx/artprov.h"

//#if !defined(new) && defined(WXDEBUG_NEW) && wxUSE_MEMORY_TRACING && wxUSE_GLOBAL_MEMORY_OPERATORS
//#define new WXDEBUG_NEW
//#endif

#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

#endif