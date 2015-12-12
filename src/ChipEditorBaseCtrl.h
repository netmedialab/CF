#pragma once
#include "wx/overlay.h"
#include "CfWx.h"
#include "CfDefines.h"
#include "FileEditorBase.h"
#include "DrawingElement.h"
#include "Panel.h"
#include "PropertyGridCtrl.h"
#include "Notification.h"
#include "History.h"

#define SCROLL_RATE 10		//1 pixel as scrolling increment

class CfChipEditorBaseCtrl:
	public wxScrolledWindow, public CfFileEditorBase, public CfNotificationSender
{
public:
    CfChipEditorBaseCtrl(const wxString &file_path,
		wxWindow *parent, 
		CfProject* project,
		  wxWindowID id = wxID_ANY,
          const wxPoint &pos = wxDefaultPosition,
          const wxSize &size = wxDefaultSize);
	~CfChipEditorBaseCtrl(void);
	enum
	{
		DRAWING_ELEMENT_NONE = 0,
		DRAWING_ELEMENT_DRAG,
		DRAWING_ELEMENT_PORT,
		DRAWING_ELEMENT_WIRE,	
		DRAWING_ELEMENT_SYMBOL,
		DRAWING_ELEMENT_SELECT
	};

	virtual cfReaderID GetReaderId() const = 0;
	virtual void RefreshEditor() {this->Refresh();};
	virtual void AddElement(CfDrawingElement *element) = 0;
	void AddActionToHistory(IAction *action, bool run_now,bool refresh=true);
	void HighlightElement(CfDrawingElement* element);

	//edit
    virtual void OnEditRedo (wxCommandEvent &event);
    virtual void OnEditUndo (wxCommandEvent &event);
    virtual void OnEditClear (wxCommandEvent &event);
    virtual void OnEditCut (wxCommandEvent &event);
    virtual void OnEditCopy (wxCommandEvent &event);
    virtual void OnEditPaste (wxCommandEvent &event);
    virtual void OnEditSelectAll (wxCommandEvent &event);
    virtual void OnEditDelete (wxCommandEvent &event);

	CfScreenView m_View;
protected:	
	void SendNotification(CfNotificationId notify_id, IElement *element = NULL, void* data = NULL);
	static CfDrawingElementGroup ms_Clipboard;
	CfProject *m_pProject;
	CfHistory m_History;
	IElementWithChildren* m_pElementWithChildren;

	int m_DrawingElementID;
	CfDrawingElement *m_pDrawingElement;
	//for select
	wxPoint m_SelectStartPos;
	bool m_bSelected;
	double m_ScaleFactor;

};

