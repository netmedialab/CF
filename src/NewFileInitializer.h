/////////////////////////////////////////////////////////////////////////////////
//
// Name        : CfNewFileInitializer.h
// Author      : MING
// Created     : 2015Äê10ÔÂ17ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "CfWx.h"


class CfNewFileInitializer
{
public:
	CfNewFileInitializer(wxString path);
	~CfNewFileInitializer(void);
	void InitNewFile();
private:
	void NewChipSchematicFile();
	void NewChipSymbolFile();
	wxString m_sFilePath;
};

