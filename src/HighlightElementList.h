#pragma once
#include "DrawingElementGroup.h"

class CfHighlightElementList
{
public:
	CfHighlightElementList(void);
	~CfHighlightElementList(void);
	static CfDrawingElementGroup HighlightElementList;
protected:
	void ClearHighlightElements();
	void SetHighlightElement(CfDrawingElement *element);	//clear first
	void AppendHighlightElement(CfDrawingElement *element);

};


