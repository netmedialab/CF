/////////////////////////////////////////////////////////////////////////////////
//
// Name        :fiiProject.cpp
// Author      :MING
// Created     :2010-01-28 ¿ÀÈÄ 8:11:51
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <wx/filename.h>
#include "CfWx.h"
#include "FileUtil.h"
#include "Workspace.h"

// -----------------------------------------
// CfWorkspace
// -----------------------------------------
CfWorkspace::CfWorkspace() 
:m_iActiveProjectIndex(-1),m_bChanged(false)
{

}
CfWorkspace::~CfWorkspace()
{
	Close();
}

bool CfWorkspace::AskForSave()
{
	if(m_bChanged)	Dump();
	return true;
}
CfProject* CfWorkspace::LoadProject(const wxChar* project_file)
{
	if(!project_file) return NULL;
	int index = FindProject(project_file);

	CfProject* project;
	if(index<0)
	{
		project = new CfProject();
		if (project->Load(project_file))
		{
			AddProject(project);
		}
		else
		{
			delete project;
			return NULL;
		}
	}
	else 
	{
		if(index != m_iActiveProjectIndex)
		{
			SetActiveProject(index);
		}
		project = GetProject(index);
	}

	return project;
}

void CfWorkspace::SaveWorkspace()
{
	for(size_t i=0;i<m_ProjectArray.GetCount();i++)
	{
		m_ProjectArray[i]->Dump();
	}
	Dump();

}

void CfWorkspace::Close()
{
	if(!AskForSave()) return;
	for(size_t i=0;i<m_ProjectArray.GetCount();i++)
	{
		delete m_ProjectArray[i];
	}
	m_ProjectArray.Clear();

}
int CfWorkspace::FindProject(const wxChar* proj_path)
{
	size_t i=0;
	for(;i<m_ProjectArray.GetCount();i++)
	{
//		wxLogMessage(_("%s vs %s"),m_ProjectArray[i]->GetProjectPath().c_str(),proj_path);
		if(m_ProjectArray[i]->GetProjectPath().CmpNoCase(proj_path) == 0)
			return i;
	}

	return -1;
}

void CfWorkspace::AddProject(CfProject* project)
{
	int index = FindProject(project->GetProjectPath().c_str());
	if(index < 0)
	{
		m_ProjectArray.Add(project);
		m_iActiveProjectIndex = m_ProjectArray.GetCount()-1;
	}
	else
		m_iActiveProjectIndex = index;
	m_bChanged = true;

}
void CfWorkspace::DeleteProject(CfProject* project)
{
	if(!project) return;
	int i=0;
	for(;i<m_ProjectArray.GetCount();i++)
	{
		///wxLogMessage(_("check %d %s"),i,m_ProjectArray[i]->GetProjectPath().c_str());
		if(m_ProjectArray[i]->GetProjectPath().Cmp(project->GetProjectPath()) == 0)
		{
			delete m_ProjectArray[i];
			//wxLogMessage(_("delete %d %s"),i,m_ProjectArray[i]->GetProjectPath().c_str());
			m_ProjectArray.RemoveAt(i);
			break;
		}
	}
	if(m_iActiveProjectIndex == i)
	{
		if(m_ProjectArray.GetCount()>0) m_iActiveProjectIndex = 0;
		else m_iActiveProjectIndex = -1;
	}
	else if((m_iActiveProjectIndex > i) && (i != m_ProjectArray.GetCount()))
	{
		m_iActiveProjectIndex--;
	}	
	m_bChanged = true;
}
CfProject* CfWorkspace::GetProject(int index)
{
	if((index<-1)||(index>=m_ProjectArray.GetCount())) return NULL;

	return m_ProjectArray[index];
}

void CfWorkspace::SetActiveProject(int index)
{
	if((index<-1)||(index>=m_ProjectArray.GetCount())) 
		m_iActiveProjectIndex = -1;
	else
		m_iActiveProjectIndex = index;
	m_bChanged = true;
}

int	CfWorkspace::GetActiveProjectIndex()
{
	return m_iActiveProjectIndex;
}


CfProject* CfWorkspace::GetActiveProject()
{
	if((m_iActiveProjectIndex != -1) && (m_iActiveProjectIndex < m_ProjectArray.GetCount()))
		return m_ProjectArray[m_iActiveProjectIndex];
	else
		return NULL;
}

int CfWorkspace::GetProjectCount()
{
	return m_ProjectArray.GetCount();
}

int	CfWorkspace::GetWorkspaceFiles(wxArrayString &files)
{
	for(size_t i=0;i<m_ProjectArray.Count();i++)
		m_ProjectArray[i]->GetProjectFiles(files);
	return files.Count();
}

wxString CfWorkspace::GetWorkspaceName() const
{
	return m_sWorksapceName;
}
wxString CfWorkspace::GetWorkspacePath() const
{
	return m_sWorkspacePath;
}
wxString CfWorkspace::GetWorkspaceDir() const
{
	return m_sWorkspacePath.BeforeLast('\\');
}

wxString CfWorkspace::GetAuthor() const
{
	return m_sAuthor;
}
wxString CfWorkspace::GetDescription() const
{
	return m_sDescription;
}
wxString CfWorkspace::GetWorkspaceVersion() const
{
	return m_sWorkspaceVersion;
}

void CfWorkspace::SetAuthor(wxString name)
{
	m_sAuthor = name;
	m_bChanged = true;
}
void CfWorkspace::SetDescription(wxString desc)
{
	m_sDescription = desc;
	m_bChanged = true;
}
void CfWorkspace::SetVersion(wxString ver)
{
	m_sWorkspaceVersion = ver;
	m_bChanged = true;
}

bool CfWorkspace::Load(const wxString& workspace_file)
{
	if(workspace_file.IsEmpty()) return false;
	wxFileName ws_fname(workspace_file);
	if(ws_fname.IsRelative())
		ws_fname.MakeAbsolute();
	wxString full_path = ws_fname.GetFullPath();
	wxTextFile	fWorkspaceFile(workspace_file);
	if(!fWorkspaceFile.Exists())
	{
		wxString msg;
		msg.Printf(wxT("Workspace file '%s' does not exist!"),full_path);
		wxLogError(msg);
		return false;
	}
	if(!fWorkspaceFile.Open()) 
	{
		wxString msg;
		msg.Printf(wxT("Failed to open workspace file '%s'!"),full_path);
		wxLogError(msg);
		return false;
	}

	m_sWorkspacePath = wxString(full_path);
	
	// get the dir path of workspace for changing relative path of item
	wxString ws_fdir = ws_fname.GetPath();
	m_sWorksapceName = ws_fname.GetName();


	wxString strLine;
	wxString name, value;

	int pos;
	for(strLine = fWorkspaceFile.GetFirstLine();!fWorkspaceFile.Eof();strLine = fWorkspaceFile.GetNextLine())
	{
		pos = strLine.Find('=');
		if(pos != wxNOT_FOUND)
		{
			name = strLine.Left(pos);
			value = strLine.Right(strLine.Len() - pos - 1);

			if(name.Cmp(WS_DESCRIPT) == 0)
			{
				m_sDescription = value;
				m_sDescription.Replace(_("<Return>"),_("\n"));
			}
			else if(name.Cmp(WS_VERSION) == 0)
				m_sWorkspaceVersion = value;
			else if(name.Cmp(WS_AUTHOR) == 0)
				m_sAuthor = value;
			else if(name.Cmp(WS_ACTIVEPROJ) == 0)
				value.ToLong(&m_iActiveProjectIndex);
			else if(name.Cmp(WS_PROJFILE) == 0) 
			{
					
				// check the path is absolute or relative.
				// change to absolute if it is relative.
				wxString item_fpath = FileUtil::ChangeToAbsolutePath(value.c_str(),ws_fdir.c_str()); 
				LoadProject(item_fpath);
			}
		}
	}
	fWorkspaceFile.Close();
	m_bChanged = false;
	return true;

}

void CfWorkspace::Dump(wxString path)
{
	if(path.IsEmpty())
		path = m_sWorkspacePath;

	wxTextFile fWorkspaceFile(path);
	if(fWorkspaceFile.Exists()) 
	{
		fWorkspaceFile.Open();
		fWorkspaceFile.Clear();
	}
	else fWorkspaceFile.Create();

	wxString strLine;
	if(!m_sDescription.IsEmpty())
	{
		m_sDescription.Replace(_("\n"),_("<Return>"));
		strLine.Printf(wxT("%s=%s"),WS_DESCRIPT,m_sDescription);
		fWorkspaceFile.AddLine(strLine);
	}
	if(!m_sAuthor.IsEmpty())
	{
		strLine.Printf(wxT("%s=%s"),WS_AUTHOR,m_sAuthor);
		fWorkspaceFile.AddLine(strLine);
	}
	if(!m_sWorkspaceVersion.IsEmpty())
	{
		strLine.Printf(wxT("%s=%s"),WS_VERSION,m_sWorkspaceVersion);
		fWorkspaceFile.AddLine(strLine);
	}
	
	for(size_t i=0;i<m_ProjectArray.GetCount();i++)
	{
		wxString prj_path = FileUtil::ChangeToRelativePath(m_ProjectArray[i]->GetProjectPath().c_str(),path.BeforeLast('\\'));
		strLine.Printf(wxT("%s=%s"),WS_PROJFILE,prj_path.c_str());
		fWorkspaceFile.AddLine(strLine);
	}

	if(m_iActiveProjectIndex >= 0)
	{
		strLine.Printf(wxT("%s=%d"),WS_ACTIVEPROJ,m_iActiveProjectIndex);
		fWorkspaceFile.AddLine(strLine);
	}
	fWorkspaceFile.Write();
	fWorkspaceFile.Close();

	m_bChanged = false;
}



CfWorkspace* CfWorkspace::m_pWorkspace = 0;

CfWorkspace* CfWorkspace::Get()
{
	if(!m_pWorkspace) Init();
	return m_pWorkspace;
}
void CfWorkspace::Init()
{
	if(!m_pWorkspace) 
		m_pWorkspace = new CfWorkspace();
}
void CfWorkspace::Destroy()
{
	if(!m_pWorkspace) return;

	delete m_pWorkspace;
	m_pWorkspace = 0;

}

void CfWorkspace::SetWorkspace(CfWorkspace* ws)
{
	m_pWorkspace = ws;
}

//MING_20110810
bool CfWorkspace::SortProject()
{
	CfProject* proj = NULL;
	int index;
	int proj_cnt = m_ProjectArray.GetCount();
	for(size_t i=0;i<proj_cnt-1;i++)
	{
		//// remove correspoinding tree node id 
		//m_ProjectArray[i]->UnsetTreeItemId();

		wxString cur_proj = m_ProjectArray[i]->GetProjectName();
		index = i;
		for(int j=i+1;j<proj_cnt;j++)
		{
			if(cur_proj.CmpNoCase(m_ProjectArray[j]->GetProjectName())>0) 
			{
				cur_proj = m_ProjectArray[j]->GetProjectName();
				index = j;
			}
		}
		if (index != i)
		{
			proj = m_ProjectArray[i];
			m_ProjectArray[i] = m_ProjectArray[index];
			m_ProjectArray[index] = proj;

			if(m_iActiveProjectIndex == index) m_iActiveProjectIndex = i;
			else if(m_iActiveProjectIndex == i) m_iActiveProjectIndex = index;
		}
	}
	//// for last project in WS, remove correspoinding tree node id 
	//m_ProjectArray[proj_cnt-1]->UnsetTreeItemId();
			
	return true;
}

bool CfWorkspace::Backup(wxString bk_dir)
{
	//Check backup directory
	if(!wxDir::Exists(bk_dir))
	{
		wxLogMessage(_("[File] Create directory '%s'"),bk_dir);
		bool done = wxMkdir(bk_dir);
		if(!done)
		{
			wxString msg;
			msg.Printf(_("Failed to create diretory '%s'"),bk_dir);
			wxLogError(msg);
			return false;
		}
	}

	wxString ws_path = bk_dir + _("\\") + GetWorkspaceName();
	ws_path.Printf(_("%s\\%s.flw"),bk_dir, GetWorkspaceName());
	//if(wxFile::Exists(ws_path))
	//{
	//	if (wxMessageBox(_("Workspace exists! Overwrite?"), PROJECT_CAPTION, wxICON_QUESTION | wxYES_NO, NULL) != wxYES)
	//		return false;
	//}
	
	// Dump workspace file
	wxTextFile fWorkspaceFile(ws_path);
	if(fWorkspaceFile.Exists()) 
	{
		fWorkspaceFile.Open();
		fWorkspaceFile.Clear();
	}
	else fWorkspaceFile.Create();

	wxString strLine;
	if(!m_sDescription.IsEmpty())
	{
		m_sDescription.Replace(_("\n"),_("<Return>"));
		strLine.Printf(wxT("%s=%s"),WS_DESCRIPT,m_sDescription);
		fWorkspaceFile.AddLine(strLine);
	}
	if(!m_sAuthor.IsEmpty())
	{
		strLine.Printf(wxT("%s=%s"),WS_AUTHOR,m_sAuthor);
		fWorkspaceFile.AddLine(strLine);
	}
	if(!m_sWorkspaceVersion.IsEmpty())
	{
		strLine.Printf(wxT("%s=%s"),WS_VERSION,m_sWorkspaceVersion);
		fWorkspaceFile.AddLine(strLine);
	}
	
	for(size_t i=0;i<m_ProjectArray.GetCount();i++)
	{
		wxString prj_nm = m_ProjectArray[i]->GetProjectName();
		strLine.Printf(wxT("%s=%s\\%s.flp"),WS_PROJFILE,prj_nm,prj_nm);
		fWorkspaceFile.AddLine(strLine);
	}

	if(m_iActiveProjectIndex >= 0)
	{
		strLine.Printf(wxT("%s=%d"),WS_ACTIVEPROJ,m_iActiveProjectIndex);
		fWorkspaceFile.AddLine(strLine);
	}
	fWorkspaceFile.Write();
	fWorkspaceFile.Close();


	// Backup projects
	for(size_t i=0;i<m_ProjectArray.GetCount();i++)
	{
		if(!m_ProjectArray[i]->Backup(bk_dir)) break;
	}

	return true;

}
