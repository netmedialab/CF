#include "CfDefines.h"
#include "Instance.h"
#include "Instance.h"
#include "DrawingElementFactory.h"
#include "EnvironmentUtil.h"
 //CfInstance
CfInstance::CfInstance(wxEvtHandler *p_event_handler)
:CfSymbol(p_event_handler)
{
}
CfInstance::CfInstance(wxEvtHandler *p_event_handler,const wxString &id, CfSymbol *symbol)
	:CfSymbol(id,id,*symbol)
	,m_sSymbolId(symbol->GetId())
{
	CfDrawingElement::SetLineColor(CfEnvironmentUtil::Get()->GetString(PROP_SETTING_SYMBOL_DEFAULTLINECOLOR,_T("Black")));
	CfDrawingElement::SetLineWidth(wxAtoi(CfEnvironmentUtil::Get()->GetString(PROP_SETTING_SYMBOL_DEFAULTLINEWIDTH,_T("6"))));
	CfDrawingElement::SetFilledColor(CfEnvironmentUtil::Get()->GetString(PROP_SETTING_SYMBOL_DEFAULTFILLEDCOLOR,_T("None")));
}

CfInstance::CfInstance(const wxString &id, const wxString &name, CfInstance& instance)
	:CfSymbol(instance.m_pEventHandler,id,name,new CfRectangle((CfRectangle*) instance.m_pShape))
{
	CfDrawingElementGroup *elements = instance.GetElementArray();
	for(size_t i=0;i<elements->Count();i++)
	{
		m_ElementArray.Add(DoCopyDrawingElement((*elements)[i]));
	}

}
CfInstance::~CfInstance()
{
	//for(size_t i=0; i<m_ElementArray.Count();i++)
	//{
	//	delete m_ElementArray[i];
	//}
}

void CfInstance::Copy(CfDrawingElement* element,bool change_name)
{
	CfDrawingElement::Copy(element,change_name);
	this->m_ElementArray.Copy(((CfInstance*)element)->GetElementArray());
}

//IProperty
void CfInstance::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_INSTANCE);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[%s]\n"),cfSTRING_COMMON);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_ID,m_sId);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_NAME,m_sName);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_SYMBOL,m_sSymbolId);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_COMMON);
	fos->Write(line.char_str(),line.size());
	
	//Dump parameters

	//Dump shape
	m_pShape->Dump(fos,offset);

	//Dump elements
	line = wxString::Format(_("[%s]\n"),cfSTRING_ELEMENTS);
	fos->Write(line.char_str(),line.size());
	for(size_t i=0; i<m_ElementArray.Count();i++)
	{
		m_ElementArray[i]->Dump(fos,offset);
	}
	line = wxString::Format(_("[/%s]\n"),cfSTRING_ELEMENTS);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_INSTANCE);
	fos->Write(line.char_str(),line.size());

	this->SetModified(false);
}
void CfInstance::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line = tis.ReadLine().Trim().Trim(true);
	if(line == wxString::Format(_("[%s]"),cfSTRING_SYMBOL))
			line = tis.ReadLine().Trim().Trim(true);

	if(line == wxString::Format(_("[%s]"),cfSTRING_COMMON))
	{
		while (fis->CanRead())
		{
			line = tis.ReadLine().Trim().Trim(true);
			if(line == wxString::Format(_("[/%s]"),cfSTRING_COMMON))
				break;
			name = line.Before('=');
			value = line.After('=');
			if(name == cfSTRING_ID) m_sId = value;
			else if(name == cfSTRING_NAME) m_sName = value;
			else if(name == cfSTRING_SYMBOL) m_sSymbolId = value;
		}
    }

	//Load parameters

	//Load shape
	m_pShape = new CfRectangle();
	m_pShape->Load(fis,offset);

	//Load elements
	CfDrawingElement *element=NULL;
	line = tis.ReadLine().Trim().Trim(true);
	if(line == wxString::Format(_("[%s]"),cfSTRING_ELEMENTS))
	{ 
		m_ElementArray.Load(m_pEventHandler,fis,offset);
	}

	
}

