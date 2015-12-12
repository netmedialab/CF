#include "IAction.h"


IAction::IAction(CfDrawingElement* element)
{
	m_ElementGroup.Add(element);
}

IAction::IAction(CfDrawingElementGroup* element_group)
{
	m_ElementGroup.Add(element_group);
}

IAction::~IAction(void)
{
}

void IAction::Run()
{
	//wxLogMessage(_("[Action] %s %s"),GetActionName().c_str(), GetElementName().c_str());
	Redo();
}

CfDrawingElementGroup* IAction::GetElementGroup() 
{
	return &m_ElementGroup;
}
//wxString IAction::GetElementName() const
//{
//	if(m_pElement) return m_pElement->GetName();
//	else return wxEmptyString;
//}

//Move
CfMove::CfMove(CfDrawingElement* element, int dx, int dy)
	:IAction(element),m_iDx(dx),m_iDy(dy)
{
}

CfMove::CfMove(CfDrawingElementGroup* element_group, int dx, int dy)
	:IAction(element_group),m_iDx(dx),m_iDy(dy)
{
}

const wxString& CfMove::GetActionName() const
{
	return _("Move");
}

void CfMove::Redo()
{
	m_ElementGroup.Move(m_iDx,m_iDy);
}
void CfMove::Undo()
{
	m_ElementGroup.Move(-m_iDx,-m_iDy);
}

//Delete
CfDelete::CfDelete(CfDrawingElement *element)
	:IAction(element)
{
}
CfDelete::CfDelete(CfDrawingElementGroup *element_group)
	:IAction(element_group)
{
}
const wxString& CfDelete::GetActionName() const
{
	return _("Delete");
}

void CfDelete::Redo()
{
	m_ElementGroup.SetDeleted(true);
}
void CfDelete::Undo()
{
	m_ElementGroup.SetDeleted(false);
}

//Create
CfCreate::CfCreate(CfDrawingElement *element)
	:IAction(element)
{
}
CfCreate::CfCreate(CfDrawingElementGroup *element_group)
	:IAction(element_group)
{
}
const wxString& CfCreate::GetActionName() const
{
	return _("Create");
}

void CfCreate::Redo()
{
	m_ElementGroup.SetDeleted(false);
}
void CfCreate::Undo()
{
	m_ElementGroup.SetDeleted(true);
}