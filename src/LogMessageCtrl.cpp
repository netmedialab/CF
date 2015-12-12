#include "LogMessageCtrl.h"


CfLogMessageCtrl* CfLogMessageCtrl::m_pLogMessage = 0;


CfLogMessageCtrl* CfLogMessageCtrl::Get()
{
	return m_pLogMessage;

}
void CfLogMessageCtrl::Init(wxWindow* parent, wxWindowID id)
{
	if (!m_pLogMessage)
		m_pLogMessage = new CfLogMessageCtrl(parent, id);	

}

void CfLogMessageCtrl::Destroy()
{
	if (!m_pLogMessage) return; 
	delete m_pLogMessage;
	m_pLogMessage = 0;

}

CfLogMessageCtrl::CfLogMessageCtrl(wxWindow* parent, wxWindowID id)
:wxTextCtrl(parent,id,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE)
{
}

CfLogMessageCtrl::~CfLogMessageCtrl(void)
{
}

void CfLogMessageCtrl::AddMessage(const int mode,const wxChar *pszFormat, ...)
{
	va_list argptr;
    va_start(argptr, pszFormat);

	wxChar sprint_buf[1024];
	int n = _vstprintf(sprint_buf, pszFormat, argptr);
    va_end(argptr);	
	AppendText(LogMode[mode]);
	AppendText(sprint_buf);
	AppendText(_("\n"));
}
