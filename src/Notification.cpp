#include "Notification.h"
#include "CfLogMessageCtrl.h"

const wxEventType cfEVT_NOTIFICATION = wxNewEventType();

wxCfEvent::wxCfEvent(int id /*=wxID_ANY*/) : wxEvent(id, cfEVT_NOTIFICATION)
{
}

wxEvent *wxCfEvent::Clone() const
{
	return new wxCfEvent(*this);
}

CfNotification::CfNotification(CfNotificationId id)
	:m_id(id)
	,m_notifyData(NULL)
{
}

CfNotification::~CfNotification()
{
}

enum CfNotificationId CfNotification::GetID() const
{
	return m_id;
}

void CfNotification::SetString(const wxString &string)
{
	m_notifyString = string;
}
void CfNotification::SetData(void *data)
{
	m_notifyData = data;
}

void CfNotification::SetElement(IElement *element)
{
	m_pElement= element;
}

wxString CfNotification::GetString() const
{
	return m_notifyString;
}
void *CfNotification::GetData() const
{
	return m_notifyData;
}
IElement *CfNotification::GetElement() const
{
	return m_pElement;
}

// CfNotificationSender
CfNotificationSender::CfNotificationSender(wxEvtHandler *p_event_handler)
:m_pEventHandler(p_event_handler)
{
}
CfNotificationSender::~CfNotificationSender()
{
}
void CfNotificationSender::AddNotification(CfNotification *pNotification)
{
	if(!m_pEventHandler) return;
	wxCfEvent evt(wxID_ANY);
	evt.SetEventObject(pNotification);
	wxPostEvent(m_pEventHandler, evt);
}