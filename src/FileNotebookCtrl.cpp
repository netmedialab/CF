#include <wx/filename.h>
#include "FileNotebookCtrl.h"
#include "FileEditorBase.h"
#include "TextEditorCtrl.h"
#include "ImageEditorCtrl.h"
#include "ChipSchematicEditorCtrl.h"
#include "ChipSymbolEditorCtrl.h"

CfFileNotebookCtrl::CfFileNotebookCtrl(wxWindow* parent,
                  wxWindowID id,const wxPoint& pos,const wxSize& size,
                  long style)
	:wxAuiNotebook(parent,id,pos,size,style)
,CfNotificationSender(parent)
{
}

CfFileNotebookCtrl::~CfFileNotebookCtrl()
{

}
	
CfFileEditorBase* CfFileNotebookCtrl::AddEditor(const wxString &file_path,CfProject* project)
{
	CfFileEditorBase *p_reader = CreateFileReader(GetParent(), file_path,project);
	if(p_reader) 
	{
		AddEditor(p_reader,wxFileName::FileName(file_path).GetFullName());
	}
	return p_reader;

}


CfFileEditorBase* CfFileNotebookCtrl::GetEditor(int index)
{
	CfFileEditorBase* editor = NULL; 
	wxWindow *pwin;
	if(this->GetPageCount()==0) return editor;

	if(index<0)		//get current page
		pwin = (wxWindow *)this->GetCurrentPage();
	else
		pwin = (wxWindow *)this->GetPage(index);
	if(!pwin) return editor;

	wxString class_name = typeid(*pwin).name();
	if(class_name == _("class CfTextEditorCtrl"))
		editor = dynamic_cast<CfTextEditorCtrl*>(pwin);
	else if(class_name == _("class CfImageEditorCtrl"))
		editor = dynamic_cast<CfImageEditorCtrl*>(pwin);
	else if(class_name == _("class CfChipSchematicEditorCtrl"))
		editor = dynamic_cast<CfChipSchematicEditorCtrl*>(pwin);
	else if(class_name == _("class CfChipSymbolEditorCtrl"))
		editor = dynamic_cast<CfChipSymbolEditorCtrl*>(pwin);
	return editor;
}

void CfFileNotebookCtrl::AddEditor(CfFileEditorBase *pFileReader, const wxString &title)
{
	//check if the reader already exists
	int idx = wxNOT_FOUND;
	switch(pFileReader->GetReaderId())
	{
	case FR_TEXT_EDITOR:
		idx = GetPageIndex((CfTextEditorCtrl*)pFileReader);
		break;
	case FR_IMAGE_EDITOR:
		idx = GetPageIndex((CfImageEditorCtrl*)pFileReader);
		break;
	case FR_CHIP_SCHEMATIC_EDITOR:
		idx = GetPageIndex((CfChipSchematicEditorCtrl*)pFileReader);
		break;
	case FR_CHIP_SYMBOL_EDITOR:
		idx = GetPageIndex((CfChipSymbolEditorCtrl*)pFileReader);
		break;
	}

	//if the reader exists, show it and return
	if(idx != wxNOT_FOUND)
	{
		SetSelection(idx);
		return;
	}

	//if the reader doesn't exist, add to notebook
	switch(pFileReader->GetReaderId())
	{
	case FR_TEXT_EDITOR:
		AddPage((CfTextEditorCtrl *)pFileReader,title,true);
		break;
	case FR_IMAGE_EDITOR:
		AddPage((CfImageEditorCtrl*)pFileReader,title,true);
		break;
	case FR_CHIP_SCHEMATIC_EDITOR:
		AddPage((CfChipSchematicEditorCtrl*)pFileReader,title,true);
		break;
	case FR_CHIP_SYMBOL_EDITOR:
		AddPage((CfChipSymbolEditorCtrl*)pFileReader,title,true);
		break;
	case FR_EXTERNAL_READER:
		//Do not add external reader
		break;
	}
}

void CfFileNotebookCtrl::CloseAllFiles()
{
	int cnt = GetPageCount();
	for(int i=cnt-1;i>=0;i--)
	{
		CloseFile(i);
	}
}

void CfFileNotebookCtrl::CloseFile(int index, bool delete_page)
{

	//clear property view
	CfNotification *pNotification = new CfNotification(cfId_PropertyView_ShowElement);
	pNotification->SetElement(NULL);
	AddNotification(pNotification);

	//clear layer view
	CfNotification *notify= new CfNotification(cfId_LayerView_Update);
	notify->SetElement(NULL);
	AddNotification(notify);

	if(index == CURRENT_PAGE)
		index = this->GetSelection();
	CfFileEditorBase* editor = GetEditor(index);
	if(editor)
		RemoveReaderFromList(editor);
	if(delete_page)
		DeletePage(index);
}

void CfFileNotebookCtrl::EditorModified()
{
	int index = GetSelection();
	CfFileEditorBase* editor = GetEditor(index);
	if(!editor) return;
	wxASSERT(editor);
	SetPageTextModified(index,editor->Modified());

	//wxString text = GetPageText(index);
	//if(editor->Modified())
	//{
	//	if (!text.EndsWith(cfSTRING_STAR))
	//		SetPageText(index, text + cfSTRING_STAR);
	//}
	//else
	//{
	//	if (text.EndsWith(cfSTRING_STAR))
	//		SetPageText(index, text.BeforeLast(cfSTRING_STAR));
	//}
}

void CfFileNotebookCtrl::SetPageTextModified(int index, bool modified)
{
	wxString text = GetPageText(index);
	if(modified)
	{
		if (!text.EndsWith(cfSTRING_STAR))
			SetPageText(index, text + cfSTRING_STAR);
	}
	else
	{
		if (text.EndsWith(cfSTRING_STAR))
			SetPageText(index, text.BeforeLast(cfSTRING_STAR));
	}
}

void CfFileNotebookCtrl::RefreshEditor(wxString editor_class_name)
{
	wxWindow *pw = (wxWindow *)this->GetCurrentPage();
	CfChipEditorBaseCtrl* editor = NULL; 
	editor = dynamic_cast<CfChipEditorBaseCtrl*>(pw);
	editor->RefreshEditor();
	//pw->Refresh();
	return;

	//CfChipEditorBaseCtrl* editor = NULL; 
	//wxWindow *pwin;
	//int count = this->GetPageCount();
	//for(int i=0;i<count;i++)
	//{
	//	pwin = (wxWindow *)this->GetPage(i);
	//	wxString class_name = typeid(*pwin).name();
	//	if(class_name == editor_class_name)
	//	{
	//		editor = dynamic_cast<CfChipEditorBaseCtrl*>(pwin);
	//		editor->RefreshEditor();
	//	}
	//}
}

bool CfFileNotebookCtrl::IfNeedSave() //check if there are files modified and not saved
{
	int c_fPage=GetPageCount();
	if(c_fPage!=0)
	{
	    for(int i=0;i<c_fPage;i++)
		{
			CfFileEditorBase* editor = (CfFileEditorBase*)this->GetPage(i);
	        wxASSERT(editor);
	        if(editor->Modified()) return 1;
		}
		return 0;
	}
	return 0;
}
void CfFileNotebookCtrl::SaveCurrentFile()
{
	CfFileEditorBase* editor = GetEditor();
	editor->SaveFile();
	SetPageTextModified(GetSelection(),editor->Modified());
}

void CfFileNotebookCtrl::SaveAllFile()
{	
	CfFileEditorBase* editor = NULL; 
	int count = this->GetPageCount();
	for(int i=0;i<count;i++)
	{
		editor = GetEditor(i);
		editor->SaveFile();
		SetPageTextModified(i,editor->Modified());
	}
}