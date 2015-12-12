/////////////////////////////////////////////////////////////////////////////////
//
// Name        :CfUtils.cpp
// Author      :James
// Created     :2010-01-28 ¿ÀÈÄ 8:11:51
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#include "CfUtils.h"
#include "wx/log.h"
#include "LogMessageCtrl.h"


void ShowErrorDialog(const tchar* err_msg,wxWindow* parent)
{
    wxMessageBox(err_msg, PROJECT_CAPTION , wxOK | wxICON_ERROR, parent);
}

void ShowWarningDialog(const tchar* warning_msg,wxWindow* parent)
{
	wxMessageBox(warning_msg, PROJECT_CAPTION , wxOK | wxICON_WARNING, parent);
}

void ShowInfoDialog(const tchar* info_msg,wxWindow* parent)
{
	wxMessageBox(info_msg, PROJECT_CAPTION , wxOK | wxICON_INFORMATION, parent);
}

void ParseURL(const wxChar* url, wxString& address, int& port)
{
	port = 80;
	wxString s_url(url);
	wxString s_port;
	long lport = 80;
	address = s_url.BeforeFirst (':');
	s_port = s_url.AfterFirst(':');
	if(!s_port.IsEmpty())
		s_port.ToLong(&lport);
	port = lport;
}

void TrunckStringToList(const wxChar *src, wxArrayString *list, const wxChar *separator)
{
	wxString str(src);


	if (!str.EndsWith(separator)) str << separator;  // expand ',' in the end

	wxString trunk_str;
	int left = 0;
	int right = str.find(',');  // eg: '.txt,.cpp,'
	int length = str.Length();
	while(right > 0)
	{
		trunk_str = str.Mid(left,right-left);
		trunk_str.Trim(false);
		list->Add(trunk_str);

		left = right + 1; // '.txt'
		if(left >= length - 1) break;
		right = str.find(',',left);
	}
}

void ConcateListToString(wxArrayString *list, wxString &str, const wxChar *separator)
{
	for(int i=0;i<list->GetCount();i++)
	{
		str << list->Item(i).c_str() << separator;
	}
}


DWORD FindProcess(TCHAR*strProcessName)
{
    DWORD aProcesses[1024], cbNeeded, cbMNeeded;
    HMODULE hMods[1024];
    HANDLE hProcess;
    TCHAR szProcessName[MAX_PATH];
	int count = 0;

    //if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )  return 0;
    for(int i=0; i< (int) (cbNeeded / sizeof(DWORD)); i++)
    {
        hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
        //EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
        //GetModuleFileNameEx( hProcess, hMods[0], szProcessName,sizeof(szProcessName));
       
        if(_tcsstr(szProcessName, strProcessName))
        {
			count++;
        }
    }

//			wxLogDebug(_T("[DEBUG]Process '%s' is running %d copy"),szProcessName,count);
    return count;
}


int SocketGetInt(wxSocketBase *sock)
{
	int key = 0;
	for(int i=0;i<4;i++)
	{
		char c=0;
		//if(recv(socket, &c, 1,MSG_WAITALL) != 1)
		sock->Read(&c, 1);
		if(sock->Error())
		{
			//throw SocketRecvException();
			wxLogError(_("Error in read int from scoket"));
			break;
		}
		int d = c;  // ming add for the problem of getting wrong int of 384
		if(c<0) 
			{d = 256+c;}
		key += d * (int)pow(256,(double)(3-i));
	}
	return key;
}

void SocketSendInt(wxSocketBase *sock, int v)
{
	for(int i=0;i<4;i++) 
	{
		const char buf = (v / (int)pow(256,(double)(3-i))) % 256;
		sock->Write(&buf,1);
		if(sock->Error())
		{
			wxLogError(_("Error in read int from scoket"));
			break;
		}
	}
}

void fiiPostAnEvent(wxWindow *pParent, wxEventType evtType, int id, wxObject *obj, int num, wxString str)
{
	wxCommandEvent e(evtType,id);
	e.SetEventObject(obj);
	e.SetString(str);
	e.SetInt(num);
//	pParent->GetEventHandler()->ProcessEvent(e);
	wxPostEvent(pParent,e);

}