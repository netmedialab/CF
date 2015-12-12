#pragma once
#include "IAction.h"
#include <wx/stack.h>

class CfHistory
{
public:
	CfHistory(void);
	~CfHistory(void);
	
	void AddAction(IAction *action);
	IAction *Undo();
	IAction *Redo();
	bool CanUndo();
	bool CanRedo();
	//IAction *GetLastAction();

private:
	wxStack<IAction *> m_UndoStack;
	wxStack<IAction *> m_RedoStack;
	//IAction *m_pLastAction;
};

