#pragma once
#include "CfWx.h"
#include "CfDefines.h"
#include "FileEditorBase.h"
#include "Image.h"
#include "PropertyGridCtrl.h"
#include "Notification.h"
#include "wx/overlay.h"

class CfImageEditorCtrl:
	public wxScrolledWindow, public CfFileEditorBase, public CfNotificationSender
{
public:
    CfImageEditorCtrl(const wxString &image_path,const wxString &schematic_path, wxWindow *parent, 
		  wxWindowID id = wxID_ANY,
          const wxPoint &pos = wxDefaultPosition,
          const wxSize &size = wxDefaultSize);
	~CfImageEditorCtrl(void);

	//FileEditorBase
	virtual bool CloseEditor(){ return true; };
	virtual void Activated();
	virtual bool LoadFile();
	virtual bool SaveFile () {return true;};
	virtual bool Modified() {return false;};

	void ProcessEvent(wxCommandEvent &event);
	cfReaderID GetReaderId() const;
	void OnChosePort();
	void OnChoseWire();
	//static CfDrawingElement *m_pDrawingElement;
private:
	CfImage m_Image;
	void OnMouseEvent(wxMouseEvent& event);
	void OnPaint( wxPaintEvent &WXUNUSED(event) );

	CfScreenView m_View;
	
	DECLARE_EVENT_TABLE()
};

