#pragma once
#include "DrawingElement.h"
#include "DrawingElementGroup.h"
#include <wx/hashmap.h>


class CfLayer 
	:public CfDrawingElementWithChildren 
	,public CfDrawingElement
{
public:
	CfLayer(wxEvtHandler *p_event_handler);
	CfLayer(wxEvtHandler *p_event_handler,const wxString &id, const wxString &name);
	CfLayer(const wxString &id, const wxString &name, CfLayer& layer);
	~CfLayer();

	void SetBackground(CfDrawingElement* element);
	CfDrawingElement* GetBackground() const;
	void ShowBackground(bool show=true);
	void ShowElements(bool show=true);
	bool IsElementShowing();

	void DrawBackground(wxDC& dc,const CfScreenView &view);

	void Draw(wxDC& dc,const CfScreenView &view);
	int GetTypeId()const {return ID_ELEMENT_LAYER;}
	virtual void Move(int dx, int dy);
	virtual void Copy(CfDrawingElement* element, bool change_name=true);
	virtual bool Modified();
	virtual void SetShowing(bool showing);
	virtual bool IsShowing();
	virtual wxPoint GetEndPos();

	//IProperty
	void GetProperty(wxPropertyGridInterface *pg);
	void SetProperty(wxPGProperty *pgproperty);
	virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication =false);
	virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties);
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset=wxPoint(0,0));
	virtual void Load(wxFileInputStream *fis, const wxPoint offset=wxPoint(0,0));

	//IElementWithChildren
private:
	CfDrawingElement * m_pBackground;
	CfHashMap m_ParameterMap;

};

