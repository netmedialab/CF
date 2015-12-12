#ifndef _CF_FILE_OPENER_H_
#define _CF_FILE_OPENER_H_
#include <list>
#include "CfWx.h"
#include <wx/list.h>
#include <wx/listimpl.cpp>
#include "Project.h"
#include "HighlightElementList.h"
const enum cfReaderID
{
	FR_TEXT_EDITOR
	,FR_IMAGE_EDITOR
	,FR_CHIP_SCHEMATIC_EDITOR
	,FR_CHIP_SYMBOL_EDITOR
	,FR_EXTERNAL_READER
};

class CfFileEditorBase : public CfHighlightElementList
{
public:
	CfFileEditorBase(){};
	CfFileEditorBase(const wxChar* file_path):m_FilePath(file_path){};
	virtual ~CfFileEditorBase() {};
	//virtual bool LoadFile(const wxString &file_path) = 0;
	virtual bool CloseEditor() = 0;
	virtual void Activated() = 0;
	virtual bool LoadFile() = 0;
    virtual bool SaveFile () = 0;
    virtual bool Modified() = 0;
	virtual void ProcessEvent(wxCommandEvent &event) = 0;
	virtual cfReaderID GetReaderId() const = 0;
	wxString GetMessage() const {return m_Message;}
	wxString GetFilePath() const {return m_FilePath;}
	bool IsOpened() const {return !m_FilePath.IsEmpty();}
	bool IsEmpty() const {return m_FilePath.IsEmpty();}
	bool operator == (const CfFileEditorBase& reader)
	{
		return ((reader.GetReaderId()==this->GetReaderId()) && (reader.GetFilePath()==this->m_FilePath));
	}
	//void SetProject(CfProject* project){this->m_pProject = project;}
protected:
	wxString m_Message;
	wxString m_FilePath;
	//CfProject *m_pProject;
};

using namespace std;
WX_DECLARE_LIST(CfFileEditorBase, FileReaderList);
typedef list<CfFileEditorBase*> FileReaderList1;


class CfDrawingEditorBase
{

};
class CfExternalFileReader : public CfFileEditorBase
{
public:
	CfExternalFileReader();
	~CfExternalFileReader();
	cfReaderID GetReaderId() const;
	bool LoadFile();
	bool LoadFile(const wxString &file_path);
	void ProcessEvent(wxCommandEvent &event);
};



#endif //_CF_FILE_OPENER_H_