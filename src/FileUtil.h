/////////////////////////////////////////////////////////////////////////////////
//
// Name        :FileUtils.h
// Author      :MING
// Created     :2010-03-19 ¿ÀÈÄ 4:23:10
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef FILE_TYPE_UTILS
#define FILE_TYPE_UTILS

#include "wx/dir.h"
//#include "CfDefines.h"

const enum
{
 ID_APPDIR =	0
,ID_CONFDIR	
,ID_DOCDIR	
,ID_TOOLSDIR 
,ID_KEYSDIR	
,ID_RESDIR	
,ID_PROJDIR	
,ID_LOGDIR	
,ID_LIBDIR	
,ID_TEMPDIR	
,ID_PROGRAMDIR
};

#define DIR_CONF	_("\\conf\\")
#define DIR_DOC		_("\\doc\\")
#define DIR_TOOLS	_("\\tools\\")
#define DIR_KEYS	_("\\keys\\")
#define DIR_RES		_("\\res\\")
#define DIR_PROJECT	_("\\projects\\")
#define DIR_LOG		_("\\log\\")
#define DIR_LIB		_("\\lib\\")

#define BIT64_PROGRAM_PATH			_("C:\\Program Files(X86)\\")
#define PROGRAM_PATH				_("C:\\Program Files\\")

//File extension
#define F_EXT_CHIP_IMAGE		_("jpg")
#define F_EXT_CHIP_SCHEMATIC	_("cpsc")
#define F_EXT_CHIP_SYMBOL		_("cpsy")
const enum FileType
{
	FT_VERILOG
	,FT_VHDL
	,FT_CPP
	,FT_C
	,FT_HEADER
	,FT_SHELL
	,FT_TEXT
	,FT_SYSVER
	,FT_CHIP_SCHEMATIC
	,FT_CHIP_SYMBOL
	,FT_UNKNOWN
};

static const wxChar *FileTypeStr[] = 
{
	wxT("Verilog File")
	,wxT("SystemVerilog File")
	,wxT("VHDL File")
	,wxT("C++ File")
	,wxT("C File")
	,wxT("Header File")
	,wxT("Shell File")
	,wxT("Text File")
	,wxT("Chip Schematic")
	,wxT("Chip Symbol")
	,wxT("Unknown File")
};

class DirDelTraverser: public wxDirTraverser
{
	DirDelTraverser();
	virtual wxDirTraverseResult OnFile(const wxString& filename);
    virtual wxDirTraverseResult OnDir(const wxString& WXUNUSED(dirname));
};


class FileUtil: public wxDirTraverser
{
public:
	FileUtil();
	static FileUtil* Get();
	static void Init();
	static void Destroy();

	static wxString WriteCmdTmepFile(const wxChar* cmd,const wxChar* file_extension=NULL);

	//static const wxChar* GetFileTypeIcon(int type_index);
	static const wxChar* GetFileTypeStr(int type_index);
	static int GetFileTypeIndex(wxString ext);
	static wxString ChangeToAbsolutePath(const wxChar *relative_path,const wxChar *base_path);
	static wxString ChangeToRelativePath(const wxChar *absolute_path,const wxChar *base_path);
	wxString GetDirectory(int dir_id);
	void DelTempFiles();
	bool DelDir(const wxString &path);
	
	virtual wxDirTraverseResult OnFile(const wxString& filename);
    virtual wxDirTraverseResult OnDir(const wxString& WXUNUSED(dirname));
private:
	static FileUtil *m_pFileUtil;
	wxString m_sFiiAppDir;
};

#endif