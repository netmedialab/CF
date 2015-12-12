
#ifndef _CF_ELEMENT_H
#define _CF_ELEMENT_H
#include "CfWx.h"
#include "Shape.h"


class IElement
{
public:
	IElement():m_bShowing(true){};
	virtual ~IElement(){};
	virtual bool IsShowing(){return m_bShowing;};
	virtual void SetShowing(bool showing){m_bShowing = showing;};
	virtual void Draw(wxDC& dc,const CfScreenView &view)=0;

protected:
	bool m_bShowing;
};




#endif //_CF_ELEMENT_H