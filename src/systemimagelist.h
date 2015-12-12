#ifndef __SYSTEMIMAGELIST_H__
#define __SYSTEMIMAGELIST_H__

#include "CfWx.h"
#include "CfDefines.h"
#include "ResourceUtil.h"
#include "Project.h"


// Required wxImageList extension
class CWxfiiSystemIconList : public wxImageList
{
public:
	CWxfiiSystemIconList();
	CWxfiiSystemIconList(int width, int height, const bool mask = true, int initialCount = 1);
	virtual ~CWxfiiSystemIconList() { }

	CWxfiiSystemIconList(int size);
	int GetIconIndex(CfItemType type, const wxString& fileName = _T(""), bool physical = true);

	static void Init32();
	static void Destroy32();
	static CWxfiiSystemIconList* Get32();

protected:
	static CWxfiiSystemIconList *m_pSysIconList32;
};


class CfIconList
{
public:
	CfIconList();
	~CfIconList();
	wxImageList* GetIconList() { return m_pCustomIconList; }
	CWxfiiSystemIconList* GetSystemIconList() { return m_pSystemIconList; }
	int GetIconIndex(CfItemType type, const wxString& fileName = _T(""), bool physical = true);

	static void Init();
	static void Destroy();
	static CfIconList* Get();
protected:
	CWxfiiSystemIconList *m_pSystemIconList;
	wxImageList *m_pCustomIconList;
	wxArrayString m_ExtList;

	static CfIconList *m_pIconList;
};

#endif //__SYSTEMIMAGELIST_H__
