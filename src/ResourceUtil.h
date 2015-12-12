/////////////////////////////////////////////////////////////////////////////////
//
// Name        :Resource.h
// Author      :MING
// Created     :2011-04-19 ¿ÀÈÄ 5:03:21
// Description :
// Copyright   :(C) 2009-2010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "CfWx.h"

const wxString IMG_CHECK	= _("check.png");
const wxString IMG_UNCHECK	= _("uncheck.png");

const wxString IMG_MSG		= _("msg.png");
const wxString IMG_QUEUE	= _("queue.png");
const wxString IMG_FIND		= _("find.png");
const wxString IMG_REPLACE	= _("replace.png");

const wxString IMG_SCROLL	= _("scroll.png");
const wxString IMG_CLEAR	= _("clear.png");
const wxString IMG_REPEAT	= _("repeat.png");

const wxString IMG_LOGO		= _("CircuitFinder.ico");
//const wxString IMG_ONLINE	= _("online.ico");
const wxString IMG_ONLINE	= _("online.png");
const wxString IMG_OFFLINE	= _("offline.png");

const wxString IMG_FOLDER		= _("folder.png");
const wxString IMG_FOLDER_SMALL	= _("folder_small.png");
const wxString IMG_PROJECT		= _("project.png");
const wxString IMG_CLASSIFIER	= _("classifier.png");
const wxString IMG_WORKSPACE	= _("workspace.png");

const wxString IMG_RUN_LOCAL	= _("run_local.png");
const wxString IMG_RUN_REMOTE	= _("run_remote.png");
const wxString IMG_CONNECT		= _("connect.png");
const wxString IMG_CONSOLE		= _("console.png");
const wxString IMG_SERVER		= _("server.png");
const wxString IMG_DLG_HEADER	= _("dlg_header.png");
const wxString IMG_TRASH_BIN	= _("trash_bin.png");

//Draw
const wxString IMG_IMAGE		= _("image.png");
const wxString IMG_PORT			= _("port.png");
const wxString IMG_WIRE			= _("wire.png");
const wxString IMG_LINK			= _("link2.png");
const wxString IMG_LAYER		= _("layer.png");
const wxString IMG_SYMBOL		= _("symbol.png");
const wxString IMG_INSTANCE		= _("instance.png");
const wxString IMG_LAYER_SELECTED		= _("layerselected.png");

//const wxString IMG_= _(".png");
//const wxString IMG_= _(".png");

static const wxChar *FileTypeIconStr[] = 
{
	 wxT("VFile.png")
	,wxT("VHDFile.png")
	,wxT("CppFile.png")
	,wxT("CFile.png")
	,wxT("HFile.png")
	,wxT("ShellFile.png")
	,wxT("File.png")
	,wxT("SVFile.png")
	,wxT("File.png")
	,wxT("File.png")

	// wxT("IDI_VERILOG")
	//,wxT("IDI_VHDL")
	//,wxT("IDI_CPP")
	//,wxT("IDI_C")
	//,wxT("IDI_HEADER")
	//,wxT("IDI_SHELL")
	//,wxT("IDI_TEXT")
	//,wxT("IDI_TEXT")
};



class ResourceUtil
{
public:
	ResourceUtil(void);
	~ResourceUtil(void);

	static ResourceUtil* Get();
	static void Init();
	static void Destroy();
	wxBitmap GetBitmap(wxString name);
	wxIcon GetIcon(wxString name);

private:
	static ResourceUtil *m_pResource;
	wxString m_sResDir;

};
