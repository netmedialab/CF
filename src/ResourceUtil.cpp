/////////////////////////////////////////////////////////////////////////////////
//
// Name        :Resource.cpp
// Author      :MING
// Created     :2011-04-19 ¿ÀÈÄ 5:03:21
// Description :
// Copyright   :(C) 2009-2010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#include "ResourceUtil.h"
#include "FileUtil.h"

ResourceUtil *ResourceUtil::m_pResource = 0;

ResourceUtil::ResourceUtil(void)
{
	wxImage::AddHandler( new wxPNGHandler );
	wxImage::AddHandler( new wxICOHandler );
	wxImage::AddHandler( new wxJPEGHandler );

	FileUtil fu;
	m_sResDir = fu.GetDirectory(ID_RESDIR);
}

ResourceUtil::~ResourceUtil(void)
{
}

ResourceUtil* ResourceUtil::Get()
{
	if(!m_pResource) Init();
	return m_pResource;
}

void ResourceUtil::Init()
{
	if(!m_pResource)
		m_pResource = new ResourceUtil();
}
void ResourceUtil::Destroy()
{
	if(!m_pResource) return;
	delete m_pResource;
	m_pResource = 0;
	
}


wxBitmap ResourceUtil::GetBitmap(wxString name)
{
	wxBitmap bm;
	if(!bm.LoadFile(m_sResDir+name,wxBITMAP_TYPE_PNG))
		wxLogError(_T("Couldn't load image from '%s'."), name.c_str());
	return bm;
}

wxIcon ResourceUtil::GetIcon(wxString name)
{
	wxIcon icon;
	if(!icon.LoadFile(m_sResDir+name,wxBITMAP_TYPE_ICO))
		wxLogError(_T("Couldn't load image from '%s'."), name.c_str());
	return icon;
}
