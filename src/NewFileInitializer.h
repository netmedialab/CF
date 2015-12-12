/////////////////////////////////////////////////////////////////////////////////
//
// Name        : CfNewFileInitializer.h
// Author      : MING
// Created     : 2015��10��17��
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

