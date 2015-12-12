/////////////////////////////////////////////////////////////////////////////////
//
// Name        : FileReaderManager.h
// Author      : MING
// Created     : 2015Äê2ÔÂ4ÈÕ
// Description : This is to manage the file readers,including internal and 
//				 external readers. The internal readers will be added to /and 
//				 released by file notebook. The external readers should managed
//				 by CfFileReaderManager
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <wx/vector.h>
#include "FileEditorBase.h"
#include "FileReaderManager.h"
#include "Project.h"

class CfFileReaderManager
{
public:
	CfFileReaderManager(void);
	~CfFileReaderManager(void);
	int RemoveReaderFromList(const CfFileEditorBase *reader);	//Do not delete object.
protected:
	CfFileEditorBase *CreateFileReader(wxWindow* parent,const wxString &file_path,CfProject *project);

private:
	FileReaderList m_ReaderList;
	CfFileEditorBase *DoCreateFileReader(const wxString &file_ext);

	int FindExistReader(const CfFileEditorBase *reader);
};

