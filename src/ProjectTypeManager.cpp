/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfProjectTypeManager.cpp
// Author      :James
// Created     :2010-06-11 ¿ÀÈÄ 6:08:46
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#include <wx/stdpaths.h>
#include <wx/textfile.h>
#include <wx/filename.h>
#include "CfDefines.h"
#include "CfWx.h"
#include "ProjectTypeManager.h"
#include "CfUtils.h"

CfProjectTypeManager::CfProjectTypeManager()
:m_bIsChanged(false),m_pEnvironment(CfEnvironmentUtil::Get())
{
	LoadTypeFromEnvir();
}

CfProjectTypeManager::~CfProjectTypeManager()
{
}

void CfProjectTypeManager::SaveChanges()
{
	DumpTypeToEnvir();
}

wxArrayString* CfProjectTypeManager::GetTypeList()
{
	return &m_lTypeStringList;
}

wxArrayString* CfProjectTypeManager::GetTypeDescriptionList()
{
	return &m_lDescritionStringList;
}

void CfProjectTypeManager::GetTypeTemplateList(int index,wxArrayString* list)
{
	if(index >= m_lTemplateStringList.GetCount()) return;
	TrunckStringToList(m_lTemplateStringList[index].c_str(),list,_(","));
}


void CfProjectTypeManager::DelTypeAt(int index)
{
	m_lTypeStringList.RemoveAt(index);
	m_lDescritionStringList.RemoveAt(index);
	m_lTemplateStringList.RemoveAt(index);
	m_bIsChanged = true;
}

bool CfProjectTypeManager::AddClassifier(int index, const wxChar* classi_name)
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

bool CfProjectTypeManager::DelClassifier(int index, const wxChar* classi_name)
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

bool CfProjectTypeManager::AddType(const wxChar* type_name,const wxChar* descrip)
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

void CfProjectTypeManager::LoadTypeFromEnvir()
{
	m_pEnvironment->GetStringList(PROP_PROJTYPE,&m_lTypeStringList);
	// get description
	m_pEnvironment->GetStringList(PROP_PROJTYPE_DESCRIPT,&m_lDescritionStringList);
	// get template
	m_pEnvironment->GetStringList(PROP_PROJTYPE_TEMPLATE,&m_lTemplateStringList);
}

void CfProjectTypeManager::DumpTypeToEnvir()
{
	if(!m_bIsChanged) return;

	m_pEnvironment->RemoveList(PROP_PROJTYPE);
	m_pEnvironment->RemoveList(PROP_PROJTYPE_DESCRIPT);
	m_pEnvironment->RemoveList(PROP_PROJTYPE_TEMPLATE);
	// skip the predefined the tool while writing to registry
	for(int i = 0; i<m_lTypeStringList.GetCount(); i++)
	{
		m_pEnvironment->AddtoStringList(PROP_PROJTYPE,m_lTypeStringList[i]);
		// save description
		m_pEnvironment->AddtoStringList(PROP_PROJTYPE_DESCRIPT,m_lDescritionStringList[i]);
		// save template
		m_pEnvironment->AddtoStringList(PROP_PROJTYPE_TEMPLATE,m_lTemplateStringList[i]);
	}

}

