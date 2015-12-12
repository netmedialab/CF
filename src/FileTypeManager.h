
/////////////////////////////////////////////////////////////////////////////////
//
// Name        : FileTypeManager.h
// Author      : MING
// Created     : 2015Äê9ÔÂ12ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef FILE_TYPE_MAN_H
#define FILE_TYPE_MAN_H

#include "CfDefines.h"
#include "EnvironmentUtil.h"


class CfFileTypeManager
{
public:
	CfFileTypeManager();
	~CfFileTypeManager(void);

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
