#include "Grid.h"
#include "CfDefines.h"
#include "CfLogMessageCtrl.h"
#include "EnvironmentUtil.h"

CfGrid::CfGrid(bool show)
{
	SetShowing(show);
}
CfGrid::~CfGrid()
{
}
void CfGrid::Draw(wxDC& dc,const CfScreenView &view)
{
	if(!m_bShowing) return;
	int precision = CfEnvironmentUtil::Get()->GetInt(PROP_SETTING_GRID_SIZE,1)*10;
	int unit = precision*view.Scale;

	int x = (view.Rectangle.x%unit);
	int y = (view.Rectangle.y%unit);
	dc.SetPen(*wxGREY_PEN);

	wxString msg = wxString::Format(_("Rectangle [%d,%d,%d,%d], shift [%d,%d]"),view.Rectangle.x,view.Rectangle.y,
		view.Rectangle.width,view.Rectangle.height,x,y);
	wxLogDebug(msg);

	for(int i=view.Rectangle.x-x;i<view.Rectangle.x+view.Rectangle.width;i+=unit)
	{
		dc.DrawLine(i,view.Rectangle.y,i,view.Rectangle.y+view.Rectangle.height);
	}
	for(int i=view.Rectangle.y-y;i<view.Rectangle.y+view.Rectangle.height;i+=unit)
	{
		dc.DrawLine(view.Rectangle.x,i,view.Rectangle.x+view.Rectangle.width,i);
	}
}