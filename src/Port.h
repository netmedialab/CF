#pragma once
#include "DrawingElement.h"
#include "PropertyGridCtrl.h"
#include <map>
#include <iostream>
using namespace std;

class CfPort : public CfDrawingElement
{
public:
	CfPort(wxEvtHandler *p_event_handler);
	CfPort(wxEvtHandler *p_event_handler,const wxString &id, const wxString &name, CfPortShape *prect);
	CfPort(const wxString &id, const wxString &name, const CfPort& port);
	int GetTypeId()const {return ID_ELEMENT_PORT;}
	~CfPort();
	virtual void Draw(wxDC& dc,const CfScreenView &view);
	void ChangeDirection();
	static const int INPUT = 0;
	static const int OUTPUT = 1;
	static const int INOUT = 2;
	wxString GetDirection() const;
	//int GetWidth();

	//property version2
	virtual void GetProperty(wxPropertyGridInterface *propertygrid);
	virtual void SetProperty(wxPGProperty *pgproperty);
	//IProperty
	virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication =false);
	virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties);
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset=wxPoint(0,0));
	virtual void Load(wxFileInputStream *fis, const wxPoint offset=wxPoint(0,0));
private:
	wxString m_sDirection;
	//int m_iWidth;
	bool m_bDirectionChanged;
	//'Parameter' (Name(string), Value(Integer, string or Equation with global parameters, Only integer equation is allowed.)
};