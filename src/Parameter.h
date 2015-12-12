#pragma once
#include "CfWx.h"
#include "IProperty.h"

class ParameterItem
{
public:
	wxString Name;
	wxString Value;
	wxString Type;
};

WX_DEFINE_ARRAY_PTR(ParameterItem*,ParameterItemPtrArray);
class CfParameter :
	public IProperty
{
public:
	CfParameter(void);
	~CfParameter(void);

	ParameterItemPtrArray *GetParameterList(){return &m_ParameterItemPtrArray;};
	wxString GetParamNames();

	//IPorperty
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset=wxPoint(0,0));
	virtual void Load(wxFileInputStream *fis, const wxPoint offset=wxPoint(0,0));
	virtual void GetProperty(wxPropertyGridInterface *propertygrid);
	virtual void SetProperty(wxPGProperty *pgproperty);

private:
	ParameterItemPtrArray m_ParameterItemPtrArray;
};


