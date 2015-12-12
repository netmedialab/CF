/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfProjectTypeManager.h
// Author      :James
// Created     :2010-06-11 ¿ÀÈÄ 6:08:46
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////


#ifndef PROJECT_TYPE_MAN_H
#define PROJECT_TYPE_MAN_H

#include "CfDefines.h"
#include "EnvironmentUtil.h"


class CfProjectTypeManager
{
public:
	CfProjectTypeManager();
	~CfProjectTypeManager(void);

	void SaveChanges();
	wxArrayString* GetTypeList();
	wxArrayString* GetTypeDescriptionList();
	void GetTypeTemplateList(int index,wxArrayString* list);
	bool AddType(const wxChar* type_name,const wxChar* descrip);
	bool AddClassifier(int index,const wxChar* classi_name);
	bool DelClassifier(int index,const wxChar* classi_name);
	void DelTypeAt(int index);
private:
	
	void LoadTypeFromEnvir();
	void DumpTypeToEnvir();

	CfEnvironmentUtil *m_pEnvironment;
	wxArrayString m_lTypeStringList;
	wxArrayString m_lDescritionStringList;
	wxArrayString m_lTemplateStringList;
	bool m_bIsChanged;
};

#endif
