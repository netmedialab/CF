#include "HighlightElementList.h"

CfDrawingElementGroup CfHighlightElementList::HighlightElementList = CfDrawingElementGroup();
CfHighlightElementList::CfHighlightElementList(void)
{
}


CfHighlightElementList::~CfHighlightElementList(void)
{
}
void CfHighlightElementList::ClearHighlightElements()
{
	for(int i=0;i<HighlightElementList.Count();i++)
	{
		HighlightElementList[i]->SetHighlighted(false);
	}
	HighlightElementList.Clear();
}

void CfHighlightElementList::SetHighlightElement(CfDrawingElement *element)	//clear first
{
	ClearHighlightElements();
	AppendHighlightElement(element);
}

void CfHighlightElementList::AppendHighlightElement(CfDrawingElement *element)
{
	element->SetHighlighted();
	HighlightElementList.Add(element);
}