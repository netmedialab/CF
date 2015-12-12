/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfUtils.h
// Author      :James
// Created     :2010-01-28 ¿ÀÈÄ 8:11:51
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CfDefines.h"
#include "ParseUtil.h"
#include <wx/msgdlg.h> 
#include "psapi.h"
#include <wx/socket.h>


void ShowErrorDialog(const tchar* err_msg,wxWindow* parent=NULL);
void ShowWarningDialog(const tchar* warning_msg,wxWindow* parent=NULL);
void ShowInfoDialog(const tchar* warning_msg,wxWindow* parent=NULL);

void ParseURL(const wxChar* url, wxString& address, int& port);

void TrunckStringToList(const wxChar *src, wxArrayString *list, const wxChar *separator);
void ConcateListToString(wxArrayString *list, wxString &str, const wxChar *separator);

DWORD FindProcess(TCHAR*strProcessName);

void SocketSendInt(wxSocketBase *sock, int v);
int SocketGetInt(wxSocketBase *sock);

void fiiPostAnEvent(wxWindow *pParent, wxEventType evtType, int id, wxObject *obj, int num=0, wxString str=wxEmptyString);
