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
#include "CfDefines.h"
#include "FileUtil.h"
#include "Project.h"

CfProjectItem::CfProjectItem(wxString sPath,CfItemType iType,wxString sDepth,CfProject *project)
:CfiiFileItem(sPath,iType),Depth(sDepth),pChildList(NULL),pParent(NULL)
,ProjectPtr(project)
{
	if(Type != PIT_FILE) 
		pChildList = new CProjectItemList();
}

CfProjectItem::CfProjectItem(CfProjectItem *item)
:CfiiFileItem(item->Path,item->Type),Depth(item->Depth),pChildList(NULL),pParent(NULL)
,ProjectPtr(item->ProjectPtr)
{
	//new (this)CfProjectItem(item->Path,item->Type,item->Depth,item->ProjectPtr);
	if(Type != PIT_FILE) 
		pChildList = new CProjectItemList();
}


CfProjectItem::~CfProjectItem()
{
	if(Type != PIT_FILE) 
	{
		//Comment the following codes, because the items are freed by the project with m_lProjItemList
		//if(pChildList)
		//{
		//	CProjectItemList::iterator iter;
		//	for(iter = pChildList->begin();iter!=pChildList->end();iter++)
		//		delete (*iter);
		//}
		pChildList->clear();
		delete pChildList;
	}
}

bool CfProjectItem::AddChild(CfProjectItem *child)
{
	if(!pChildList || ChildExist(child)) return false;
	{
		child->pParent =  this;
		pChildList->push_back(child);
	}
	return true;
}

bool CfProjectItem::DelChild(CfProjectItem *item)
{
	bool done = false;
	if(pChildList)
	{
		CProjectItemList::iterator iter;
		for(iter = pChildList->begin();iter!=pChildList->end();iter++)
		{
			if(!(*(*iter) == *item)) continue;
			
			pChildList->erase(iter);
			//wxLogMessage(wxT("del success"));
			done = true;
			break;
		}
	}
	return done;
}

void CfProjectItem::Sort()
{
	if(pChildList)
	{
		int ptor;
		for(size_t i=0;i<pChildList->size();i++)
		{
			if((*pChildList)[i]->Type != PIT_FILE) (*pChildList)[i]->Sort();
			ptor = i;
			for(int j=i+1;j<pChildList->size();j++)
			{
				if((*(*pChildList)[j]) < (*(*pChildList)[ptor])) ptor = j;
			}
			CfProjectItem *temp_item = (*pChildList)[ptor];
			(*pChildList)[ptor] = (*pChildList)[i];
			(*pChildList)[i] = temp_item;
		}
	}
}

bool CfProjectItem::ChildExist(CfProjectItem *item)
{
	bool exist = false;
	if(pChildList)
	{
		CProjectItemList::iterator iter;
		for(iter = pChildList->begin();iter!=pChildList->end();iter++)
		{
			if(((*iter)->Path != item->Path) || ((*iter)->Type != item->Type)) continue;
			exist = true;
			break;
		}
	}
	return exist;
}

void CfProjectItem::DumpItem(wxTextFile* pro_file,wxString depth)
{
	wxFileName proj_fname(pro_file->GetName());
	wxString proj_fdir = proj_fname.GetPath();

	int cnt = 0;
	wxString sDepth;
	CProjectItemList::iterator iter;
	for(iter=pChildList->begin();iter!=pChildList->end();iter++)
	{
		sDepth.Printf(wxT("%s.%d"),depth.c_str(),cnt);
		wxString line(ItemTypeStr[(*iter)->Type]);
		wxString item_fpath = (*iter)->Path;
		if((*iter)->Type == PIT_FILE)
		{
			wxFileName item_fname(item_fpath);
			item_fname.MakeRelativeTo(proj_fdir);
			item_fpath = item_fname.GetFullPath();
		}
		line +=  wxT("=") + sDepth + wxT("=") + item_fpath;

		pro_file->AddLine(line);
		if((*iter)->Type != PIT_FILE)
		{
			(*iter)->DumpItem(pro_file,sDepth);
		}
		cnt++;
	}
}

CfProjectItem* CfProjectItem::FindItem(const wxString name, bool full_path)
{
	CfProjectItem* item = NULL;
	CProjectItemList::iterator iter;
	wxString file_name;
	for(iter=pChildList->begin();iter!=pChildList->end();iter++)
	{
		//if((*iter)->Type == PIT_FILE)
		{
			file_name = (*iter)->GetItemName();
			if(full_path)
			{
				if( file_name == name) 
				{
					item = (*iter);
					break;
				}
			}
			else
			{
				if(file_name.EndsWith(_("\\")+name))
				{
					item = (*iter);
					break;
				}
			}
		}
		//else 
		if((*iter)->Type != PIT_FILE)
		{
			item = (*iter)->FindItem(name);
			if(item) break;
		}
	}
	return item;
}

//<<MING_20110414
//
void CfProjectItem::BackupItem(wxTextFile* pro_file,wxString depth,wxString bk_path)
{
	int cnt = 0;
	wxString sDepth;
	CProjectItemList::iterator iter;
	for(iter=pChildList->begin();iter!=pChildList->end();iter++)
	{
		sDepth.Printf(wxT("%s.%d"),depth.c_str(),cnt);
		wxString line(ItemTypeStr[(*iter)->Type]);
		wxString item_fpath_org = (*iter)->Path;
		wxFileName item_fname(item_fpath_org);
		wxString item_name = item_fname.GetFullName();

		// Write to project file
		line +=  wxT("=") + sDepth + wxT("=") + item_name;
		pro_file->AddLine(line);
	
		if((*iter)->Type == PIT_FOLDER) 
		{
			wxString item_fpath_new;
			item_fpath_new.Printf(_("%s\\%s"),bk_path,item_name);
			//Create folder
			::wxMkDir(item_fpath_new); 
			//Backup child items
			(*iter)->BackupItem(pro_file,sDepth,item_fpath_new);
		}
		else if((*iter)->Type == PIT_CLASSIFIER)
		{
			//Backup child items
			(*iter)->BackupItem(pro_file,sDepth,bk_path);
		}
		else //PIT_FILE
		{
			//Copy file
			wxString item_fpath_new;
			item_fpath_new.Printf(_("%s\\%s"),bk_path,item_name);
			::wxCopyFile(item_fpath_org,item_fpath_new);
		}

		cnt++;
	}
}
//>>



CProjectItemList* CfProjectItem::GetChildList()
{
	return pChildList;
}

// To get the count of files, not include folders and classifiers
int CfProjectItem::GetFileCount()
{
	int count = 0;
	CProjectItemList::iterator iter;
	for(iter=pChildList->begin();iter!=pChildList->end();iter++)
	{
		if((*iter)->Type == PIT_FILE)	count++;
		else count += (*iter)->GetFileCount();
	}
	return count;
}

//CfProject::CfProject(wxWindow *parent, const wxString& project_file):m_pParentWin(parent),m_bChanged(true)
//{
//	LoadProject(project_file);
//}

CfProject::CfProject()
:m_pParentWin(NULL)
,m_bChanged(false)
,m_pProRootItem(NULL)
,m_sProjectPath(wxEmptyString)
{
}

CfProject::~CfProject(void)
{
	AskForSave();
	//Comment the following codes, because the items are freed by the tree in the Workspace Explorer 
	//CProjectItemList::iterator iter;
	//for(iter = m_lProjItemList.begin();iter!=m_lProjItemList.end();iter++)
	//	delete (*iter);
	m_lProjItemList.clear();	
}
void CfProject::AskForSave()
{
	if(m_bChanged)// | m_pFtpParamUnit->IsChanged())
	{
		wxMessageDialog dlg(m_pParentWin,wxT("Save the changes to project?"),PROJECT_CAPTION,wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
		int id = dlg.ShowModal();
		if(id == wxID_YES)
			Dump();
	}
}
void CfProject::Init()
{
	wxString pro_name = GetProjectName();
	m_pProRootItem = new CfProjectItem(m_sProjectPath,PIT_RPOJECT,wxT("0"),this);
	m_lProjItemList.push_back(m_pProRootItem);

}
bool CfProject::Load(const wxString& project_file)
{
	if(project_file.IsEmpty()) return false;


	wxTextFile	fProjectFile(project_file);
	if(!fProjectFile.Exists())
	{
		wxString msg;
		msg.Printf(wxT("Project file '%s' does not exist!"),project_file);
		wxLogError(msg);
		return false;
	}
	if(!fProjectFile.Open()) 
	{
		wxString msg;
		msg.Printf(wxT("Failed to open project file '%s'!"),project_file);
		wxLogError(msg);
		return false;
	}

	//m_sProjectPath = _tcsdup(project_file);
	m_sProjectPath = project_file;
	// get the dir path of project for changing relative path of item
	wxFileName proj_fname(project_file);
	wxString proj_fdir = proj_fname.GetPath();

	wxString strLine;
	wxString name, value;
	vector<CfProjectItem*> parent_vector;
	CfProjectItem	*parent_item = 0;
	
	// initial project, add root item
	Init(); 

	parent_vector.push_back(m_pProRootItem);
	parent_item = m_pProRootItem;


	int pos;
	for(strLine = fProjectFile.GetFirstLine();!fProjectFile.Eof();strLine = fProjectFile.GetNextLine())
	{
		pos = strLine.Find('=');
		if(pos != wxNOT_FOUND)
		{
			name = strLine.Left(pos);
			value = strLine.Right(strLine.Len() - pos - 1);

			if(name.Cmp(PROJ_TYPE) == 0)
				m_sProjectTypeString = value;
			else if(name.Cmp(PROJ_AUTHOR) == 0)
				m_sAuthor = value;
			else
			{
				CfItemType type = PIT_UNKNOWN;
				if (name.Cmp(ItemTypeStr[PIT_FILE]) == 0) type = PIT_FILE;
#ifdef USE_FOLDER
				else if (name.Cmp(ItemTypeStr[PIT_FOLDER]) == 0) type = PIT_FOLDER;
#else
				else if (name.Cmp(ItemTypeStr[PIT_FOLDER]) == 0) type = PIT_CLASSIFIER;
#endif
				else if (name.Cmp(ItemTypeStr[PIT_CLASSIFIER]) == 0) type = PIT_CLASSIFIER;
				
				if(type < 0) continue;
				wxString s_depth = value.BeforeFirst('=');
				wxString s_path= value.AfterFirst('=');
				
				// check the path is absolute or relative.
				// change to absolute if it is relative.

				wxString item_fpath;
				if(type == PIT_CLASSIFIER) item_fpath = s_path;
				else item_fpath = FileUtil::ChangeToAbsolutePath(s_path.c_str(),proj_fdir.c_str()); 

				CfProjectItem *ppi = new CfProjectItem(item_fpath,type,s_depth,this);
				m_lProjItemList.push_back(ppi);


				while(parent_item)
				{
					if(ppi->Depth.StartsWith(parent_item->Depth.c_str()))
					{
						parent_item->AddChild(ppi);
						break;
					}
					else
					{
						if(parent_vector.size() > 0)
						{
							parent_item = parent_vector.back();
							parent_vector.pop_back();
						}
						else break;
					}
				}

				if((type == PIT_FOLDER) || (type == PIT_CLASSIFIER))
				{
					if(parent_item)
						parent_vector.push_back(parent_item);
					parent_item = ppi;
				}
			}
		}

	
	}
	Sort();
	fProjectFile.Close();
	m_bChanged = false;
	return true;

}

void CfProject::Clear()
{
	if(m_bChanged)// | m_pFtpParamUnit->IsChanged())
	{
		wxMessageDialog dlg(m_pParentWin,wxT("Save the changes to project?"),PROJECT_CAPTION,wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
		int id = dlg.ShowModal();
		if(id == wxID_YES)
			Dump();
	}

	m_sProjectPath	= wxEmptyString;
	m_lProjItemList.clear();
	m_TreeNodeId = NULL;
}

void CfProject::Dump(wxString path)
{
	if(path.IsEmpty())
		path = m_sProjectPath;

	wxTextFile fProjectFile(path);
	if(fProjectFile.Exists()) 
	{
		fProjectFile.Open();
		fProjectFile.Clear();
	}
	else fProjectFile.Create();

	wxString strLine;
	
	if(!m_sProjectTypeString.IsEmpty())
	{
		strLine.Printf(wxT("%s=%s"),PROJ_TYPE,m_sProjectTypeString);
		fProjectFile.AddLine(strLine);
	}
	if(!m_sAuthor.IsEmpty())
	{
		strLine.Printf(wxT("%s=%s"),PROJ_AUTHOR,m_sAuthor);
		fProjectFile.AddLine(strLine);
	}

	if(m_pProRootItem)
		m_pProRootItem->DumpItem(&fProjectFile,wxT("0"));


	fProjectFile.Write();
	fProjectFile.Close();

	m_bChanged = false;
}

void CfProject::Sort()
{
	if(m_pProRootItem) m_pProRootItem->Sort();
}

bool CfProject::IsEmpty()
{

	return (//(m_sUserId.IsEmpty() || m_sKeyFilePath.IsEmpty()) && 
		(m_lProjItemList.size() < 1));
}

// To get the count of files, not include folders and classifiers
int CfProject::GetFileCount()
{
	int count = 0;
	if(m_pProRootItem) 
		count = m_pProRootItem->GetFileCount();
	return count;

}

wxString CfProject::GetProjectName()
{
	wxString project_name = m_sProjectPath.AfterLast('\\').BeforeLast('.');
	if(project_name.IsEmpty())
		project_name = m_sProjectPath.BeforeLast('.');
	return project_name;

}
wxString CfProject::GetDirectory()
{
	return m_sProjectPath.BeforeLast('\\');
}

wxString CfProject::GetProjectPath()
{
	return m_sProjectPath;
}

wxTreeItemId CfProject::GetTreeItemId()
{
	return m_TreeNodeId;
}

void CfProject::UnsetTreeItemId()
{
	//m_TreeNodeId.Unset();
	m_TreeNodeId = NULL;
}

wxString CfProject::GetProjectTypeString()
{
	return m_sProjectTypeString;
}

CProjectItemList* CfProject::GetProjItemList()
{
	return &m_lProjItemList;
}

CfProjectItem* CfProject::GetRootItem()
{
	return m_pProRootItem;
}

void CfProject::GetProjectFiles(wxArrayString &files)
{
	for(size_t i=0;i<m_lProjItemList.size();i++)
		files.Add(m_lProjItemList[i]->Path);
}

void CfProject::SetProjectType(wxString project_type)
{
	m_sProjectTypeString = project_type;
}
void CfProject::SetProjectPath(wxString project_path)
{
	m_sProjectPath = project_path;
	//m_bEmpty = false;
}

void CfProject::SetChanged(bool changed)
{
	m_bChanged = changed;
//	m_bEmpty = false;
}

wxString CfProject::GetAuthor()
{
	return m_sAuthor;
}
void CfProject::SetAuthor(wxString name)
{
	m_sAuthor = name;
}


void CfProject::SetWorkspaceExplorerItemId(wxTreeItemId tid)
{
	m_TreeNodeId = tid;
}
CfProjectItem* CfProject::AddProjectItem(CfProjectItem *parent,wxString name,CfItemType type)
{
	int cnt = parent->pChildList->size();
	wxString depth;
	depth << parent->Depth << '.' << cnt;
	CfProjectItem *item = new CfProjectItem(name,type,depth,this);
	if(parent->AddChild(item))
	{
		m_lProjItemList.push_back(item);
		m_bChanged = true;
		return item;
	}
	else 
	{
		delete item;
		return NULL;
	}
}

bool CfProject::RenameFolder(CfProjectItem *item, wxString new_name)
{
	CfItemType type = item->Type;
	wxASSERT((type==PIT_FOLDER)|| (type==PIT_CLASSIFIER) || (type==OPEN_DIR));

	CfProjectItem *parent = item->pParent;
	CProjectItemList::iterator iter;
	for(iter = m_lProjItemList.begin();iter!=m_lProjItemList.end();iter++)
	{
		if(((*iter)->GetItemName() == new_name) && ((*iter)->Type == item->Type)) 
		{
			wxLogError(wxT("Same name exists!"));
			return false;
		}
	}

	wxString new_path = item->GetDirectory()+_("\\")+new_name;

	if(item->Type==PIT_FOLDER)
	{
		if(!::wxRenameFile(item->Path,new_path))
		{
			wxLogError(wxT("[Project] Failed to rename folder '%s' to '%s'!"),item->Path,new_path);
			return false;
		}
	}

	wxLogError(wxT("[Project] Rename '%s' to '%s'."),item->GetItemName(),new_name);
	item->Path = new_path;
	m_bChanged = true;	
	return true;
}

void CfProject::DelProjectItem(CfProjectItem *item)
{
	CfProjectItem *parent = item->pParent;
	CProjectItemList::iterator iter;
	for(iter = m_lProjItemList.begin();iter!=m_lProjItemList.end();iter++)
	{
		if(!(*(*iter) == *item)) continue;
		m_lProjItemList.erase(iter);
		break;
	}

	if(parent && parent->DelChild(item))
	{
		m_bChanged = true;	
		
	}
	else
		wxLogError(wxT("failed to del"));

}

CfProjectItem* CfProject::FindItem(const wxString name)
{
	wxASSERT(m_pProRootItem);
	return m_pProRootItem->FindItem(name);

}

bool CfProject::Backup(wxString bk_dir)
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

	//Check project directory
	wxString prj_dir;
	wxString prj_nm = GetProjectName();
	prj_dir.Printf(_("%s\\%s"),bk_dir,prj_nm);
	if(!wxDir::Exists(prj_dir))
	//{
	//	if (wxMessageBox(_("Project diretory exists! Overwrite?"), PROJECT_CAPTION, wxICON_QUESTION | wxYES_NO, NULL) != wxYES)
	//		return false;
	//}
	//else
		wxMkdir(prj_dir);

	//Dump project file
	wxString prj_path;
	prj_path.Printf(_("%s\\%s.flp"),prj_dir,prj_nm);

	wxTextFile fProjectFile(prj_path);
	if(fProjectFile.Exists()) 
	{
		fProjectFile.Open();
		fProjectFile.Clear();
	}
	else fProjectFile.Create();

	wxString strLine;
	
	if(!m_sProjectTypeString.IsEmpty())
	{
		strLine.Printf(wxT("%s=%s"),PROJ_TYPE,m_sProjectTypeString);
		fProjectFile.AddLine(strLine);
	}
	if(!m_sAuthor.IsEmpty())
	{
		strLine.Printf(wxT("%s=%s"),PROJ_AUTHOR,m_sAuthor);
		fProjectFile.AddLine(strLine);
	}

	if(m_pProRootItem)
		m_pProRootItem->BackupItem(&fProjectFile,wxT("0"),prj_dir);


	fProjectFile.Write();
	fProjectFile.Close();

	//Backup project items
	

	return true;
}
