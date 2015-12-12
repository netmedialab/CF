/////////////////////////////////////////////////////////////////////////////////
//
// Name        : CfFileReaderManager.cpp
// Author      : MING
// Created     : 2015Äê2ÔÂ4ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////
#include <wx/filename.h>
#include "FileReaderManager.h"
#include "FileUtil.h"
#include "TextEditorCtrl.h"
#include "ImageEditorCtrl.h"
#include "ChipSchematicEditorCtrl.h"
#include "ChipSymbolEditorCtrl.h"

CfFileReaderManager::CfFileReaderManager(void)
{
}

CfFileReaderManager::~CfFileReaderManager(void)
{
	//Delete the external reader. Internal reader is released by FileNoteboolCtrl
	FileReaderList::iterator iter;
	for(iter = m_ReaderList.begin();iter!=m_ReaderList.end();iter++)
	{
		CfFileEditorBase *pReader = *iter;
		if(pReader && (pReader->GetReaderId() == FR_EXTERNAL_READER))
		{
			delete (CfExternalFileReader*)pReader;
		}
	}
}

CfFileEditorBase *CfFileReaderManager::CreateFileReader(wxWindow* parent,const wxString &file_path,CfProject* project)
{

	CfFileEditorBase *p_freader = NULL;
	wxFileName fn(file_path);
	if(!fn.Exists()) return p_freader;

	wxString ext = fn.GetExt();
	if(ext == F_EXT_CHIP_IMAGE)
	{
		fn.SetExt(F_EXT_CHIP_SCHEMATIC);
		p_freader = new CfImageEditorCtrl(file_path,fn.GetFullPath(),parent);
	}
	else if(ext == F_EXT_CHIP_SCHEMATIC)
	{
		p_freader = new CfChipSchematicEditorCtrl(file_path,parent,project);
	}
	else if(ext == F_EXT_CHIP_SYMBOL)
	{
		p_freader = new CfChipSymbolEditorCtrl(file_path,parent,project);
	}
	else
		p_freader = new CfTextEditorCtrl(file_path,parent);

	//p_freader->SetProject(project);

	//Check if the file already has been opened
	int idx = FindExistReader(p_freader);
	if(idx == wxNOT_FOUND)
	{
		if(p_freader->LoadFile())
			m_ReaderList.Append(p_freader);
		else
		{
			wxLogError("Failed to open file - "+file_path);
			wxLogError(p_freader->GetMessage());
			delete p_freader;
			p_freader = NULL;
		}
	}
	else
	{
		delete p_freader;
		p_freader = m_ReaderList[idx];
	}
	//CfFileEditorBase *p_freader = new CfExternalFileReader();
	return p_freader;
}


int CfFileReaderManager::FindExistReader(const CfFileEditorBase *reader)
{
	for(size_t i=0;i < m_ReaderList.size();i++)
	{
		wxLogDebug(m_ReaderList[i]->GetFilePath());
		if(*m_ReaderList[i] == *reader)
		return i;
	}
	return wxNOT_FOUND;
}

int CfFileReaderManager::RemoveReaderFromList(const CfFileEditorBase *reader)	//Do not delete object.
{
	bool result = false;
	//int idx = FindExistReader((CfTextEditorCtrl*)reader);
	int idx = FindExistReader(reader);
	wxFileReaderListNode *node = m_ReaderList.Find(reader);
	if(node)
		result = m_ReaderList.DeleteNode(node);
	return result;

}
