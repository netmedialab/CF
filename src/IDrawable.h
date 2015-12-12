#ifndef _CF_DRAWABLE_H
#define _CF_DRAWABLE_H
#include "CfWx.h"

class CfScreenView
{
public:
	CfScreenView(int offsetx, int offsety, int width, int height, float scale)
		:Rectangle(offsetx, offsety, width, height), Scale(scale){};

	~CfScreenView(){};
	
	bool Contains(wxPoint &point) const
	{
		return Rectangle.Contains(point.x*Scale,point.y*Scale);
	};
	wxRect Rectangle;
	float Scale;
};

class IDrawable
{
public:
	IDrawable(){};
	virtual ~IDrawable() {};
	virtual void Draw(wxDC& dc,const CfScreenView &view)=0;
	virtual void Move(int dx, int dy)=0;
	virtual bool Contains(wxPoint &point)=0;
	virtual wxPoint GetStartPos()=0;
	virtual wxPoint GetEndPos()=0;
	virtual wxSize GetSize()=0;

};

WX_DEFINE_ARRAY_PTR(IDrawable*, CfIDrawableArray);


#endif //_CF_DRAWABLE_H