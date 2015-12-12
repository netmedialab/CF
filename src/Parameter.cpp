#include "Parameter.h"
#include "CfDefines.h"
#include "CfLogMessageCtrl.h"


CfParameter::CfParameter(void)
{
}


CfParameter::~CfParameter(void)
{
	for(size_t i=0;i<m_ParameterItemPtrArray.Count();i++)
	{
		delete m_ParameterItemPtrArray[i];
	}
	m_ParameterItemPtrArray.Clear();
}

void CfParameter::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_PARAMETER);
	fos->Write(line.char_str(),line.size());
	for(size_t i = 0; i < m_ParameterItemPtrArray.Count(); i++)
	{
		line = wxString::Format(_("[%s]\n"),cfSTRING_PARAMETER_ITEM);
		fos->Write(line.char_str(),line.size());
		line = wxString::Format(_("%s=%s\n"),cfSTRING_PARAMETER_NAME, m_ParameterItemPtrArray[i]->Name);
		fos->Write(line.char_str(),line.size());
		line = wxString::Format(_("%s=%s\n"),cfSTRING_PARAMETER_VALUE, m_ParameterItemPtrArray[i]->Value);
		fos->Write(line.char_str(),line.size());
		line = wxString::Format(_("%s=%s\n"),cfSTRING_PARAMETER_TYPE, m_ParameterItemPtrArray[i]->Type);
		fos->Write(line.char_str(),line.size());
		line = wxString::Format(_("[/%s]\n"),cfSTRING_PARAMETER_ITEM);
		fos->Write(line.char_str(),line.size());
	}
	line = wxString::Format(_("[/%s]\n"),cfSTRING_PARAMETER);
	fos->Write(line.char_str(),line.size());
}

wxString CfParameter::GetParamNames()
{
	wxString names;
	for(size_t i = 0; i < m_ParameterItemPtrArray.Count(); i++)
	{
		names += m_ParameterItemPtrArray[i]->Name + ";";
	}
	return names;
}
void CfParameter::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line;
	ParameterItem *item;
	{
		while (fis->CanRead())
		{
			line = tis.ReadLine().Trim().Trim(true);
			if(line == wxString::Format(_("[/%s]"),cfSTRING_PARAMETER))
				break;
			else if(line == wxString::Format(_("[%s]"),cfSTRING_PARAMETER))
			{
			}
			else if(line == wxString::Format(_("[%s]"),cfSTRING_PARAMETER_ITEM))
				item = new ParameterItem();
			else if(line == wxString::Format(_("[/%s]"),cfSTRING_PARAMETER_ITEM))
				m_ParameterItemPtrArray.Add(item);
			else
			{
				name = line.Before('=');
				value = line.After('=');
				if(name == cfSTRING_PARAMETER_NAME) item->Name = value;
				else if(name == cfSTRING_PARAMETER_VALUE) item->Value = value;
				else if(name == cfSTRING_PARAMETER_TYPE) item->Type = value;
				else
				{
					wxString msg = wxString::Format(_("Loading failed: Unrecognized line %s in %s"), 
						line,typeid(this).name());
					wxLogMessage(msg);
					break;
				}
			}
		}
    }
}