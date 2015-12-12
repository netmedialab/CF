#include "History.h"


CfHistory::CfHistory(void)
{
}


CfHistory::~CfHistory(void)
{
	IAction * action;
	size_t size = m_UndoStack.size();
	for(size_t i=0;i<size;i++)
	{
		action = m_UndoStack.top();
		m_UndoStack.pop();
		delete action;
	}
	size = m_RedoStack.size();
	for(size_t i=0;i<size;i++)
	{
		action = m_RedoStack.top();
		m_RedoStack.pop();
		delete action;
	}
}

void CfHistory::AddAction(IAction *action)
{
	m_UndoStack.push(action);
}
IAction *CfHistory::Undo()
{
	if(m_UndoStack.size()==0) return NULL;
	IAction *action = m_UndoStack.top();
	action->Undo();
	m_UndoStack.pop();
	m_RedoStack.push(action);
	return action;
}
IAction *CfHistory::Redo()
{
	if(m_RedoStack.size()==0) return NULL;
	IAction *action = m_RedoStack.top();
	action->Redo();
	m_RedoStack.pop();
	m_UndoStack.push(action);
	return action;
}

bool CfHistory::CanUndo()
{
	return m_UndoStack.size()!=0;
}
bool CfHistory::CanRedo()
{
	return m_RedoStack.size()!=0;
}