#pragma once
#include "DrawingElement.h"
#include "DrawingElementGroup.h"
#include "Parameter.h"
#include <wx/hashmap.h>

class CfSymbol :
	public CfDrawingElementWithChildren
	,public CfDrawingElement
	//, public IElementWithChildren
{
public:
	CfSymbol(wxEvtHandler *p_event_handler);
	CfSymbol(wxEvtHandler *p_event_handler,const wxString &id, const wxString &name, CfRectangle *prect);
	CfSymbol(const wxString &id, const wxString &name, CfSymbol& symbol);
	~CfSymbol();

	void Draw(wxDC& dc,const CfScreenView &view);
	virtual int GetTypeId()const {return ID_ELEMENT_SYMBOL;}
	virtual void Move(int dx, int dy);
	virtual void Copy(CfDrawingElement* element, bool change_name=true);
	virtual bool Modified();
	virtual bool Contains(wxPoint &point);

	//IProperty
	void GetProperty(wxPropertyGridInterface *pg);
	void SetProperty(wxPGProperty *pgproperty);
	//virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication =false);
	//virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties);
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset=wxPoint(0,0));
	virtual void Load(wxFileInputStream *fis, const wxPoint offset=wxPoint(0,0));

	//Symbol
	CfDrawingElement* GetElement(wxPoint &point);
	CfDrawingElement* Find(CfDrawingElement *element);

	//CfDrawingElementGroup* GetElementArray(){ return &m_ElementArray; };
	//CfDrawingElementGroup* GetHighlightElements() { return &m_HightlightElements; };
protected:
	CfParameter m_Parameter;
	//CfDrawingElementGroup m_ElementArray;
	//CfDrawingElementGroup m_HightlightElements;

};

