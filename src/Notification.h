/////////////////////////////////////////////////////////////////////////////////
//
// Name        : Notification.h
// Author      : MING
// Created     : 2015Äê1ÔÂ31ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __CF_NOTIFICATION_H__
#define __CF_NOTIFICATION_H__

// Notification overview
// ---------------------

// To inform the application about what's happening, the engine sends
// some notifications to the application using events.
// For example the event table entry could look like this:
//     EVT_FZ_NOTIFICATION(wxID_ANY, CMainFrame::OnEngineEvent)
// and the handler function has the following declaration:
//     void OnEngineEvent(wxEvent& event);
// You can get the engine which sent the event by calling
//     event.GetEventObject()
// Whenever you get a notification event,
// CFileZillaEngine::GetNextNotification has to be called until it returns 0,
// or you will lose important notifications or your memory will fill with
// pending notifications.

// A special class of notifications are the asynchronous requests. These 
// requests have to be answered. Once proessed, call 
// CFileZillaEngine::SetAsyncRequestReply to continue the current operation.

#include "CfWx.h"
#include "IElement.h"
extern const wxEventType cfEVT_NOTIFICATION;
#define EVT_CF_NOTIFICATION(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        cfEVT_NOTIFICATION, id, -1, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

class wxCfEvent : public wxEvent
{
public:
	wxCfEvent(int id = wxID_ANY);
	virtual wxEvent *Clone() const;
};

enum CfNotificationId
{
	//File
	cfId_OpenFile			// Open a file
	,cfId_UpdateWinTitle	// update title of the window 
	,cfId_RefreshWorkspace	// reload workspace
	,cfId_AddFile			// add a file to project, and show it in workspace explorer
	
	//Image editor
	,cfId_ImageEditor_Refresh

	//schematic editor
	,cfId_Editor_Modified

	//property view
	,cfId_PropertyView_ShowElement

	//Layer view
	,cfId_LayerView_Refresh
	,cfId_LayerView_Update

	//Drawing element
	,cfId_DrawingElement_Change
	,cfId_Schematic_ElementChange	//Element move, create, change size, change property
	,cfId_Schematic_LayerChange		//editor activated, highlight element,new layer

	//Status bar
	,cfId_StatusBar_Update


};

// Async request IDs
enum RequestId
{
	reqId_fileexists,		// Target file already exists, awaiting further instructions
	reqId_interactiveLogin, // gives a challenge prompt for a password
	reqId_hostkey,			// used only by SSH/SFTP to indicate new host key
	reqId_hostkeyChanged,	// used only by SSH/SFTP to indicate changed host key
	reqId_certificate		// sent after a successful TLS/SSL handshake to allow certificate
							// validation
};

class CfNotification : public wxObject
{
public:
	CfNotification(CfNotificationId id);
	~CfNotification();
	enum CfNotificationId GetID() const;
	void SetString(const wxString &string);
	void SetData(void *data);
	void SetElement(IElement *element);
	wxString GetString() const;
	void *GetData() const;
	IElement *GetElement() const;
private:
	CfNotificationId m_id;
	wxString m_notifyString;
	void * m_notifyData;
	IElement * m_pElement;

};

class CfNotificationSender
{
public:
	CfNotificationSender(wxEvtHandler *p_event_handler);
	~CfNotificationSender();
	void AddNotification(CfNotification *pNotification);
protected:
	wxEvtHandler *m_pEventHandler;
};

#endif
