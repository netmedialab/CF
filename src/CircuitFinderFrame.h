#pragma once
#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <stdlib.h>
#include <wx/aui/framemanager.h>
#include <wx/aui/auibook.h>
#include "CfWx.h"
#include "CfUtils.h"
#include "WorkspaceExplorerCtrl.h"
#include "FileNotebookCtrl.h"
#include "FileReaderManager.h"
#include "PropertyGridCtrl.h"
#include "LayerViewCtrl.h"
#include "LayerListViewCtrl.h"
#include "CfAboutDialogCtrl.h"
#include <wx/filehistory.h>
#include "CfSettingDialogCtrl.h"
#include "CfParameterDialogCtrl.h"
#include "CfDefines.h"
////Dialog Style Start
#undef REVERSIAN_FRAME_STYLE
#define REVERSIAN_FRAME_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX | wxMAXIMIZE
//#define REVERSIAN_FRAME_STYLE wxMAXIMIZE |  wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX 
////Dialog Style End


// Define a new application type
class CircuitFinderApp: public wxApp
{
public:
    bool OnInit();
};

IMPLEMENT_APP(CircuitFinderApp)

class CircuitFinderFrame :
	public wxFrame
{
public:
	CircuitFinderFrame(wxString& workspace_file);
	~CircuitFinderFrame(void);

private:
	wxDECLARE_EVENT_TABLE();

	wxAuiManager m_AuiMgr;
	wxAuiNotebook *m_pMessageNotebook;
	wxStatusBar *m_pStatusBar;
	CfWorkspaceExplorerCtrl *m_pWorkspaceExplorer;
	CfFileNotebookCtrl *m_pFileNotebook;
	CfPropertyGridCtrl *m_pElementPropertyCtrl;
	CfLayerViewCtrl *m_pLayerViewCtrl;
	CfLayerListViewCtrl *m_pLayerListViewCtrl;
	wxTextCtrl *m_pLogMessgeWin;
	wxAuiToolBar* m_pToolBar;
	
	CfWorkspace *m_pWorkspace;
	CfFileReaderManager m_FileReaderManager;

	wxString m_sProjectWorkingDir;
	wxString m_sOpenFileDir;

	wxFileHistory* m_pRecentProjectList;
	FileUtil *m_pFileUtils;
	//CLoadProjSuccessNotifyListenerPtrList m_LoadProjSuccuessNotifyListenerPtrList;
	CfEnvironmentUtil *m_pEnvironment;
	int m_iSettingPageNum;
	void LoadProjectSuccessed();

	void InitializeComponent();
	void AddMenu();
	void AddToolBar();
	void OnCircuitFinderEvent(wxEvent& event);
	void OnClickMenu(wxCommandEvent& event);
	void OnClosePage(wxAuiNotebookEvent& event);
	void OnClosedPage(wxAuiNotebookEvent& event);
	void OnChangedPage(wxAuiNotebookEvent& event);
	void OnAllowNotebookDnD(wxAuiNotebookEvent& event);
	void OnNewTabGroupUpdateUI(wxUpdateUIEvent &event);
	void OnSchematicAndSymbolEditorUpdateUI(wxUpdateUIEvent &event);
	void OnSchematicEditorUpdateUI(wxUpdateUIEvent &event);
	void ShowSettings();
	void ShowParameter();
	//UI update
	void OnFileExistUpdateUI(wxUpdateUIEvent &event);
	void OnWorkspaceExistUpdateUI(wxUpdateUIEvent &event);

	//File

	void NewFile();
	void OpenFile();
	void CloseFile();
	void CloseAllFiles();
	void CloseWorkspace();
	void SaveFile();
	void SaveFileAs();
	void SaveAllFiles();
	void NewProject();
	//void SetStatusBarText(wxString msg = wxEmptyString);
	void UpdateWinTitle();
	//void SentLoadProjSuccessNotify();
	void ShowAbout();
	void ShowManual();
	void ShowOpenWorkspaceDialog();
	void NewTabGroup(int direction);

	void OpenWorkspace(const wxChar* workspace_file, bool is_refresh = false);
	void OpenProject(const wxChar* project_file);
	void RefreshWS();
	wxString GetOpenDir(int mode = 0); // 0 :proj/workspace;  1: file
};

//size of chip editor 
