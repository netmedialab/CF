
/////////////////////////////////////////////////////////////////////////////////
//
// Name        : WorkspaceExplorerCtrl.h
// Author      : MING
// Created     : 2015Äê1ÔÂ31ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef WORKSPACE_EXPLORER_CTRL_H
#define WORKSPACE_EXPLORER_CTRL_H


#include <map>
#include "CfDefines.h"
#include "Project.h"
#include "Workspace.h"
#include "EnvironmentUtil.h"
#include "Notification.h"


enum
{
    ID_WorkspaceExplorer_NewProject = ID_WorkspaceExplorer + 1
	,ID_NewProject
	,ID_OpenProject
	,ID_WorkspaceExplorer_OpenProject
	,ID_WorkspaceExplorer_AddFolder
	,ID_WorkspaceExplorer_AddNewFile
	,ID_WorkspaceExplorer_AddExistingFile
	,ID_WorkspaceExplorer_AddClassifier
	,ID_WorkspaceExplorer_BackupProject		//MING20110415
	,ID_WorkspaceExplorer_BackupWorkspace	//MING20110415
	,ID_WorkspaceExplorer_OpenFile
	,ID_WorkspaceExplorer_RemoveItem
	,ID_WorkspaceExplorer_UploadProject
	,ID_WorkspaceExplorer_UploadItem
	,ID_WorkspaceExplorer_SetActiveProject
	,ID_WorkspaceExplorer_CloseProject
	,ID_WorkspaceExplorer_SaveProject
	,ID_WorkspaceExplorer_OpenProperty
	,ID_WorkspaceExplorer_RefreshWS
	,ID_WorkspaceExplorer_SortProject		//MING_20110810
	,ID_WorkspaceExplorer_Rename

	,ID_WorkspaceExplorer_AnalyzeProject
	,ID_WorkspaceExplorer_GenerateScript
	,ID_WorkspaceExplorer_OpenWSProperty
	,ID_WorkspaceExplorer_OpenWSRefresh

};

class CfWorkspaceExplorerCtrl 
	: public wxTreeCtrl, public CfNotificationSender
{
public:
	// class constructor
	//CfWorkspaceExplorerCtrl(wxWindow *parent,CfProject *pProject,wxAuiNotebook *pFileNotebook,CFMFtpManager *pFtp);
	CfWorkspaceExplorerCtrl(wxWindow *parent);
	// class destructor
	~CfWorkspaceExplorerCtrl();

	void LoadWorkspace(CfWorkspace *pWorkspace);
	void CloseCurrentWorkspace();
	void BackupWorkspace();

	void AddProjectNode(CfProject *project);
	void SetActiveProject(wxTreeItemId id);
	void DeleteProjectNode(wxTreeItemId id);
	virtual void LoadProjectSuccessed();
	//first find the node with parent_name under node of root_id, add to parent_node
	//if failed to find, add to root
	CfProjectItem* AddProjectItem(const wxTreeItemId& rood_id,wxString name,const CfItemType type, const wxString& parent_name=wxEmptyString);

private:
	void AddChildTreeNode(wxTreeItemId id, CProjectItemList *child_list);
    void OnContextMenu(wxContextMenuEvent& event);
	void OnItemActivated(wxTreeEvent& event);
    void OnItemMenu(wxTreeEvent& event);  // right click item
    void OnPressKey(wxTreeEvent& event);  // press del or ... keys
	void OnLabelEdit(wxTreeEvent& event);
	void OnEndLabelEdit(wxTreeEvent& event);
	void OnSelChanged(wxTreeEvent& event);

	void OnAddNewFile(wxCommandEvent& event);  
	void OnAddExistingFile(wxCommandEvent& event);  
	void OnAddFolder(wxCommandEvent& event);  
	void OnAddClassifier(wxCommandEvent& event);  
	void OnBackupProject(wxCommandEvent& event);  
	void OnBackupWorkspace(wxCommandEvent& event);  
	void OnOpenFile(wxCommandEvent& event);  
	void OnRemoveItem(wxCommandEvent& event);
	void OnRenameFolder(wxCommandEvent& event);
	void OnUploadProject(wxCommandEvent& event);
	void OnUploadItem(wxCommandEvent& event);
	void OnSetActiveProject(wxCommandEvent& event);
	void OnNewProject(wxCommandEvent& event);
	void OnOpenProject(wxCommandEvent& event);
	void OnCloseProject(wxCommandEvent& event);
	void OnSaveProject(wxCommandEvent& event);
	void OnOpenProperty(wxCommandEvent& event);
	void OnOpenWSProperty(wxCommandEvent& event);
	void OnRefreshWS(wxCommandEvent& event);
	void OnSortProject(wxCommandEvent& event);
	void OnAnalyzeProject(wxCommandEvent& event);
	void OnGenerateScript(wxCommandEvent& event);

	//CUserInputEventHandler
	void OnUserInputEvent(wxCommandEvent &event);
	void OnMouseEvents(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);

	//void UploadItem(CfProjectItem *item,CServerPath *remote_path);
	//void UploadChildItems(CfProjectItem *item,std::vector<CServerPath> *path_stack, bool &file_add);
	void SendInitEvent();

    void ShowMenu(CfProjectItem *item, const wxPoint& pt);
	//void OpenFile(CfProjectItem *item,bool IsNew = false);
	bool RenameFolder(const wxTreeItemId id,const wxString new_name);
	void RemoveItem(const wxTreeItemId id);

	bool CreateFolder(const wxTreeItemId parent_id,wxString name);
	CfProject *GetProjectFromItem(const wxTreeItemId id);
	int GetTypeImageIndex(CfProjectItem *item);

	void NewFileDialog(const wxString& dir, CfProject* project);

	

	DECLARE_EVENT_TABLE()

private:
//	wxWindow *m_pParent;
	CfWorkspace *m_pWorkspace;

	std::map<wxTreeItemIdValue,CfProject*> m_ProjectRootNodeMap;
	wxTreeItemId m_RootId;
	wxTreeItemId m_BoldId;

	bool m_bRunProject;
	bool m_bEvtFromTreeMenu;

	wxTreeItemId FindTreeItemByName(const wxTreeItemId &parent_id, const wxString& name);

};

#endif // PROJECTFILETREECTRL_H
