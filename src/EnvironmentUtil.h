/////////////////////////////////////////////////////////////////////////////////
//
// Name        :Environment.h
// Author      :MING
// Created     :2010-01-28 ¿ÀÈÄ 8:11:51
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once
//#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/arrstr.h>
#include <wx/wfstream.h>

//#define PROP_PREFIX wxT("CircuitFinder")
#define CONFIG_FILE_NAME	wxT("env.ini")
// drawing element
#define ENV_DRAWING_LINECOLOR	wxT("Drawing/LineColor")
#define ENV_DRAWING_FILLEDCOLOR	wxT("Drawing/FilledColor")
#define ENV_DRAWING_LINEWIDTH	wxT("Drawing/LineWidth")

// layout
#define PROP_FRAME_LAYOUT	wxT("Layout/Layout")
#define PROP_FRAME_WIDTH	wxT("Layout/Width")
#define PROP_FRAME_HEIGHT	wxT("Layout/Height")

// workspace
#define PROP_WORKSPACE _("Workspace/")
//#define PROP_LAST_WORKSPACE_NUM	wxT("Workspace/LastWorkspaceNum")
#define PROP_LAST_WORKSPACE		wxT("Workspace/LastWorkspace")

#define PROP_LASTOPENED_WORKSPACE		wxT("Workspace/LastOpenedWorkspace")
//add my last opened workspace

//#define PROP_LAST_ACTIVEPROJ	wxT("Workspace/LastActiveProject")
#define PROP_RECENT_PROJECT_NUM		wxT("Workspace/RecentProjectNum")
#define PROP_RECENT_PROJECT			wxT("Workspace/RecentProject")
#define PROP_RECENT_FILE_NUM	wxT("Workspace/RecentFileNum")
#define PROP_RECENT_FILE		wxT("Workspace/RecentFile")

// setting
#define PROP_GEN_SAVELOGFILE	wxT("Setting/SaveLogFile")
#define PROP_GEN_EXITREMIND		wxT("Setting/ExitRemind")
#define PROP_SETTING_PRIVATELIBPATH	wxT("Setting/PrivateLibPath")
#define PROP_SETTING_AUTOLOGOUTINVERVAL	wxT("Setting/AutoLogoutInterval")

#define PROP_SETTING_KEYFILE	wxT("Setting/KeyFile")
#define PROP_SETTING_ISPUTTY	wxT("Setting/IsPutty")
#define PROP_SETTING_PLINKPATH	wxT("Setting/PlinkPath")
#define PROP_SETTING_PUTTYPATH	wxT("Setting/PuttyPath")
#define PROP_SETTING_XMINGPATH	wxT("Setting/XmingPath")
#define PROP_SETTING_XMANKEYNAME wxT("Setting/XmanKeyName")
#define PROP_SETTING_XMANPATH	wxT("Setting/XmanagerPath")

//wbren setting gerenal
#define PROP_SETTING_PORT_DEFAULTLINECOLOR	wxT("Setting/Default Porperty/Port/Default Line Color")
#define PROP_SETTING_PORT_DEFAULTLINEWIDTH	wxT("Setting/Default Porperty/Port/Default Line Width")
#define PROP_SETTING_PORT_DEFAULTFILLEDCOLOR	wxT("Setting/Default Porperty/Port/Default Filled Color")
#define PROP_SETTING_PORT_SHOWN	wxT("Setting/Default Porperty/Port/Shown")

#define PROP_SETTING_WIRE_DEFAULTLINECOLOR	wxT("Setting/Default Porperty/Wire/Default Line Color")
#define PROP_SETTING_WIRE_DEFAULTLINEWIDTH	wxT("Setting/Default Porperty/Wire/Default Line Width")
#define PROP_SETTING_WIRE_DEFAULTFILLEDCOLOR	wxT("Setting/Default Porperty/Wire/Default Filled Color")
#define PROP_SETTING_WIRE_SHOWN		wxT("Setting/Default Porperty/Wire/Shown")

#define PROP_SETTING_SYMBOL_DEFAULTLINECOLOR	wxT("Setting/Default Porperty/Symbol/Default Line Color")
#define PROP_SETTING_SYMBOL_DEFAULTLINEWIDTH	wxT("Setting/Default Porperty/Symbol/Default Line Width")
#define PROP_SETTING_SYMBOL_DEFAULTFILLEDCOLOR	wxT("Setting/Default Porperty/Symbol/Default Filled Color")
#define PROP_SETTING_SYMBOL_SHOWN		wxT("Setting/Default Porperty/Symbol/Shown")

#define PROP_SETTING_GRID_SIZE		wxT("Setting/Default Porperty/Grid/Defaule Size")
//wbren setting layer
#define PROP_SETTING_DEFAULTLAYER	wxT("Setting/Layer/Default layer list")

// ftp
#define PROP_SETTING_FTP_STARTUP	wxT("Setting/FtpStartup")
#define PROP_SETTING_FTP_TRANSMODE	wxT("Setting/FtpTransMode")
#define PROP_SETTING_FTP_PORT		wxT("Setting/FtpPort")
#define PROP_SETTING_FTP_PROTOCOL	wxT("Setting/FtpProtocol")

// file association
#define PROP_FILE_ASSOCIATION wxT("FileAssociation/")

// user defined local tool
#define PROP_UDTOOLS			wxT("UserDefinedTools/")
#define PROP_UDTOOL				wxT("UserDefinedTools/Tool")
#define PROP_UDTOOL_NUM			wxT("UserDefinedTools/ToolNum")
#define PROP_UDTOOLS_DESCRIPT	wxT("UserDefinedTools/Description/Desc")

// project type
#define PROP_PROJTYPE			wxT("ProjectTypes/Type")
#define PROP_PROJTYPE_NUM		wxT("ProjectTypes/TypeNum")
#define PROP_PROJTYPE_DESCRIPT	wxT("ProjectTypes/Description/Desc")
#define PROP_PROJTYPE_TEMPLATE	wxT("ProjectTypes/Template/Temp")

// file type
#define PROP_FILETYPE			wxT("FileTypes/Type")
#define PROP_FILETYPE_NUM		wxT("FileTypes/TypeNum")
#define PROP_FILETYPE_DESCRIPT	wxT("FileTypes/Description/Desc")
#define PROP_FILETYPE_TEMPLATE	wxT("FileTypes/Template/Temp")


class IEnvironment 
{
public:
	IEnvironment(){};
	virtual ~IEnvironment(){};

	virtual bool IsModified()=0;
	virtual int GetInt(const wxChar* key, int defualt)=0;
	virtual bool GetBool(const wxChar* key, bool defualt)=0;
	virtual wxString GetString(const wxChar* key, const wxChar* default)=0;
	virtual void SetNotModified()=0;
	virtual void Remove(const wxChar* key)=0;
	virtual void RemoveByPrefix(const wxChar* prefix)=0;
	virtual void SetString(const wxChar* key, const wxChar* value)=0;
	virtual void SetInt(const wxChar* key, int value)=0;
	virtual void SetBool(const wxChar* key, bool value)=0;

};

class CfEnvironmentUtil : public IEnvironment, public wxFileConfig// wxConfig
{
public:
	//CfEnvironmentUtil(const wxChar* appName);
	CfEnvironmentUtil(const wxString & file_path);
	//CfEnvironmentUtil::CfEnvironmentUtil(wxFileInputStream* is)
	~CfEnvironmentUtil();

	static CfEnvironmentUtil* Get();
	static void Init();
	static void Destroy();

	bool IsModified();
	int GetInt(const wxChar* key, int defualt);
	bool GetBool(const wxChar* key, bool defualt);
	wxString GetString(const wxChar* key, const wxChar* default);
	void SetNotModified();
	void Remove(const wxChar* key);
	void RemoveByPrefix(const wxChar* prefix);
	void SetString(const wxChar* key, const wxChar* value);
	void SetStringList(const wxChar* key, const wxArrayString* string_list);
	void SetInt(const wxChar* key, int value);
	void SetBool(const wxChar* key, bool value);
	void GetStringList(const wxChar* key, wxArrayString* string_list);
	void AppendStringList(const wxChar* key, const wxArrayString* string_list);
	void AddtoStringList(const wxChar* key, const wxChar* item);
	void RemoveList(const wxChar* key);


private:
	static CfEnvironmentUtil* m_pEnvironment;
	wxString m_sConfigFilePath;

	bool m_bModified;
	wxArrayString m_lRecentProjects;
	wxArrayString m_lUrlList;
	//wxString m_sFtpTransferMode;

};
