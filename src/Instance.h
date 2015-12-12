#pragma once
#include "Symbol.h"
#include <wx/hashmap.h>

class CfInstance :
	public CfSymbol
{
public:
	CfInstance(wxEvtHandler *p_event_handler);
	CfInstance(wxEvtHandler *p_event_handler,const wxString &id, CfSymbol *symbol);
	CfInstance(const wxString &id, const wxString &name, CfInstance& instance);
	~CfInstance();

	virtual int GetTypeId()const {return ID_ELEMENT_INSTANCE;}
	virtual void Copy(CfDrawingElement* element, bool change_name=true);

	//IProperty
	void GetProperty(wxPropertyGridInterface *pg);
	void SetProperty(wxPGProperty *pgproperty);
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset=wxPoint(0,0));
	virtual void Load(wxFileInputStream *fis, const wxPoint offset=wxPoint(0,0));

private:
	wxString m_sSymbolId;
	wxString m_sLibraryName;
	wxString m_sFilePath;

};

