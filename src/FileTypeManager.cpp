
/////////////////////////////////////////////////////////////////////////////////
//
// Name        : FileTypeManager.cpp
// Author      : MING
// Created     : 2015Äê9ÔÂ12ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////

#include <wx/stdpaths.h>
#include <wx/textfile.h>
#include <wx/filename.h>
#include "CfDefines.h"
#include "CfWx.h"
#include "FileTypeManager.h"
#include "CfUtils.h"

CfFileTypeManager::CfFileTypeManager()
:m_bIsChanged(false),m_pEnvironment(CfEnvironmentUtil::Get())
{
	LoadTypeFromEnvir();
}

CfFileTypeManager::~CfFileTypeManager()
{
}

void CfFileTypeManager::SaveChanges()
{
	DumpTypeToEnvir();
}

wxArrayString* CfFileTypeManager::GetTypeList()
{
	return &m_lTypeStringList;
}

wxArrayString* CfFileTypeManager::GetTypeDescriptionList()
{
	return &m_lDescritionStringList;
}

void CfFileTypeManager::GetTypeTemplateList(int index,wxArrayString* list)
{
	if(index >= m_lTemplateStringList.GetCount()) return;
	TrunckStringToList(m_lTemplateStringList[index].c_str(),list,_(","));
}


void CfFileTypeManager::DelTypeAt(int index)
{
	m_lTypeStringList.RemoveAt(index);
	m_lDescritionStringList.RemoveAt(index);
	m_lTemplateStringList.RemoveAt(index);
	m_bIsChanged = true;
}

bool CfFileTypeManager::AddClassifier(int index, const wxChar* classi_name)
{
	wxArrayString list;
	//wxString name(classi_name);
	TrunckStringToList(m_lTemplateStringList[index].c_str(),&list,_(","));
	if(list.Index(classi_name) < 0)
	{
		m_lTemplateStringList[index] << classi_name << _(",");
		m_bIsChanged = true;
		return true;
	}
	return false;

}

bool CfFileTypeManager::DelClassifier(int index, const wxChar* classi_name)
{
	wxArrayString list;
	wxString str;
	TrunckStringToList(m_lTemplateStringList[index].c_str(),&list,_(","));
	int pos = list.Index(classi_name);
	if(pos!= -1)
	{
		list.RemoveAt(pos);
		ConcateListToString(&list,str,_(","));
		m_lTemplateStringList[index] = str;		
		m_bIsChanged = true;
		return true;
	}
	return false;

}

bool CfFileTypeManager::AddType(const wxChar* type_name,const wxChar* descrip)
{
	int n = m_lTypeStringList.Index(type_name);
	if(n==wxNOT_FOUND)
	{
		n = m_lTypeStringList.Add(type_name);
		m_lDescritionStringList.Add(descrip);
		m_lTemplateStringList.Add(wxEmptyString);
		m_bIsChanged = true;
		return true;
	}
	return false;
}

void CfFileTypeManager::LoadTypeFromEnvir()
{
	m_pEnvironment->GetStringList(PROP_FILETYPE,&m_lTypeStringList);
	// get description				   
	m_pEnvironment->GetStringList(PROP_FILETYPE_DESCRIPT,&m_lDescritionStringList);
	// get template					   
	m_pEnvironment->GetStringList(PROP_FILETYPE_TEMPLATE,&m_lTemplateStringList);
}

void CfFileTypeManager::DumpTypeToEnvir()
{
	if(!m_bIsChanged) return;

	m_pEnvironment->RemoveList(PROP_PROJTYPE);
	m_pEnvironment->RemoveList(PROP_PROJTYPE_DESCRIPT);
	m_pEnvironment->RemoveList(PROP_PROJTYPE_TEMPLATE);
	// skip the predefined the tool while writing to registry
	for(int i = 0; i<m_lTypeStringList.GetCount(); i++)
	{
		m_pEnvironment->AddtoStringList(PROP_FILETYPE,m_lTypeStringList[i]);
		// save description
		m_pEnvironment->AddtoStringList(PROP_FILETYPE_DESCRIPT,m_lDescritionStringList[i]);
		// save template
		m_pEnvironment->AddtoStringList(PROP_FILETYPE_TEMPLATE,m_lTemplateStringList[i]);
	}

}

