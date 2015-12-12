
/////////////////////////////////////////////////////////////////////////////////
//
// Name        : IAction.h
// Author      : MING
// Created     : 2015Äê8ÔÂ19ÈÕ
// Description : The inferface for all action of chip schematic operation.
// Copyright   : NetMedia Lab.
//
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "DrawingElement.h"
#include "DrawingElementGroup.h"

enum
{
	cfId_Action_Move = 0
	,cfId_Action_Delete
	,cfId_Action_Create

};

class IAction
{
public:
	IAction(CfDrawingElement* element);
	IAction(CfDrawingElementGroup *element_group);
	~IAction(void);

	virtual const wxString& GetActionName() const =0;
	virtual void Undo()=0;
	virtual void Redo()=0;
	void Run();
	CfDrawingElementGroup* GetElementGroup();
	//wxString GetElementName() const;

protected:
	CfDrawingElementGroup m_ElementGroup;
	//CfDrawingElement* m_pElement;


};

class CfMove : public IAction
{
public:
	CfMove(CfDrawingElement* element, int dx, int dy);
	CfMove(CfDrawingElementGroup *element_group, int dx,int dy);

	virtual void Undo();
	virtual void Redo();
	virtual const wxString& GetActionName() const;

private:
	int m_iDx;
	int m_iDy;
};

class CfDelete: public IAction
{
public:
	CfDelete(CfDrawingElement* element);
	CfDelete(CfDrawingElementGroup *element_group);

	virtual void Undo();
	virtual void Redo();
	virtual const wxString& GetActionName() const;
};

class CfCreate: public IAction
{
public:
	CfCreate(CfDrawingElement* element);
	CfCreate(CfDrawingElementGroup *element_group);

	virtual void Undo();
	virtual void Redo();
	virtual const wxString& GetActionName() const;
};
