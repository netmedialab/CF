
/////////////////////////////////////////////////////////////////////////////////
//
// Name        : CfNewFileInitializer.cpp
// Author      : MING
// Created     : 2015Äê10ÔÂ17ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////
#include <wx/filename.h>
#include "NewFileInitializer.h"
#include "FileUtil.h"
#include "Panel.h"
#include "Symbol.h"
#include "EnvironmentUtil.h"
#include "DrawingElementFactory.h"
#include "SolidColorBackground.h"


CfNewFileInitializer::CfNewFileInitializer(wxString path)
	:m_sFilePath(path)
{


}


CfNewFileInitializer::~CfNewFileInitializer(void)
{
}

void CfNewFileInitializer::InitNewFile()
{
	wxFileName fn(m_sFilePath);
	wxString ext = fn.GetExt();
	if(ext == F_EXT_CHIP_SCHEMATIC)
	{
		NewChipSchematicFile();
	}
	if(ext == F_EXT_CHIP_SYMBOL)
	{
		NewChipSymbolFile();
	}
	else if(ext == F_EXT_CHIP_IMAGE)
	{
	}
}

void CfNewFileInitializer::NewChipSchematicFile()
{
	CfPanel panel(NULL);
	wxArrayString layer_list;
	CfEnvironmentUtil::Get()->GetStringList(PROP_SETTING_DEFAULTLAYER,&layer_list);
	for(int i=0;i<layer_list.Count();i++)
	{
		CfDrawingElement *bgd = new CfSolidColorBackground(NULL,*wxWHITE);
		CfLayer *layer = (CfLayer *)DoCreateDrawingElement(NULL,ID_ELEMENT_LAYER,NULL,wxEmptyString);
		layer->SetName(layer_list[i]);
		layer->SetBackground(bgd);
		panel.AddLayer(layer);
	}
	
	wxFileOutputStream fos(m_sFilePath);
	panel.Dump(&fos);
	fos.Close();
}

void CfNewFileInitializer::NewChipSymbolFile()
{
	CfRectangle *rect = new CfRectangle(10,10,200,200);
	CfSymbol *symbol= (CfSymbol*)DoCreateDrawingElement(NULL,ID_ELEMENT_SYMBOL,rect,wxEmptyString);
	
	wxFileOutputStream fos(m_sFilePath);
	symbol->Dump(&fos);
	fos.Close();
	delete symbol;
}