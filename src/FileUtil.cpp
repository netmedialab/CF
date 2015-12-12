/////////////////////////////////////////////////////////////////////////////////
//
// Name        :FileUtils.cpp
// Author      :MING
// Created     :2010-04-05 ¿ÀÈÄ 2:15:27
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/file.h>
#include <wx/filename.h>
#include "CfWx.h"
#include "FileUtil.h"
#include "LogMessageCtrl.h"

DirDelTraverser::DirDelTraverser()
{}

wxDirTraverseResult DirDelTraverser::OnFile(const wxString& filename)
{
    wxRemoveFile(filename);
    return wxDIR_CONTINUE;
}

wxDirTraverseResult DirDelTraverser::OnDir(const wxString& dirname)
{
	wxRmDir(dirname);
    return wxDIR_CONTINUE;
}

//FileUtil
FileUtil *FileUtil::m_pFileUtil = 0;
FileUtil::FileUtil()
{
	m_sFiiAppDir = wxStandardPaths::Get().GetExecutablePath().BeforeLast('\\');
}

FileUtil* FileUtil::Get()
{
	if(!m_pFileUtil) Init();
	return m_pFileUtil;
}

void FileUtil::Init()
{
	if(!m_pFileUtil)
		m_pFileUtil = new FileUtil();
}
void FileUtil::Destroy()
{
	if(!m_pFileUtil) return;
	delete m_pFileUtil;
	m_pFileUtil = 0;
	
}

//const wxChar* FileUtil::GetFileTypeIcon(int type_index) 
//{
//	return FileTypeIconStr[type_index];
//}

const wxChar* FileUtil::GetFileTypeStr(int type_index) 
{
	return FileTypeStr[type_index];
}

int FileUtil::GetFileTypeIndex(wxString ext) 
{
	int type_index = -1;
	if(ext.CmpNoCase(wxT("v")) == 0) type_index = FT_VERILOG;
	else if(ext.CmpNoCase(wxT("sv")) == 0) type_index = FT_SYSVER;
	else if(ext.CmpNoCase(_("vhdl")) == 0) type_index = FT_VHDL;
	else if((ext.CmpNoCase(_("cpp")) == 0)||(ext.CmpNoCase(_("cc")) == 0)) type_index = FT_CPP;
	else if(ext.CmpNoCase(_("c")) == 0) type_index = FT_C;
	else if(ext.CmpNoCase(_("h")) == 0) type_index = FT_HEADER;
	else if(ext.CmpNoCase(_("sh")) == 0) type_index = FT_SHELL;
	else //if(ext.CmpNoCase(_("txt")) == 0) 
		type_index = FT_TEXT;

	return type_index;
}

wxString FileUtil::ChangeToAbsolutePath(const wxChar *relative_path,const wxChar *base_path)
{
	wxFileName item_fname(relative_path);
	if(item_fname.IsRelative()) item_fname.MakeAbsolute(base_path);
	return item_fname.GetFullPath();
}

wxString FileUtil::ChangeToRelativePath(const wxChar *absolute_path,const wxChar *base_path)
{
	wxFileName item_fname(absolute_path);
	wxLogDebug(item_fname.GetFullPath());
	if(!item_fname.IsRelative()) 
		item_fname.MakeRelativeTo(base_path);
	wxLogDebug(item_fname.GetFullPath());
	return item_fname.GetFullPath();
}

wxString FileUtil::GetDirectory(int dir_id)
{
	wxString dir;
	switch(dir_id)
	{
	case ID_APPDIR:		dir = m_sFiiAppDir; break;
	case ID_CONFDIR:	dir = m_sFiiAppDir+DIR_CONF; break;
	case ID_DOCDIR:		dir = m_sFiiAppDir+DIR_DOC; break;
	case ID_TOOLSDIR:	dir = m_sFiiAppDir+DIR_TOOLS; break;
	case ID_RESDIR:		dir = m_sFiiAppDir+DIR_RES; break;
	case ID_PROJDIR:	dir = m_sFiiAppDir+DIR_PROJECT; break;
	case ID_KEYSDIR:	dir = m_sFiiAppDir+DIR_KEYS; break;
	case ID_LOGDIR:		dir = m_sFiiAppDir+DIR_LOG; break;
	case ID_LIBDIR:		dir = m_sFiiAppDir+DIR_LIB; break;
	case ID_TEMPDIR:	dir = wxStandardPaths::Get().GetTempDir(); break;
	case ID_PROGRAMDIR: dir = ::wxIsPlatform64Bit()?BIT64_PROGRAM_PATH:PROGRAM_PATH; break;
	}

	if(!wxDirExists(dir.c_str()) && !wxMkdir(dir.c_str())) 
	{
		wxLogMessage(_("Failed to create directory '%s'"),dir.c_str());
		return wxEmptyString;
	}

	return dir;
}

bool FileUtil::DelDir(const wxString &path)
{
	
	wxDir dir(path);
	dir.Traverse(*this,wxEmptyString,wxDIR_DIRS | wxDIR_FILES);
	return wxRmdir(path);
}

wxDirTraverseResult FileUtil::OnFile(const wxString& filename)
{
    wxRemoveFile(filename);
    return wxDIR_CONTINUE;
}

wxDirTraverseResult FileUtil::OnDir(const wxString& dirname)
{
	DelDir(dirname);
    return wxDIR_CONTINUE;
}

wxString FileUtil::WriteCmdTmepFile(const wxChar* cmd,const wxChar* file_extension)
{
	FileUtil file_utils;
	wxString temp_dir = file_utils.GetDirectory(ID_TEMPDIR);
	wxFile cmd_file;
	wxString cmd_file_name; 
	wxDateTime dt = wxDateTime::Now();
	wxString fn= dt.Format(_("_tmp_%H%M%S"));

		cmd_file_name.Printf(_("%s\\%s%s"),temp_dir,fn,file_extension);

	if(wxFile::Exists(cmd_file_name))
		cmd_file.Open(cmd_file_name,wxFile::write);
	else
	{
		if(!cmd_file.Create(cmd_file_name))	return wxEmptyString;
	}
	cmd_file.Write(cmd);
	cmd_file.Close();
	return cmd_file_name;
}


void FileUtil::DelTempFiles()
{
	wxDir dir(GetDirectory(ID_TEMPDIR));
	dir.Traverse(*this,_("_tmp_*"),wxDIR_FILES);

}