#pragma once
#include "CfWx.h"
#include <wx/wfstream.h>
#include <wx/filename.h>
#include <wx/txtstrm.h>
#include "wx/propgrid/propgridiface.h"

class PropertyUnit
{
public:
	wxString Name;
	wxString Value;
	wxString ValueType;	//wxGRID_VALUE_STRING, wxGRID_VALUE_BOOL, wxGRID_VALUE_NUMBER, 
					//wxGRID_VALUE_FLOAT and wxGRID_VALUE_CHOICE
	bool ReadOnly;	//the property is readonly
	const wxString *Choice;
	int	Size;		//size of choice
	PropertyUnit(wxString name,wxString value,wxString type,bool read_only = false,const wxString *choice=NULL, int size = 0):
	Name(name),Value(value),ValueType(type),ReadOnly(read_only),Choice(choice),Size(size){};

};


WX_DEFINE_ARRAY_PTR(PropertyUnit *, PropertyUnitPtrArray);

class IProperty
{
public:
	IProperty(void){};
	virtual ~IProperty(void){};
	//virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication =false)=0;
	//virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties)=0;
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset=wxPoint(0,0))=0;
	virtual void Load(wxFileInputStream *fis, const wxPoint offset=wxPoint(0,0))=0;
	//property version2
	virtual void GetProperty(wxPropertyGridInterface *propertygrid)=0;
	virtual void SetProperty(wxPGProperty *pgproperty)=0;

	bool AddProperty(PropertyUnitPtrArray *properties,PropertyUnit* unit, bool check_name_duplication =false)
	{
		if(check_name_duplication)
		{
			for(size_t i=0;i<properties->Count();i++)
			{
				if((*properties)[i]->Name == unit->Name) return false;
			}
		}
		properties->Add(unit);
		return true;
	};
};

