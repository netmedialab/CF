#pragma once
#include "Shape.h"
#include "Notification.h"
#include "IProperty.h"
#include <map>
#include <iostream>
using namespace std;
WX_DECLARE_STRING_HASH_MAP( wxString, CfHashMap);
enum
{
	ID_ELEMENT_IMAGE = 0
	,ID_ELEMENT_PORT
	,ID_ELEMENT_WIRE
	,ID_ELEMENT_SYMBOL
	,ID_ELEMENT_LAYER
	,ID_ELEMENT_INSTANCE
	,ID_ELEMENT_SOLIDCOLORBACKGROUND
	,ID_ELEMENT_COUNT
};

class CfDrawingElement: public IProperty, public IDrawable, public CfNotificationSender, public IElement
{
public:
	CfDrawingElement(wxEvtHandler *p_event_handler);
	CfDrawingElement(wxEvtHandler *p_event_handler,const wxString& id, const wxString &name, CfShape *shape);
	~CfDrawingElement(void);
	wxString GetName() const;
	void SetName(wxString name);
	wxString GetId() const;
	//CfShapeArray* GetShapeArray();
	CfShape* GetShape();
	static CfDrawingElement* LoadFromFile(wxEvtHandler *p_event_handler,wxFileInputStream *fis, const wxPoint offset=wxPoint(0,0));

	virtual int GetTypeId() const = 0;
	//virtual void AddShape(CfShape *shape);
	
	void SetModified(bool modified = true);
	virtual bool Modified();

	//IDrawalble
	virtual void Draw(wxDC& dc,const CfScreenView &view);
	virtual bool Contains(wxPoint &point);
	virtual bool Within(wxRect &rect);
	virtual bool IsEndPoint(wxPoint &point);
	//virtual bool Merge(CfShape* shape);
	virtual wxPoint GetStartPos();
	virtual wxPoint GetEndPos();
	virtual wxSize GetSize();
	bool IsDeleted();
	bool IsHighlighted();

	//action
	virtual void Move(int dx, int dy);
	virtual void Copy(CfDrawingElement* element, bool change_name=true);
	void SetDeleted(bool del);
	void SetHighlighted(bool highlight=true);

	//property version2
	virtual void GetProperty(wxPropertyGridInterface *propertygrid);
	virtual void SetProperty(wxPGProperty *pgproperty);
	//IProperty
	//virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication =false){};
	//virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties)=0;

	bool operator == (const CfDrawingElement& element)
	{
		return ((element.GetTypeId()==this->GetTypeId()) && (element.GetId()==this->GetId()));
	}
	static wxString ElementTypeArray[];

	void SetLineColor(wxColor lcolor);
	void SetLineWidth(int width);
	void SetFilledColor(wxColor fcolor);
	wxColor GetLineColor();
	int GetLineWidth();
	wxColor GetFilledColor();
protected:
	wxString m_sId;
	wxString m_sName;
	//CfShapeArray m_pShape;
	CfShape* m_pShape;
	bool m_bModified;
	bool m_bDeleted;
	bool m_bHighlighted;
};


