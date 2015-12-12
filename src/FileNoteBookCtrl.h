
/////////////////////////////////////////////////////////////////////////////////
//
// Name        : CfFileNoteBookCtrl.h
// Author      : MING
// Created     : 2015Äê2ÔÂ4ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _FILE_OPEN_HELPER_H_
#define _FILE_OPEN_HELPER_H_

#include <wx/hashmap.h>
#include "FileEditorBase.h"
#include "FileReaderManager.h"
#include "Project.h"
#include "Notification.h"

#define CURRENT_PAGE -1
class CfFileNotebookCtrl : public wxAuiNotebook, public CfFileReaderManager, public CfNotificationSender
{
public:
	CfFileNotebookCtrl(wxWindow* parent,
                  wxWindowID id = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxAUI_NB_DEFAULT_STYLE);
	~CfFileNotebookCtrl();

	CfFileEditorBase* AddEditor(const wxString &file_path,CfProject* project);
	CfFileEditorBase* GetEditor(int index = CURRENT_PAGE);
	void RefreshEditor(wxString editor_class_name);
	void EditorModified();
	bool IfNeedSave(); //check if it need save
	void SaveCurrentFile();
	void SaveAllFile();
	void CloseAllFiles();
	void CloseFile(int index = CURRENT_PAGE, bool delete_page=true);

private:
	WX_DECLARE_STRING_HASH_MAP( wxString, m_fileOpenerMap);
	wxString m_FilePath;

	void AddEditor(CfFileEditorBase *pFileReader, const wxString &title);
	void CfFileNotebookCtrl::SetPageTextModified(int index, bool modified);
};


#endif	//_FILE_OPEN_HELPER_H_