
/////////////////////////////////////////////////////////////////////////////////
//
// Name        : Grid.h
// Author      : MING
// Created     : 2015Äê10ÔÂ5ÈÕ
// Description : 
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "IElement.h"


class CfGrid: public IElement 
{
public:
	CfGrid(bool show);
	~CfGrid(void);

	//IDrawable
	virtual void Draw(wxDC& dc,const CfScreenView &view);
};


