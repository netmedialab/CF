#pragma once
#include "IDrawable.h"
#include "DrawingElement.h"
#include "DrawingElementGroup.h"
#include "Layer.h"
#include "Project.h"
#include "Notification.h"
#include "Grid.h"

class CfPanel :
	public CfNotificationSender, public IElementWithChildren, public IElement
{
public:
	CfPanel(wxEvtHandler *p_event_handler);
	~CfPanel(void);
	CfLayer *GetActiveLayer() const;
	void SetActiveLayer(CfLayer* layer);
	void Draw(wxDC& dc,const CfScreenView &view);
	CfDrawingElement* GetElement(wxPoint &point,double scale);
	void AddLayer(CfLayer* layer);
	bool DelElement(CfDrawingElement* element);
	//void Dump();
	//void Load();
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset=wxPoint(0,0));
	virtual void Load(wxEvtHandler *p_event_handler,wxFileInputStream *fis, const wxPoint offset=wxPoint(0,0));

	void SetChanged(bool change = true);
	bool IsChanged();

	wxSize GetSize(); //the size of chip image
	wxPoint GetEndPos(); //the largest end position of elements

	CfDrawingElement* GetShowingBackground();
	void ToggleGrid();

	//IElementWithChildren
	CfDrawingElementGroup* GetElementArray();
	//CfDrawingElementGroup* GetHighlightElements();
	CfDrawingElement* FindElement(CfDrawingElement *element) const;
	virtual CfDrawingElement* FindElement(wxString &element_id) const;
	void AddElement(CfDrawingElement* element);

private:
	CfGrid m_Grid;
	bool m_bChanged;
	CfDrawingElementGroup m_ElementArray;
	//CfDrawingElementWithChildren *m_pActiveElement;
	CfLayer *m_pActiveElement;


};

