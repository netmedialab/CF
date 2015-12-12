#pragma once
#include "wx/overlay.h"
#include "CfWx.h"
#include "CfDefines.h"
#include "FileEditorBase.h"
#include "ChipEditorBaseCtrl.h"
#include "DrawingElement.h"
#include "Symbol.h"
#include "PropertyGridCtrl.h"
#include "Notification.h"
#include "History.h"

//#define SCROLL_RATE 1		//1 pixel as scrolling increment

class CfChipSymbolEditorCtrl:
	//public wxScrolledWindow, public CfFileEditorBase, public CfNotificationSender//, public CfPanel
	public CfChipEditorBaseCtrl
{
public:
    CfChipSymbolEditorCtrl(const wxString &file_path,
		wxWindow *parent, 
		CfProject* project,
		  wxWindowID id = wxID_ANY,
          const wxPoint &pos = wxDefaultPosition,
          const wxSize &size = wxDefaultSize);
	~CfChipSymbolEditorCtrl(void);

	//FileEditorBase
	virtual bool CloseEditor();
	virtual void Activated();
	virtual bool LoadFile();
	virtual bool SaveFile();
	virtual bool Modified();

	void ProcessEvent(wxCommandEvent &event);
	cfReaderID GetReaderId() const;
	void NewSymbol();
	void ChosePort();
	void ChoseWire();
	void ChoseSymbol();
	void SaveSymbol();
	void OnDrawingElementEvent(wxEvent& event);
	//static CfDrawingElement *m_pDrawingElement;
private:
	void OnMouseEvent(wxMouseEvent& event);
	void OnPaint( wxPaintEvent &WXUNUSED(event) );
	void OnSize(wxSizeEvent& event);
	void OnPressKey(wxKeyEvent& event);

	//draw
	void OnNewSymbol(wxCommandEvent &event);
	void OnAddSymbol(wxCommandEvent &event);
	void OnDrawPort(wxCommandEvent &event);
	void OnDrawWire(wxCommandEvent &event);
	void OnDrawSymbol(wxCommandEvent &event);

	////edit
 //   void OnEditRedo (wxCommandEvent &event);
 //   void OnEditUndo (wxCommandEvent &event);
 //   void OnEditClear (wxCommandEvent &event);
 //   void OnEditCut (wxCommandEvent &event);
 //   void OnEditCopy (wxCommandEvent &event);
 //   void OnEditPaste (wxCommandEvent &event);
 //   void OnEditSelectAll (wxCommandEvent &event);
 //   void OnEditDelete (wxCommandEvent &event);

	void OnContextMenu(wxContextMenuEvent &event);
	void OnRedoUpdateUI(wxUpdateUIEvent &event);
	void OnUndoUpdateUI(wxUpdateUIEvent &event);

    
	//void DrawElement(IDrawable *element);
	
	//CfScreenView m_View;
	wxOverlay m_overlay;	//for cover the old shape
	//int m_ElementUsing;

	wxPoint m_DrawStartPos;
	//wxPoint m_WireEndPos;
	wxPoint m_DragStartPos;	
	
	//CfPanel m_Panel;
	//CfLayer *m_pActiveLayer;
	CfSymbol *m_pSymbol;
	
	enum
	{
		DRAWING_ELEMENT_NONE = 0,
		DRAWING_ELEMENT_DRAG,
		DRAWING_ELEMENT_PORT,
		DRAWING_ELEMENT_WIRE,	
		DRAWING_ELEMENT_SYMBOL,
		DRAWING_ELEMENT_SELECT
	};
	DECLARE_EVENT_TABLE()

private:	
	static CfDrawingElementGroup ms_Clipboard;
	//the follow fuction is work for mouse event
	void MouseLDown_DrawingElement(wxPoint pt);
	void MouseDragging_DrawingElement(wxPoint pt);
	void MouseLUp_DrawingElement(wxPoint pt);
	//void HighlightElement(CfDrawingElement* element);
	CfDrawingElement* CreateDrawingElement(int type_id, CfShape *shape,const wxString& path);
	CfDrawingElement* CopyDrawingElement(CfDrawingElement *source_element); 
	wxPoint AddjustMousePos(wxPoint pos, bool alignment = true);			//Addjust the mouse postion to the absolute one, and it can perform the auto alignment
	wxPoint GetScrollOffset();
	//void AddActionToHistory(IAction *action, bool run_now,bool refresh=true);
	void UpdateLayerView();
	void UpdateScrollBar();
	void AddElement(CfDrawingElement *element);
	void Draw(wxDC& dc,const CfScreenView &view,CfDrawingElementGroup* element_group,CfDrawingElementGroup* highlight_array);


};

