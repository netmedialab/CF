/////////////////////////////////////////////////////////////////////////////////
//
// Name        :Environment.cpp
// Author      :MING
// Created     :2010-01-28 ¿ÀÈÄ 8:11:51
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#include "CfWx.h"
#include "CfDefines.h"
#include "EnvironmentUtil.h"
#include "FileUtil.h"

CfEnvironmentUtil* CfEnvironmentUtil::m_pEnvironment = 0;
void CfEnvironmentUtil::Init()
{
	if (!m_pEnvironment)
	{
		wxString path = FileUtil::Get()->GetDirectory(ID_APPDIR)+_("\\")+PROJECT_CAPTION+_(".ini");
		//m_pEnvironment = new CfEnvironmentUtil(PROJECT_CAPTION);	
		if(!wxFileName::Exists(path))
		{
			wxFileOutputStream fos(path);
			fos.Close();
		}
		m_pEnvironment = new CfEnvironmentUtil(path);	
	}
}
void CfEnvironmentUtil::Destroy()
{
	if (!m_pEnvironment) return; 
	delete m_pEnvironment;
	m_pEnvironment = 0;
}

CfEnvironmentUtil* CfEnvironmentUtil::Get()
{
	if (!m_pEnvironment) Init();
	return m_pEnvironment;
}

//CfEnvironmentUtil::CfEnvironmentUtil(const wxChar* appName)
//CfEnvironmentUtil::CfEnvironmentUtil(wxFileInputStream* is)
CfEnvironmentUtil::CfEnvironmentUtil(const wxString& file_path)
	:wxFileConfig(wxFileInputStream(file_path))
	,m_sConfigFilePath(file_path)
//,wxConfig(appName)
,m_bModified(false)
{	
	//LoadRecentProjects();
	//LoadUrlList();
}

////CfEnvironmentUtil::CfEnvironmentUtil(const wxChar* appName)
//CfEnvironmentUtil::CfEnvironmentUtil(wxString &file_name)
//:wxFileConfig(file_name)
////,wxConfig(appName)
//,m_bModified(false)
//{	
//	//LoadRecentProjects();
//	//LoadUrlList();
//}

CfEnvironmentUtil::~CfEnvironmentUtil()
{
	
	wxFileOutputStream fos(m_sConfigFilePath);
	this->Save(fos);
	fos.Close();
	//DumpRecentProjects();
	//DumpUrlList();
	
	//CFileUtils file_utils;
	//wxFileOutputStream fs(file_utils.GetDirectory(ID_CONFDIR)+wxT("environment2.ini"));
	//bool done = Save(fs);
	//fs.Close();
}

bool CfEnvironmentUtil::IsModified()
{
	return m_bModified;

}

int CfEnvironmentUtil::GetInt(const wxChar* key, int defualt)
{
	wxString value;
	long lv;
	Read(key,&value);
	if(!value.IsEmpty() && (value.ToLong(&lv)))
		return (int)lv;
	else return defualt;

}

bool CfEnvironmentUtil::GetBool(const wxChar* key, bool defualt)
{
	return (GetInt(key,defualt) != 0);
}

wxString CfEnvironmentUtil::GetString(const wxChar* key, const wxChar* default)
{
	wxString value;
	Read(key,&value,default);
	return value;
}

void CfEnvironmentUtil::GetStringList(const wxChar* key, wxArrayString* string_list)
{
	wxString item;
	wxString num_key,item_key;
	num_key.Printf(wxT("%sNum"),key);

	int count = GetInt(num_key.c_str(),0);

	string_list->Alloc(string_list->GetCount() + count);

	for(size_t i=0;i<count;i++)
	{
		item_key.Printf(wxT("%s%d"),key,i);
		if(Read(item_key,&item))
		{
			//if(!item.IsEmpty())
				string_list->Add(item);
			//else
			//	break;
		}
		else
			break;
	}
}

void CfEnvironmentUtil::SetStringList(const wxChar* key, const wxArrayString* string_list)
{
	RemoveList(key);
	AppendStringList(key,string_list);
}


void CfEnvironmentUtil::AppendStringList(const wxChar* key, const wxArrayString* string_list)
{
	wxString item;
	wxString num_key,item_key;
	num_key.Printf(wxT("%sNum"),key);

	int count = GetInt(num_key.c_str(),0);

	for(size_t i=0;i<string_list->GetCount();i++)
	{
		item_key.Printf(wxT("%s%d"),key,i+count);
		SetString(item_key,string_list->Item(i));
	}
	
	SetInt(num_key,string_list->GetCount() + count);

}

void CfEnvironmentUtil::AddtoStringList(const wxChar* key, const wxChar* item)
{
	wxString num_key,item_key;
	num_key.Printf(wxT("%sNum"),key);

	int count = GetInt(num_key.c_str(),0);

	item_key.Printf(wxT("%s%d"),key,count);

	Write(item_key,item);			// write string item
	SetInt(num_key.c_str(),count+1);// count++

	m_bModified = true;
}

void CfEnvironmentUtil::SetNotModified()
{
	m_bModified = false;
}

void CfEnvironmentUtil::Remove(const wxChar* key)
{
	DeleteEntry(key);

}

void CfEnvironmentUtil::RemoveList(const wxChar* key)
{
	wxString item;
	wxString num_key,item_key;
	num_key.Printf(wxT("%sNum"),key);

	int count = GetInt(num_key.c_str(),0);
	for(size_t i=0;i<count;i++)
	{
		item_key.Printf(wxT("%s%d"),key,i);
		//DeleteEntry(item_key,&item);
		DeleteEntry(item_key);
	}

	SetInt(num_key,0);
}

void CfEnvironmentUtil::RemoveByPrefix(const wxChar* prefix)
{

}
void CfEnvironmentUtil::SetString(const wxChar* key, const wxChar* value)
{
	Write(key,value);
	m_bModified = true;
}
void CfEnvironmentUtil::SetInt(const wxChar* key, int value)
{
	wxString s;
	s << value;
	Write(key,s);
}
void CfEnvironmentUtil::SetBool(const wxChar* key, bool value)	
{
	wxString s;
	s << value;
	Write(key,s);
}

//wxArrayString* CfEnvironmentUtil::GetRecentProjects()
//{
//	return &m_lRecentProjects;
//}
//
//void CfEnvironmentUtil::AddToRecentProjects(const wxChar* proj_name)
//{
//	wxString name(proj_name);
//	int index = m_lRecentProjects.Index(name);
//	if(index != wxNOT_FOUND)
//	{
//		m_lRecentProjects.RemoveAt(index);
//	}
//	else if(m_lRecentProjects.Count() == MAX_RECENT_PROJECT)
//	{
//		m_lRecentProjects.RemoveAt(MAX_RECENT_PROJECT-1);
//	}
//	m_lRecentProjects.Insert(name,0);
//}
//
//void CfEnvironmentUtil::DumpRecentProjects()
//{
//	wxString key;
//	SetInt(PROP_RECENT_PROJECT_NUM,m_lRecentProjects.Count());	
//	for(size_t i=0;i<m_lRecentProjects.Count();i++)
//	{
//		key.Printf(wxT("%s%d"),PROP_RECENT_PROJECT,i);
//		Write(key,m_lRecentProjects[i]);
////		wxMessageBox(m_lRecentProjects[i], key,wxOK, this);
//	}
//}
//
//void CfEnvironmentUtil::LoadRecentProjects()
//{
//	wxString key;
//	wxString proj_name;
//	long index = 0;
//
//	int count = GetInt(PROP_RECENT_PROJECT_NUM,MAX_RECENT_PROJECT);
//
//	m_lRecentProjects.Alloc(count);
//
//	//int cnt = m_pEnvironment->GetNumberOfEntries();
//
//	//m_pConfig->GetFirstEntry(proj_name,index);
//
//	for(size_t i=0;i<count;i++)
//	{
//		key.Printf(wxT("%s%d"),PROP_RECENT_PROJECT,i);
//		if(Read(key,&proj_name))
//		{
//			if(!proj_name.IsEmpty())
//				m_lRecentProjects.Add(proj_name);
//			else
//				break;
//			//wxMessageBox(proj_name,key, wxOK, this);
//		}
//		else
//			break;
//	}
//}
//
