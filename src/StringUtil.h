#pragma once
#include "CfWx.h"
#include "CfDefines.h"

enum
{
	ID_COLOR_BLACK = 0
	,ID_COLOR_WHITE
	,ID_COLOR_YELLOW
	,ID_COLOR_BLUE
	,ID_COLOR_HIGHLIGHT
	,ID_COLOR_COUNT
};

static const wxColor cfColorList[] = {*wxBLACK,*wxWHITE,*wxYELLOW,*wxBLUE,*wxRED};
static const wxString cfColorStringList[] = {_("Black"),_("White"),_("Yellow"),_("Blue"),_("Highlight"),_("None")};
static const wxChar* cfPortStringList[] = {_("Input"),_("Output"),_("Inout"),NULL};
static const wxChar* cfBackgroundTypeList[] = {cfSTRING_IMAGE,cfSTRING_COLOR,cfSTRING_NONE,NULL};
wxColor GetColorFromStr(wxString& str);