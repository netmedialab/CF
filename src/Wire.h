#pragma once
#include "DrawingElement.h"
#include "wx/grid.h"

class CfWire : public CfDrawingElement
{
public:
	CfWire(wxEvtHandler *p_event_handler);
	CfWire(wxEvtHandler *p_event_handler,const wxString &id, const wxString &name, CfLineSegment *prect);
	CfWire(const wxString &id, const wxString &name, CfWire& wire);
	~CfWire();
	int GetTypeId()const {return ID_ELEMENT_WIRE;}

	//IProperty
	void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication =false);
	void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties);
	virtual void SetProperty(wxPGProperty *pgproperty);
	virtual void GetProperty(wxPropertyGridInterface *propertygrid);
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset=wxPoint(0,0));
	virtual void Load(wxFileInputStream *fis, const wxPoint offset=wxPoint(0,0));

	void Merge(CfShape *shape);
	//void AddShape(CfShape *shape);
//private:
	//int m_iWidth;
};