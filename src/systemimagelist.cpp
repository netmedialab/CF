#include <wx/filename.h>
#include "systemimagelist.h"
#include "ResourceUtil.h"
#include "FileUtil.h"

CWxfiiSystemIconList* CWxfiiSystemIconList::m_pSysIconList32 = 0;
CfIconList* CfIconList::m_pIconList = 0;

void CWxfiiSystemIconList::Init32()
{
	m_pSysIconList32 = new CWxfiiSystemIconList(32);
}

void CWxfiiSystemIconList::Destroy32()
{
	if(m_pSysIconList32) 
	{
		delete m_pSysIconList32;
		m_pSysIconList32 = NULL;
	}
}

CWxfiiSystemIconList* CWxfiiSystemIconList::Get32()
{
	if(!m_pSysIconList32) Init32();
	return m_pSysIconList32;
}

CWxfiiSystemIconList::CWxfiiSystemIconList(): wxImageList()
{
}

CWxfiiSystemIconList::CWxfiiSystemIconList(int width, int height, const bool mask /*=true*/, int initialCount /*=1*/)
	: wxImageList(width, height, mask, initialCount)
{
}

CWxfiiSystemIconList::CWxfiiSystemIconList(int size)
{

	SHFILEINFO shFinfo;
	wxChar buffer[MAX_PATH + 10];
	if (!GetWindowsDirectory(buffer, MAX_PATH))
#ifdef _tcscpy
		_tcscpy(buffer, _T("C:\\"));
#else
		strcpy(buffer, _T("C:\\"));
#endif
		m_hImageList = (WXHIMAGELIST)SHGetFileInfo(buffer,0,&shFinfo,sizeof( shFinfo ),SHGFI_SYSICONINDEX |((size != 16) ? SHGFI_ICON : SHGFI_SMALLICON));
}

int CWxfiiSystemIconList::GetIconIndex(CfItemType type, const wxString& fileName /*=_T("")*/, bool physical /*=true*/)
{
	if (fileName == _T("")) physical = false;
	wxString fn = fileName;
	fn.Replace(_("\\\\"),_("\\"));

	SHFILEINFO shFinfo;
	memset(&shFinfo, 0, sizeof(SHFILEINFO));
	if (SHGetFileInfo(fn != _T("") ? fn : _T("{B97D3074-1830-4b4a-9D8A-17A38B074052}"),
		(type != PIT_FILE) ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL,
		&shFinfo,
		sizeof(SHFILEINFO),
		SHGFI_ICON | ((type == OPEN_DIR) ? SHGFI_OPENICON : 0) | ((physical) ? 0 : SHGFI_USEFILEATTRIBUTES) ) )
	{
		int icon = shFinfo.iIcon;
		// we only need the index from the system image list
		DestroyIcon(shFinfo.hIcon);
		return icon;
	}
	return -1;
}

void CfIconList::Init()
{
	if(!m_pIconList) m_pIconList = new CfIconList();
}

void CfIconList::Destroy()
{
	if(m_pIconList) delete m_pIconList;
	m_pIconList = 0;
}


CfIconList* CfIconList::Get()
{
	if(!m_pIconList) Init();
	return m_pIconList;
}

CfIconList::CfIconList()
{
	m_pSystemIconList = new CWxfiiSystemIconList(16);
	m_pCustomIconList = new wxImageList(16,16);	
	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(IMG_WORKSPACE));
	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(IMG_PROJECT));
	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(IMG_FOLDER));
	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(IMG_CLASSIFIER));

	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(FileTypeIconStr[FT_VERILOG]));
	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(FileTypeIconStr[FT_VHDL]));
	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(FileTypeIconStr[FT_CPP]));
	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(FileTypeIconStr[FT_C]));
	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(FileTypeIconStr[FT_HEADER]));
	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(FileTypeIconStr[FT_SHELL]));
	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(FileTypeIconStr[FT_TEXT]));
	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(FileTypeIconStr[FT_SYSVER]));
	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(FileTypeIconStr[FT_CHIP_SCHEMATIC]));
	m_pCustomIconList->Add(ResourceUtil::Get()->GetBitmap(FileTypeIconStr[FT_CHIP_SYMBOL]));

	m_ExtList.Add(_("flw"));
	m_ExtList.Add(_("flp"));
	m_ExtList.Add(_("fiiNULL"),2);
	m_ExtList.Add(_("v"));
	m_ExtList.Add(_("vhdl"));
	m_ExtList.Add(_("cpp"));
	m_ExtList.Add(_("c"));
	m_ExtList.Add(_("h"));
	m_ExtList.Add(_("sh"));
	m_ExtList.Add(_("txt"));
	m_ExtList.Add(_("sv"));
	m_ExtList.Add(_("cpsc"));
	m_ExtList.Add(_("cpsy"));

}

CfIconList::~CfIconList()
{
	if(m_pSystemIconList) delete m_pSystemIconList;
	if(m_pCustomIconList) delete m_pCustomIconList;
}

#ifndef __WXMSW__
// This function converts to the right size with the given background colour
wxBitmap PrepareIcon(wxIcon icon, wxSize size)
{
	if (icon.GetWidth() == size.GetWidth() && icon.GetHeight() == size.GetHeight())
		return icon;
	wxBitmap bmp;
	bmp.CopyFromIcon(icon);
	return bmp.ConvertToImage().Rescale(size.GetWidth(), size.GetHeight());
}
#endif

int CfIconList::GetIconIndex(CfItemType type, const wxString& fileName /*=_T("")*/, bool physical /*=true*/)
{
	if((type == PIT_WORKSPACE) || (type == PIT_RPOJECT) || (type == PIT_CLASSIFIER) || (type == PIT_FOLDER))
	{
		return type;
	}

	if (fileName == _T("")) physical = false;
	//wxString fn = fileName;
	//fn.Replace(_("\\\\"),_("\\"));

	int index = -1;
	wxFileName fn(fileName);
	wxString ext = fn.GetExt();

	// Add file name for exe file
	if(ext == _("exe"))
	{
		wxString name = fn.GetName();
		index  = m_ExtList.Index(name,false);
		if(index != wxNOT_FOUND) return index;

		int sys_index = m_pSystemIconList->GetIconIndex(type,fileName,physical);
		if(sys_index != -1)
		{
			index = m_pCustomIconList->Add(m_pSystemIconList->GetBitmap(sys_index));
			m_ExtList.Add(name);
		}
	}
	else
	{
		index  = m_ExtList.Index(ext,false);
		if(index != wxNOT_FOUND) 
			return index;


		int sys_index = m_pSystemIconList->GetIconIndex(type,fileName,physical);
		if(sys_index != -1)
		{
			index = m_pCustomIconList->Add(m_pSystemIconList->GetBitmap(sys_index));
			m_ExtList.Add(ext);
		}
	}
	return index;
}


//int CSystemImageList::GetIconIndex(CfItemType type, const wxString& fileName /*=_T("")*/, bool physical /*=true*/)
//{
//	if((type == PIT_WORKSPACE) || (type == PIT_RPOJECT) || (type == PIT_CLASSIFIER))
//	{

//		return type;
//	}
//
//	if (fileName == _T("")) physical = false;
//	wxString fn = fileName;
//	fn.Replace(_("\\\\"),_("\\"));
//
//	if ((type == PIT_FILE) || (type == PIT_EXTERN_FILE))
//	{
//		wxString ext = fileName.AfterLast('.');
//		int index = CfiiFileUtils::GetFileTypeIndex(ext);
//		if(index != FT_UNKNOWN) return index + 4;  // 4 item types
//		
//	}
//
//	SHFILEINFO shFinfo;
//	memset(&shFinfo, 0, sizeof(SHFILEINFO));
//	if (SHGetFileInfo(fn != _T("") ? fn : _T("{B97D3074-1830-4b4a-9D8A-17A38B074052}"),
//		(type != PIT_FILE) ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL,
//		&shFinfo,
//		sizeof(SHFILEINFO),
//		SHGFI_ICON | ((type == OPEN_DIR) ? SHGFI_OPENICON : 0) | ((physical) ? 0 : SHGFI_USEFILEATTRIBUTES) ) )
//	{
//		int icon = shFinfo.iIcon;
//		// we only need the index from the system image list
//		DestroyIcon(shFinfo.hIcon);
//		return icon + m_pImageList->m_pCustomizedImageList->GetImageCount();
//	}
//	return -1;
//}
