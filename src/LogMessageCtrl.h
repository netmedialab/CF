#pragma once

#include "CfWx.h"
#include <stdarg.h>

#undef wxLogMessage
#undef wxLogError
#undef wxLogWarning
#define wxLogMessage(x,...)	CfLogMessageCtrl::Get()->AddMessage(0,x,__VA_ARGS__)
#define wxLogError(x,...)	CfLogMessageCtrl::Get()->AddMessage(1,x,__VA_ARGS__)
#define wxLogWarning(x,...)	CfLogMessageCtrl::Get()->AddMessage(2,x,__VA_ARGS__)
#ifdef _DEBUG
	#undef wxLogDebug
	#define wxLogDebug(x,...) CfLogMessageCtrl::Get()->AddMessage(3,x,__VA_ARGS__)
#else
	#define wxLogDebug(x,...) 
#endif


const static wxChar* LogMode[4] = {_(""),_("[Error] "),_("[Warning] "),_("[Debug] ")};

class CfLogMessageCtrl : public wxTextCtrl
{
public:
	CfLogMessageCtrl(wxWindow* parent, wxWindowID id);
	~CfLogMessageCtrl(void);

	static CfLogMessageCtrl* Get();
	static void Init(wxWindow* parent, wxWindowID id);
	static void Destroy();

	void AddMessage(const int mode,const wxChar *pszFormat, ...); // Mode0: message, Mode1:Error, Mode2:Warning
private:
	static CfLogMessageCtrl *m_pLogMessage;
};
