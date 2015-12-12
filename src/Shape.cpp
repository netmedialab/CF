#include "CfDefines.h"
#include "EnvironmentUtil.h"
#include "Shape.h"
#include "CfLogMessageCtrl.h"
#include <math.h>
#include <vector>
using namespace std;

//CfShape
CfShape::CfShape()
{
}

bool CfShape::Within(wxRect &rect)
{
	if((rect.GetLeftTop() == GetStartPos()) && (rect.GetRightBottom() == GetEndPos()))	// same shape
		return false;
	return rect.Contains(GetStartPos())&&rect.Contains(GetEndPos());
}
// CfRectangle
CfRectangle::CfRectangle()
{
}
CfRectangle::CfRectangle(int x, int y, int width, int height)
	:AbsoluteRect(x,y,width,height)
{
	//FilledColor.Set(CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_FILLEDCOLOR,_("None")));
	//LineColor.Set(CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_LINECOLOR,_("Blue")));
	LineWidth = CfEnvironmentUtil::Get()->GetInt(ENV_DRAWING_LINEWIDTH,3);
	//FilledColor(filledColor),BorderColor(boundaryColor),LineWidth(lineWidth)
}

CfRectangle::CfRectangle(CfRectangle *rectangle)
	:AbsoluteRect(rectangle->AbsoluteRect)
	,FilledColor(rectangle->FilledColor)
	,LineColor(rectangle->LineColor)
	,LineWidth(rectangle->LineWidth)
{

}

CfRectangle::~CfRectangle()
{
}


void CfRectangle::Draw(wxDC& dc,const CfScreenView &view,bool highlight)
{
	//1. Check if the element in the screen view
	if(!view.Contains(AbsoluteRect.GetLeftTop()) && !view.Contains(AbsoluteRect.GetRightBottom())
		&& !view.Contains(AbsoluteRect.GetLeftBottom()) && !view.Contains(AbsoluteRect.GetRightTop()))
		return;

	//2. Scale the element to current scale
	wxRect rect(AbsoluteRect.GetLeft()*view.Scale,AbsoluteRect.GetTop()*view.Scale, 
		AbsoluteRect.GetWidth()*view.Scale,AbsoluteRect.GetHeight()*view.Scale);


	//3. Subtract the offset of screen
	//rect.Offset(view.Rectangle.GetLeft(),view.Rectangle.GetTop());
	//rect.Offset(view.Rectangle.GetLeftTop());

	//4. Draw the rectangle
	if(highlight)

		dc.SetPen(wxPen(cfColorList[ID_COLOR_HIGHLIGHT],LineWidth));
	else
		dc.SetPen(wxPen(LineColor,LineWidth));
	//if(FilledColor == wxNullColour)
		dc.SetBrush(wxBrush(*wxRED,wxTRANSPARENT));
	//else
		//dc.SetBrush(wxBrush(FilledColor));
	dc.DrawRectangle(rect);
}

void CfRectangle::Move(int dx, int dy)
{
	AbsoluteRect.Offset(dx,dy);
}

bool CfRectangle::Contains(wxPoint &point)
{
	return AbsoluteRect.Contains(point);
}

CfShape* CfRectangle::Clone()
{
	return new CfRectangle(this);
}

CfRectangle& CfRectangle::operator = (const CfRectangle& rect)
{
	this->AbsoluteRect = rect.AbsoluteRect;
	this->LineColor = rect.LineColor;
	this->FilledColor = rect.FilledColor;
	this->LineWidth = rect.LineWidth;
	return *this;
}

void CfRectangle::SetStartPosition(wxPoint start_pt)
{
	AbsoluteRect.SetPosition(start_pt);
}


//IProperty
void CfRectangle::GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication)
{
	//PropertyUnit *punit;
	//punit = new PropertyUnit(cfSTRING_APPEARANCE,wxEmptyString,cfSTRING_GROUP);
	//properties->Add(punit);

	//punit = new PropertyUnit(cfSTRING_LINE_COLOR,LineColor,cfGRID_VALUE_CHOICE,false,cfColorStringList,WXSIZEOF(cfColorStringList)-1);
	//IProperty::AddProperty(properties,punit,check_name_duplication);
	//punit = new PropertyUnit(cfSTRING_FILLED_COLOR,FilledColor,cfGRID_VALUE_CHOICE,false,cfColorStringList,WXSIZEOF(cfColorStringList));
	//IProperty::AddProperty(properties,punit,check_name_duplication);
	//punit = new PropertyUnit(cfSTRING_LINE_WIDTH,wxString::Format(_("%d"),LineWidth),cfGRID_VALUE_NUMBER);
	//IProperty::AddProperty(properties,punit,check_name_duplication);
}

void CfRectangle::SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties)
{
	//PropertyUnit *punit;
	//for(size_t i=0;i<properties->Count();i++)
	//{
	//	punit = (*properties)[i];
	//	int value = wxAtoi(punit->Value);
	//	if(punit->Name == cfSTRING_X_POS) AbsoluteRect.x = value;
	//	else if(punit->Name == cfSTRING_Y_POS) AbsoluteRect.y = value;
	//	else if(punit->Name == cfSTRING_WIDTH) AbsoluteRect.width = value;
	//	else if(punit->Name == cfSTRING_HEIGHT) AbsoluteRect.height = value;
	//	else if(punit->Name == cfSTRING_LINE_COLOR) LineColor = punit->Value;
	//	else if(punit->Name == cfSTRING_FILLED_COLOR) FilledColor = punit->Value;
	//	else if(punit->Name == cfSTRING_LINE_WIDTH) LineWidth = value;
	//}
}

void CfRectangle::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_RECTANGLE);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_X_POS,AbsoluteRect.x+offset.x);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_Y_POS,AbsoluteRect.y+offset.y);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_WIDTH,AbsoluteRect.width);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_HEIGHT,AbsoluteRect.height);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_LINE_COLOR,LineColor.GetAsString());
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_FILLED_COLOR,FilledColor.GetAsString());
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_LINE_WIDTH,LineWidth);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_RECTANGLE);
	fos->Write(line.char_str(),line.size());


}
void CfRectangle::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line;// = tis.ReadLine().Trim().Trim(true);
	//if(line == wxString::Format(_("[%s]"),cfSTRING_RECTANGLE))
	{
		while (fis->CanRead())
		{
			line = tis.ReadLine().Trim().Trim(true);
			if(line == wxString::Format(_("[/%s]"),cfSTRING_RECTANGLE))
				break;
			name = line.Before('=');
			value = line.After('=');
			if(name == cfSTRING_X_POS) AbsoluteRect.x = wxAtoi(value);
			else if(name == cfSTRING_Y_POS) AbsoluteRect.y = wxAtoi(value);
			else if(name == cfSTRING_WIDTH) AbsoluteRect.width = wxAtoi(value);
			else if(name == cfSTRING_HEIGHT) AbsoluteRect.height = wxAtoi(value);
			else if(name == cfSTRING_LINE_COLOR) LineColor.Set(value);
			else if(name == cfSTRING_FILLED_COLOR) FilledColor.Set(value);
			else if(name == cfSTRING_LINE_WIDTH) LineWidth = wxAtoi(value);
		}
    }
}

void CfRectangle::SetLineColor(wxColor color)
{
	//LineColor = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_LINECOLOR, color.GetAsString());
	LineColor = color;
}

void CfRectangle::SetLineWidth(int width)
{
	LineWidth = CfEnvironmentUtil::Get()->GetInt(ENV_DRAWING_LINEWIDTH,width);
}

void CfRectangle::SetFilledColor(wxColor color)
{
	//FilledColor = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_FILLEDCOLOR,color.GetAsString());
	FilledColor = color;
}
//CfLineSegment
CfLineSegment::CfLineSegment()
{

}

CfLineSegment::CfLineSegment(int sx, int sy, int ex, int ey)
	:StartPoint(sx,sy),EndPoint(ex,ey)
{
	points.push_back(CoupledPoint(wxPoint(sx,sy),wxPoint(ex,ey)));

	//LineColor = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_LINECOLOR,_("Blue"));
	//LineWidth = CfEnvironmentUtil::Get()->GetInt(ENV_DRAWING_LINEWIDTH,2);
}

CfLineSegment::CfLineSegment(CfLineSegment* line)
	:StartPoint(line->StartPoint)
	,EndPoint(line->EndPoint)
	,LineColor(line->LineColor)
	,LineWidth(line->LineWidth)
	,points(line->points)
{
	
}
CfLineSegment::~CfLineSegment()
{

}

CfShape* CfLineSegment::Clone()
{
	return new CfLineSegment(this);
}

void CfLineSegment::Draw(wxDC& dc,const CfScreenView &view,bool highlight)
{
	//1. Scale and no need shift

	wxPoint sp = StartPoint*view.Scale;//-view.Rectangle.GetLeftTop();
	//wxPoint ep = EndPoint*view.Scale;//-view.Rectangle.GetLeftTop();

	//2. Draw line 
	if(highlight)
	{
		dc.SetPen(wxPen(cfColorList[ID_COLOR_HIGHLIGHT],LineWidth));
		dc.SetBrush(wxBrush(cfColorList[ID_COLOR_HIGHLIGHT]));
	}
	else
	{
		dc.SetPen(wxPen(LineColor,LineWidth));
		dc.SetBrush(wxBrush(LineColor));
	}
	for(size_t i = 0; i < points.size(); i++)
		dc.DrawLine(points[i].m_StartPoint * view.Scale, points[i].m_EndPoint * view.Scale);
	bool* need_s = new bool[points.size()];
	bool* need_e = new bool[points.size()];
	for(size_t i = 0; i < points.size(); i++)
	{
		need_s[i] = true;
		need_e[i] = true;
	}
	for(size_t i = 0; i < points.size(); i++)
	{
		if(need_s[i] || need_e[i])
		{
			for(size_t j = i + 1; j < points.size(); j++)
			{
				if(points[i].m_StartPoint == points[j].m_StartPoint) 
				{
					need_s[i] = false;
					need_s[j] = false;
				}
				if(points[i].m_StartPoint == points[j].m_EndPoint)
				{
					need_s[i] = false;
					need_e[j] = false;
				}
				if(points[i].m_EndPoint == points[j].m_StartPoint)
				{
					need_e[i] = false;
					need_s[j] = false;
				}
				if(points[i].m_EndPoint == points[j].m_EndPoint)
				{
					need_e[i] = false;
					need_e[j] = false;
				}
			}
		}
	}
	const int d = LineWidth * 1.5;
	for(size_t i = 0; i < points.size(); i++)
	{
		if(need_s[i])
		{
			wxRect rt((points[i].m_StartPoint.x - d) * view.Scale, (points[i].m_StartPoint.y - d) * view.Scale, 2 * d  * view.Scale, 2 * d * view.Scale);
			dc.DrawRectangle(rt);
		}
		if(need_e[i])
		{
			wxRect rt((points[i].m_EndPoint.x - d) * view.Scale, (points[i].m_EndPoint.y - d) * view.Scale, 2 * d * view.Scale, 2 * d * view.Scale);
			dc.DrawRectangle(rt);
		}
	}
	delete need_s;
	delete need_e;

}

void CfLineSegment::Move(int dx, int dy)
{
	StartPoint += wxPoint(dx,dy);
	EndPoint += wxPoint(dx,dy);
	for(size_t i = 0; i < points.size(); i++)
	{
		points[i].m_StartPoint += wxPoint(dx, dy);
		points[i].m_EndPoint += wxPoint(dx, dy);
	}
}

bool CfLineSegment::Contains(wxPoint &point)
{
	//double d1 = GetPointDistance(StartPoint,point);
	//double d2 = GetPointDistance(EndPoint,point);
	//double d3 = GetPointDistance(StartPoint,EndPoint);
	//return (d1+d2-d3 < 5);
	if(this->IsEndPoint(point))
		return true;
	wxRect rect;
	wxPoint top, buttom;
	bool is_contain = false;
	const int d = LineWidth * 1.5;
	for(size_t i = 0; i < points.size(); i++)
	{
		if(IsContained(points[i].m_StartPoint, points[i].m_EndPoint, d, point))
			is_contain =true;
	}
	return is_contain;
}
bool CfLineSegment::IsContained(wxPoint sp, wxPoint ep, int d, wxPoint point)
{
	int right_x = 0;
	int right_y = 0;
	double k;
	double b;
	wxPoint temp;
	if(sp.x == ep.x) //there is no k;
	{
		if(sp.y > ep.y)
		{
			temp = sp;
			sp = ep;
			ep = temp;
		}
		wxRect rt(sp.x - d, sp.y - d, 2 * d, ep.y - sp.y + 2 * d);
		return rt.Contains(point);
	}
	else
	{
		int x_max = sp.x;
		int x_min = sp.x;
		int y_max = sp.y;
		int y_min = sp.y;
		if(ep.x > x_max)
			x_max = ep.x;
		if(ep.x < x_min)
			x_min = ep.x;
		if(ep.y > y_max)
			y_max = ep.y;
		if(ep.y < y_min)
			y_min = ep.y;

		if(point.x < x_min || point.x > x_max || point .y < y_min || point.y > y_max)
			return false;

		k = double(ep.y - sp.y) / double(ep.x - sp.x);
		b = double(sp.y - k * double(sp.x));
		
		if(k < 0.5)
		{
			right_y = int(double(point.x) * k + b);
			if(point.y <= right_y + d && point.y >= right_y - d)
				return true;
			else 
				return false;
		 }
		 else
		 {
			 right_x = int((double(point.y) - b) / k);
			 if(point.x <= right_x + d && point.x >= right_x - d)
				 return true;
			else 
				return false;
		 }
	}


}
bool CfLineSegment::IsEndPoint(wxPoint &point)
{
	wxRect rect;
	const int d = LineWidth * 1.5;
	for(size_t i = 0; i < points.size(); i++)
	{
		rect = wxRect(points[i].m_StartPoint.x - d, points[i].m_StartPoint.y - d, 2 * d, 2 * d);
		if(rect.Contains(point))
		{
			point = points[i].m_StartPoint;
			return true;
		}
		rect = wxRect(points[i].m_EndPoint.x - d, points[i].m_EndPoint.y - d, 2 * d, 2 * d);
		if(rect.Contains(point))
		{
			point = points[i].m_EndPoint;
			return true;
		}
	}
	return false;
}
void CfLineSegment::Merge(CfShape * line)
{
	points.push_back(CoupledPoint(line->GetStartPos(),line->GetEndPos()));
	delete line;
	for(size_t i = 0; i < points.size(); i++)
	{
		for(size_t j = 0; j < points.size(); j++)
		{
			if(i == j)
				continue;
			if(	points[i].m_StartPoint == points[j].m_StartPoint || points[i].m_StartPoint == points[j].m_EndPoint ||
				points[i].m_EndPoint == points[j].m_StartPoint || points[i].m_EndPoint == points[j].m_EndPoint )
			{
				wxPoint pt[4] = {points[i].m_StartPoint, points[i].m_EndPoint, points[j].m_StartPoint, points[j].m_EndPoint};
				if(pt[0].x == pt[1].x == pt[2].x == pt[3].x)
				{
					points.erase(i);
					points.erase(j);
					wxPoint sp, ep;
					sp = pt[0];
					ep = pt[0];
					for(int i = 1;i < 4; i++)
					{
						if(pt[i].y < sp.y)
							sp = pt[i];
						if(pt[i].y > ep.y)
							ep = pt[i];
					}
					points.push_back(CoupledPoint(sp,ep));
				}
				if(pt[0].y == pt[1].y == pt[2].y == pt[3].y)
				{
					points.erase(i);
					points.erase(j);
					wxPoint sp, ep;
					sp = pt[0];
					ep = pt[0];
					for(int i = 1;i < 4; i++)
					{
						if(pt[i].x < sp.x)
							sp = pt[i];
						if(pt[i].x > ep.x)
							ep = pt[i];
					}
					points.push_back(CoupledPoint(sp,ep));
				}
			}
		}
	}
	wxPoint sp,ep;
	sp = points[0].m_StartPoint;
	ep = points[0].m_EndPoint;
	for(size_t i = 0; i < points.size(); i++)
	{
		//for sp
		if(points[i].m_StartPoint.x < sp.x)
			sp = points[i].m_StartPoint;
		if(points[i].m_StartPoint.x == sp.x && points[i].m_StartPoint.y < sp.y)
			sp = points[i].m_StartPoint;
		if(points[i].m_EndPoint.x < sp.x)
			sp = points[i].m_EndPoint;
		if(points[i].m_EndPoint.x == sp.x && points[i].m_EndPoint.y < sp.y)
			sp = points[i].m_EndPoint;

		//for ep
		if(points[i].m_StartPoint.x > ep.x)
			ep = points[i].m_StartPoint;
		if(points[i].m_StartPoint.x == ep.x && points[i].m_StartPoint.y > ep.y)
			ep = points[i].m_StartPoint;
		if(points[i].m_EndPoint.x > ep.x)
			ep = points[i].m_EndPoint;
		if(points[i].m_EndPoint.x == ep.x && points[i].m_EndPoint.y > ep.y)
			ep = points[i].m_EndPoint;
	}
	StartPoint = sp;
	EndPoint = ep;
}
double CfLineSegment::GetPointDistance(wxPoint p1, wxPoint p2)    
{   
	return sqrt((float)(p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));   
}   

//IProperty
void CfLineSegment::GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication)
{
	//PropertyUnit *punit;
	//punit = new PropertyUnit(cfSTRING_LINE_WIDTH,wxString::Format(_("%d"),LineWidth),cfGRID_VALUE_NUMBER);
	//if(!IProperty::AddProperty(properties,punit,check_name_duplication))
	//	delete punit;
	//punit = new PropertyUnit(cfSTRING_LINE_COLOR,LineColor,cfGRID_VALUE_CHOICE,false,cfColorStringList,WXSIZEOF(cfColorStringList)-1);
	//if(!IProperty::AddProperty(properties,punit,check_name_duplication))
	//	delete punit;
}

void CfLineSegment::SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties)
{
//	PropertyUnit *punit;
//	for(size_t i=0;i<properties->Count();i++)
//	{
//		punit = (*properties)[i];
//		int value = wxAtoi(punit->Value);
//		if(punit->Name == cfSTRING_START_X_POS) StartPoint.x = value;
//		else if(punit->Name == cfSTRING_START_Y_POS) StartPoint.y = value;
//		else if(punit->Name == cfSTRING_END_X_POS) EndPoint.x = value;
//		else if(punit->Name == cfSTRING_END_Y_POS) EndPoint.y = value;
//		else if(punit->Name == cfSTRING_LINE_COLOR) LineColor= punit->Value;
//		else if(punit->Name == cfSTRING_LINE_WIDTH) LineWidth = value;
//	}
}

void CfLineSegment::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_LINE_SEGMENT);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_START_X_POS,StartPoint.x+offset.x);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_START_Y_POS,StartPoint.y+offset.y);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_END_X_POS,EndPoint.x+offset.x);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_END_Y_POS,EndPoint.y+offset.y);
	fos->Write(line.char_str(),line.size());
	//real lines
	for(size_t i = 0; i < points.size(); i++)
	{
		line = wxString::Format(_("[%s]\n"),cfSTRING_LINE_REALLINE);
		fos->Write(line.char_str(),line.size());
		line = wxString::Format(_("%s=%d\n"),cfSTRING_START_X_POS,points[i].m_StartPoint.x+offset.x);
		fos->Write(line.char_str(),line.size());
		line = wxString::Format(_("%s=%d\n"),cfSTRING_START_Y_POS,points[i].m_StartPoint.y+offset.y);
		fos->Write(line.char_str(),line.size());
		line = wxString::Format(_("%s=%d\n"),cfSTRING_END_X_POS,points[i].m_EndPoint.x+offset.x);
		fos->Write(line.char_str(),line.size());
		line = wxString::Format(_("%s=%d\n"),cfSTRING_END_Y_POS,points[i].m_EndPoint.y+offset.y);
		fos->Write(line.char_str(),line.size());
		line = wxString::Format(_("[/%s]\n"),cfSTRING_LINE_REALLINE);
		fos->Write(line.char_str(),line.size());
	}
	line = wxString::Format(_("%s=%s\n"),cfSTRING_LINE_COLOR,LineColor.GetAsString());
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_LINE_WIDTH,LineWidth);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_LINE_SEGMENT);
	fos->Write(line.char_str(),line.size());


}
void CfLineSegment::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line;// = tis.ReadLine().Trim().Trim(true);
	//if(line == wxString::Format(_("[%s]"),cfSTRING_LINE_SEGMENT))
	{
		while (fis->CanRead())
		{
			line = tis.ReadLine().Trim().Trim(true);
			if(line == wxString::Format(_("[/%s]"),cfSTRING_LINE_SEGMENT))
				break;
			if(line == wxString::Format(_("[%s]"),cfSTRING_LINE_REALLINE))
			{
				wxPoint sp,ep;
				while (fis->CanRead())
				{
					line = tis.ReadLine().Trim().Trim(true);
					if(line == wxString::Format(_("[/%s]"),cfSTRING_LINE_REALLINE))
						break;
					name = line.Before('=');
					value = line.After('=');
					if(name == cfSTRING_START_X_POS) sp.x = wxAtoi(value);
					else if(name == cfSTRING_START_Y_POS) sp.y = wxAtoi(value);
					else if(name == cfSTRING_END_X_POS) ep.x = wxAtoi(value);
					else if(name == cfSTRING_END_Y_POS) 
					{
						ep.y = wxAtoi(value);
						this->points.push_back(CoupledPoint(sp,ep));
					}
				}
			}
			name = line.Before('=');
			value = line.After('=');
			if(name == cfSTRING_START_X_POS) StartPoint.x = wxAtoi(value);
			else if(name == cfSTRING_START_Y_POS) StartPoint.y = wxAtoi(value);
			else if(name == cfSTRING_END_X_POS) EndPoint.x = wxAtoi(value);
			else if(name == cfSTRING_END_Y_POS) EndPoint.y = wxAtoi(value);
			else if(name == cfSTRING_LINE_COLOR) LineColor.Set(value);
			else if(name == cfSTRING_LINE_WIDTH) LineWidth = wxAtoi(value);
		}
    }
}

void CfLineSegment::SetLineColor(wxColor color)
{
	//LineColor = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_LINECOLOR, color);
	LineColor = color;
}

void CfLineSegment::SetLineWidth(int width)
{
	LineWidth = CfEnvironmentUtil::Get()->GetInt(ENV_DRAWING_LINEWIDTH,width);
}
void CfLineSegment::SetChanged(int motion, int item)
{
	wxPoint pt;
	switch(item)
	{
	case Start_x:
	case End_x:
		pt = wxPoint(motion, 0);
		break;
	case Start_y:
	case End_y:
		pt = wxPoint(0, motion);
		break;
	}
	StartPoint += pt;
	EndPoint += pt;
	for(size_t i = 0; i < points.size();i++)
	{
		points[i].m_StartPoint += pt;
		points[i].m_EndPoint += pt;
	}
}
//---------------------------------------PortShape--------------------------------------------------//
CfPortShape::CfPortShape()
{
}

CfPortShape::CfPortShape(int x, int y, int width, int height):
m_BorderRect(x, y, width, height)
{
	//FilledColor = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_FILLEDCOLOR,_("None"));
	//LineColor = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_LINECOLOR,_("Blue"));
	LineWidth = CfEnvironmentUtil::Get()->GetInt(ENV_DRAWING_LINEWIDTH,3);
}

CfPortShape::~CfPortShape()
{
}

bool CfPortShape::Contains(wxPoint &point)
{
	return m_BorderRect.Contains(point);
}

void CfPortShape::SetLineColor(wxColor color)
{
	LineColor = color;
}

void CfPortShape::SetLineWidth(int width)
{
	LineWidth = width;
}

void CfPortShape::SetFilledColor(wxColor color)
{
	FilledColor = color;
}

//---------------------------------------InputPort---------------------------------------------------//
CfInputPort::CfInputPort()
{
}
CfInputPort::CfInputPort(int x, int y, int width, int height):
CfPortShape(x, y, width, height)
{
	//FilledColor = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_FILLEDCOLOR,_("None"));
	//LineColor = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_LINECOLOR,_("Blue"));
	InitPoints();
}

CfInputPort::CfInputPort(CfInputPort *port)
{
	FilledColor = port->FilledColor;
	LineColor = port->LineColor;
	LineWidth = port->LineWidth;
	m_BorderRect = port->m_BorderRect;
	InitPoints();
}

CfInputPort::~CfInputPort()
{
}

void CfInputPort::InitPoints()
{
	int x, y, width, height;
	x = this->m_BorderRect.GetLeftTop().x;
	y = this->m_BorderRect.GetLeftTop().y;
	width = this->m_BorderRect.GetSize().x;
	height= this->m_BorderRect.GetSize().y;
	
	double length_square;
	length_square = 15;
	//square left_top and right_buttom
	m_EndPoint[8] = wxPoint(x + width - length_square, y + (height-15) / 2);
	m_EndPoint[9] = wxPoint(m_EndPoint[8] .x + length_square, m_EndPoint[8].y + length_square);

	m_EndPoint[0] = wxPoint(m_EndPoint[8] .x, m_EndPoint[8].y + length_square / 4);
	m_EndPoint[1] = wxPoint(x , y);
	m_EndPoint[2] = wxPoint(m_EndPoint[8] .x + length_square / 2, y);
	m_EndPoint[3] = wxPoint(m_EndPoint[8] .x + length_square / 2,  m_EndPoint[8].y);
	 
	m_EndPoint[4] = wxPoint(m_EndPoint[8] .x, m_EndPoint[8].y + length_square * 3 / 4);
	m_EndPoint[5] = wxPoint(x, y + height);
	m_EndPoint[6] = wxPoint(m_EndPoint[8] .x + length_square / 2, y + height);
	m_EndPoint[7] = wxPoint(m_EndPoint[8] .x + length_square / 2, m_EndPoint[9] .y);
	
	m_Center = wxPoint(m_EndPoint[8].x + length_square / 2, m_EndPoint[8].y + length_square / 2);

}

void CfInputPort::Draw(wxDC& dc,const CfScreenView &view,bool highlight)
{
	wxPoint scaled_end_point[10];
	wxPoint scaled_center;
	//1. Scale the element to current scale
	for(int i = 0; i < 10; i++)
	{
		scaled_end_point[i].x = m_EndPoint[i].x * view.Scale;
		scaled_end_point[i].y = m_EndPoint[i].y * view.Scale;
	}
	scaled_center.x = m_Center.x * view.Scale;
	scaled_center.y = m_Center.y * view.Scale;

	//2. Check if the element in the screen view
	//if(!view.Contains(m_BorderRect.GetTopLeft()) && !view.Contains(m_BorderRect.GetBottomLeft())
	//	&& !view.Contains(m_BorderRect.GetTopRight()) && !view.Contains(m_BorderRect.GetBottomRight()))
	//	return;

	//3. Subtract the offset of screen
	//rect.Offset(view.Rectangle.GetLeft(),view.Rectangle.GetTop());
	//rect.Offset(view.Rectangle.GetLeftTop());

	//4. Draw the rectangle
	if(highlight)

		dc.SetPen(wxPen(cfColorList[ID_COLOR_HIGHLIGHT],LineWidth));
	else
		dc.SetPen(wxPen(LineColor,LineWidth));
	
//	if(FilledColor == wxNullColour)
		dc.SetBrush(wxBrush(*wxRED,wxTRANSPARENT));
//	else
//		dc.SetBrush(wxBrush(FilledColor));
	for(int i = 0; i < 3; i++)
	{	
		dc.DrawLine(scaled_end_point[i],scaled_end_point[i + 1]);
	}
	for(int i = 4; i < 7; i++)
	{	
		dc.DrawLine(scaled_end_point[i],scaled_end_point[i + 1]);
	}
	wxRect rect(scaled_end_point[8], scaled_end_point[9]);
	dc.DrawRectangle(rect);

	dc.DrawLine(wxPoint(scaled_center.x - 5, scaled_center.y), wxPoint(scaled_center.x + 5, scaled_center.y));
	dc.DrawLine(wxPoint(scaled_center.x, scaled_center.y - 5), wxPoint(scaled_center.x, scaled_center.y + 5));
}

void CfInputPort::Move(int dx, int dy)
{
	for(int i = 0; i < 10; i++)
	{
		m_EndPoint[i].x += dx;
		m_EndPoint[i].y += dy;
	}
	m_Center.x += dx;
	m_Center.y += dy;
	m_BorderRect.Offset(dx, dy);
}

CfShape*CfInputPort::Clone()
{
	return new CfInputPort(this);
}

CfInputPort& CfInputPort::operator = (const CfInputPort& port)
{
	m_BorderRect = port.m_BorderRect;
	this->LineColor = port.LineColor;
	this->FilledColor = port.FilledColor;
	this->LineWidth = port.LineWidth;
	InitPoints();
	return *this;
}

//IProperty

void CfInputPort::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_INPUT_PORT);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_X_POS,m_BorderRect.x+offset.x);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_Y_POS,m_BorderRect.y+offset.y);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_WIDTH,m_BorderRect.width);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_HEIGHT,m_BorderRect.height);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_LINE_COLOR,LineColor.GetAsString());
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_FILLED_COLOR,FilledColor.GetAsString());
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_LINE_WIDTH,LineWidth);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_INPUT_PORT);
	fos->Write(line.char_str(),line.size());
}
void CfInputPort::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line;// = tis.ReadLine().Trim().Trim(true);
	//if(line == wxString::Format(_("[%s]"),cfSTRING_RECTANGLE))
	{
		while (fis->CanRead())
		{
			line = tis.ReadLine().Trim().Trim(true);
			if(line == wxString::Format(_("[/%s]"),cfSTRING_INPUT_PORT))
				break;
			name = line.Before('=');
			value = line.After('=');
			if(name == cfSTRING_X_POS) m_BorderRect.x = wxAtoi(value);
			else if(name == cfSTRING_Y_POS) m_BorderRect.y = wxAtoi(value);
			else if(name == cfSTRING_WIDTH) m_BorderRect.width = wxAtoi(value);
			else if(name == cfSTRING_HEIGHT) m_BorderRect.height = wxAtoi(value);
			else if(name == cfSTRING_LINE_COLOR) LineColor.Set(value);
			else if(name == cfSTRING_FILLED_COLOR) FilledColor.Set(value);
			else if(name == cfSTRING_LINE_WIDTH) LineWidth = wxAtoi(value);
		}
		InitPoints();

    }
}

//---------------------------------------OutputPort---------------------------------------------------//
CfOutputPort::CfOutputPort()
{
}
CfOutputPort::CfOutputPort(int x, int y, int width, int height):
CfPortShape(x, y, width, height)
{
	//FilledColor = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_FILLEDCOLOR,_("None"));
	//LineColor = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_LINECOLOR,_("Blue"));
	LineWidth = CfEnvironmentUtil::Get()->GetInt(ENV_DRAWING_LINEWIDTH,3);
	InitPoints();
}

CfOutputPort::CfOutputPort(CfOutputPort *port)
{
	FilledColor = port->FilledColor;
	LineColor = port->LineColor;
	LineWidth = port->LineWidth;
	m_BorderRect = port->m_BorderRect;
	InitPoints();
}

CfOutputPort::~CfOutputPort()
{
}

void CfOutputPort::InitPoints()
{
	int x, y, width, height;
	x = this->m_BorderRect.GetLeftTop().x;
	y = this->m_BorderRect.GetLeftTop().y;
	width = this->m_BorderRect.GetSize().x;
	height= this->m_BorderRect.GetSize().y;
	double length_square;
	length_square = 15;
	//square left_top and right_buttom
	m_EndPoint[0] = wxPoint(x,y + (height - 15) / 2);
	m_EndPoint[1] = wxPoint(x + length_square, m_EndPoint[0].y + length_square);

	m_EndPoint[2] = wxPoint(m_EndPoint[0] .x + length_square / 2, m_EndPoint[0] .y );
	m_EndPoint[3] = wxPoint(m_EndPoint[2].x, y);
	m_EndPoint[4] = wxPoint(x + width, y + height / 2);
	m_EndPoint[5] = wxPoint(m_EndPoint[2] .x, y + height);
	m_EndPoint[6] = wxPoint(m_EndPoint[2] .x, m_EndPoint[1].y);
	
	m_Center = wxPoint(m_EndPoint[0].x + length_square / 2, m_EndPoint[0].y + length_square / 2);

}

void CfOutputPort::Draw(wxDC& dc,const CfScreenView &view,bool highlight)
{
	wxPoint scaled_end_point[10];
	wxPoint scaled_center;
	//1. Scale the element to current scale
	for(int i = 0; i < 10; i++)
	{
		scaled_end_point[i].x = m_EndPoint[i].x * view.Scale;
		scaled_end_point[i].y = m_EndPoint[i].y * view.Scale;
	}
	scaled_center.x = m_Center.x * view.Scale;
	scaled_center.y = m_Center.y * view.Scale;

	//2. Check if the element in the screen view
	if(!view.Contains(m_BorderRect.GetTopLeft()) && !view.Contains(m_BorderRect.GetBottomLeft())
		&& !view.Contains(m_BorderRect.GetTopRight()) && !view.Contains(m_BorderRect.GetBottomRight()))
		return;

	//3. Subtract the offset of screen
	//rect.Offset(view.Rectangle.GetLeft(),view.Rectangle.GetTop());
	//rect.Offset(view.Rectangle.GetLeftTop());

	//4. Draw the rectangle
	if(highlight)

		dc.SetPen(wxPen(cfColorList[ID_COLOR_HIGHLIGHT],LineWidth));
	else
		dc.SetPen(wxPen(LineColor,LineWidth));
	
//	if(FilledColor == wxNullColour)
		dc.SetBrush(wxBrush(*wxRED,wxTRANSPARENT));
	//else
		//dc.SetBrush(wxBrush(FilledColor));
	for(int i = 2; i < 6; i++)
	{	
		dc.DrawLine(scaled_end_point[i],scaled_end_point[i + 1]);
	}
	wxRect rect(scaled_end_point[0], scaled_end_point[1]);
	dc.DrawRectangle(rect);

	dc.DrawLine(wxPoint(scaled_center.x - 5, scaled_center.y), wxPoint(scaled_center.x + 5, scaled_center.y));
	dc.DrawLine(wxPoint(scaled_center.x, scaled_center.y - 5), wxPoint(scaled_center.x, scaled_center.y + 5));
}

void CfOutputPort::Move(int dx, int dy)
{
	for(int i = 0; i < 7; i++)
	{
		m_EndPoint[i].x += dx;
		m_EndPoint[i].y += dy;
	}
	m_Center.x += dx;
	m_Center.y += dy;
	m_BorderRect.Offset(dx, dy);
}

CfShape*CfOutputPort::Clone()
{
	return new CfOutputPort(this);
}

CfOutputPort& CfOutputPort::operator = (const CfOutputPort& port)
{
	m_BorderRect = port.m_BorderRect;
	this->LineColor = port.LineColor;
	this->FilledColor = port.FilledColor;
	this->LineWidth = port.LineWidth;
	InitPoints();
	return *this;
}

void CfOutputPort::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_OUTPUT_PORT);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_X_POS,m_BorderRect.x+offset.x);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_Y_POS,m_BorderRect.y+offset.y);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_WIDTH,m_BorderRect.width);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_HEIGHT,m_BorderRect.height);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_LINE_COLOR,LineColor.GetAsString());
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_FILLED_COLOR,FilledColor.GetAsString());
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_LINE_WIDTH,LineWidth);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_OUTPUT_PORT);
	fos->Write(line.char_str(),line.size());
}
void CfOutputPort::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line;// = tis.ReadLine().Trim().Trim(true);
	//if(line == wxString::Format(_("[%s]"),cfSTRING_RECTANGLE))
	{
		while (fis->CanRead())
		{
			line = tis.ReadLine().Trim().Trim(true);
			if(line == wxString::Format(_("[/%s]"),cfSTRING_OUTPUT_PORT))
				break;
			name = line.Before('=');
			value = line.After('=');
			if(name == cfSTRING_X_POS) m_BorderRect.x = wxAtoi(value);
			else if(name == cfSTRING_Y_POS) m_BorderRect.y = wxAtoi(value);
			else if(name == cfSTRING_WIDTH) m_BorderRect.width = wxAtoi(value);
			else if(name == cfSTRING_HEIGHT) m_BorderRect.height = wxAtoi(value);
			else if(name == cfSTRING_LINE_COLOR) LineColor.Set(value);
			else if(name == cfSTRING_FILLED_COLOR) FilledColor.Set(value);
			else if(name == cfSTRING_LINE_WIDTH) LineWidth = wxAtoi(value);
		}
		InitPoints();

    }
}

//---------------------------------------InoutPort---------------------------------------------------//
CfInoutPort::CfInoutPort()
{
}
CfInoutPort::CfInoutPort(int x, int y, int width, int height):
CfPortShape(x, y, width, height)
{
	//FilledColor = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_FILLEDCOLOR,_("None"));
	//LineColor = CfEnvironmentUtil::Get()->GetString(ENV_DRAWING_LINECOLOR,_("Blue"));
	LineWidth = CfEnvironmentUtil::Get()->GetInt(ENV_DRAWING_LINEWIDTH,3);
	InitPoints();
}

CfInoutPort::CfInoutPort(CfInoutPort *port)
{
	FilledColor = port->FilledColor;
	LineColor = port->LineColor;
	LineWidth = port->LineWidth;
	m_BorderRect = port->m_BorderRect;
	InitPoints();
}

CfInoutPort::~CfInoutPort()
{
}

void CfInoutPort::InitPoints()
{
	int x, y, width, height;
	x = this->m_BorderRect.GetLeftTop().x;
	y = this->m_BorderRect.GetLeftTop().y;
	width = this->m_BorderRect.GetSize().x;
	height= this->m_BorderRect.GetSize().y;
	double length_square;
	length_square = 15;
	//square left_top and right_buttom
	m_EndPoint[0] = wxPoint(x +( width  - length_square) / 2,y + (height - length_square) / 2);
	m_EndPoint[1] = wxPoint(m_EndPoint[0].x + length_square, m_EndPoint[0].y + length_square);

	m_EndPoint[2] = wxPoint(x, y);
	m_EndPoint[3] = wxPoint(x, y + height);
	m_EndPoint[4] = wxPoint(x + width, y + height / 2);
	m_EndPoint[5] = m_EndPoint[2];

	m_EndPoint[6] = wxPoint(x + width, y);
	m_EndPoint[7] = wxPoint(x + width, y + height);
	m_EndPoint[8] = wxPoint(x, y + height / 2);
	m_EndPoint[9] = m_EndPoint[6];

	m_Center = wxPoint(m_EndPoint[0].x + length_square / 2, m_EndPoint[0].y + length_square / 2);
}

void CfInoutPort::Draw(wxDC& dc,const CfScreenView &view,bool highlight)
{
	wxPoint scaled_end_point[10];
	wxPoint scaled_center;
	//1. Scale the element to current scale
	for(int i = 0; i < 10; i++)
	{
		scaled_end_point[i].x = m_EndPoint[i].x * view.Scale;
		scaled_end_point[i].y = m_EndPoint[i].y * view.Scale;
	}
	scaled_center.x = m_Center.x * view.Scale;
	scaled_center.y = m_Center.y * view.Scale;

	//2. Check if the element in the screen view
	if(!view.Contains(m_BorderRect.GetTopLeft()) && !view.Contains(m_BorderRect.GetBottomLeft())
		&& !view.Contains(m_BorderRect.GetTopRight()) && !view.Contains(m_BorderRect.GetBottomRight()))
		return;

	//3. Subtract the offset of screen
	//rect.Offset(view.Rectangle.GetLeft(),view.Rectangle.GetTop());
	//rect.Offset(view.Rectangle.GetLeftTop());

	//4. Draw the rectangle
	if(highlight)
		dc.SetPen(wxPen(cfColorList[ID_COLOR_HIGHLIGHT],LineWidth));
	else
		dc.SetPen(wxPen(LineColor,LineWidth));
	
//	if(FilledColor == wxNullColour)
		dc.SetBrush(wxBrush(*wxRED,wxTRANSPARENT));
	//else
		//dc.SetBrush(wxBrush(FilledColor));
	for(int i = 2; i < 5; i++)
	{	
		dc.DrawLine(scaled_end_point[i],scaled_end_point[i + 1]);
	}
	for(int i = 6; i < 9; i++)
	{	
		dc.DrawLine(scaled_end_point[i],scaled_end_point[i + 1]);
	}
	wxRect rect(scaled_end_point[0], scaled_end_point[1]);
	dc.DrawRectangle(rect);

	dc.DrawLine(wxPoint(scaled_center.x - 5, scaled_center.y), wxPoint(scaled_center.x + 5, scaled_center.y));
	dc.DrawLine(wxPoint(scaled_center.x, scaled_center.y - 5), wxPoint(scaled_center.x, scaled_center.y + 5));
}

void CfInoutPort::Move(int dx, int dy)
{
	for(int i = 0; i < 10; i++)
	{
		m_EndPoint[i].x += dx;
		m_EndPoint[i].y += dy;
	}
	m_Center.x += dx;
	m_Center.y += dy;
	m_BorderRect.Offset(dx, dy);
}

CfShape*CfInoutPort::Clone()
{
	return new CfInoutPort(this);
}

CfInoutPort& CfInoutPort::operator = (const CfInoutPort& port)
{
	m_BorderRect = port.m_BorderRect;
	this->InitPoints();
	this->LineColor = port.LineColor;
	this->FilledColor = port.FilledColor;
	this->LineWidth = port.LineWidth;
	return *this;
}

//IProperty
void CfInoutPort::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_INOUT_PORT);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_X_POS,m_BorderRect.x+offset.x);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_Y_POS,m_BorderRect.y+offset.y);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_WIDTH,m_BorderRect.width);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_HEIGHT,m_BorderRect.height);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_LINE_COLOR,LineColor.GetAsString());
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_FILLED_COLOR,FilledColor.GetAsString());
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%d\n"),cfSTRING_LINE_WIDTH,LineWidth);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_INOUT_PORT);
	fos->Write(line.char_str(),line.size());
}
void CfInoutPort::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line;// = tis.ReadLine().Trim().Trim(true);
	//if(line == wxString::Format(_("[%s]"),cfSTRING_RECTANGLE))
	{
		while (fis->CanRead())
		{
			line = tis.ReadLine().Trim().Trim(true);
			if(line == wxString::Format(_("[/%s]"),cfSTRING_INOUT_PORT))
				break;
			name = line.Before('=');
			value = line.After('=');
			if(name == cfSTRING_X_POS) m_BorderRect.x = wxAtoi(value);
			else if(name == cfSTRING_Y_POS) m_BorderRect.y = wxAtoi(value);
			else if(name == cfSTRING_WIDTH) m_BorderRect.width = wxAtoi(value);
			else if(name == cfSTRING_HEIGHT) m_BorderRect.height = wxAtoi(value);
			else if(name == cfSTRING_LINE_COLOR) LineColor.Set(value);
			else if(name == cfSTRING_FILLED_COLOR) FilledColor.Set(value);
			else if(name == cfSTRING_LINE_WIDTH) LineWidth = wxAtoi(value);
		}
		InitPoints();

    }
}

//CfShapeArray
CfShapeArray::CfShapeArray()
{
}

CfShapeArray::CfShapeArray(const CfShapeArray &shape_array)
{
	for(size_t i=0;i<shape_array.m_ShapePtrArray.Count();i++)
		m_ShapePtrArray.Add(shape_array[i]);


}
CfShapeArray::~CfShapeArray()
{
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		delete m_ShapePtrArray[i];
	}
	m_ShapePtrArray.Clear();
}

void CfShapeArray::AddShape(CfShape* shape)
{
	m_ShapePtrArray.Add(shape);
}
void CfShapeArray::ClearShape()
{
	m_ShapePtrArray.Clear();
}
bool CfShapeArray::Contains(wxPoint &point)
{
	bool res = false;
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		res = m_ShapePtrArray[i]->Contains(point);
		if(res) break;
	}
	return res;
}

void CfShapeArray::Copy(CfShapeArray* shapes)
{
	for(size_t i=0;i<shapes->Count();i++)
	{
		m_ShapePtrArray.Add((*shapes)[i]->Clone());
	}
}

bool CfShapeArray::Within(wxRect &rect)
{
	wxASSERT(m_ShapePtrArray.Count()>0);
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		//If one shape not in rect, return false
		if(!m_ShapePtrArray[i]->Within(rect)) return false;
	}
	return true;
}

bool CfShapeArray::IsEndPoint(wxPoint &point)
{
	bool res = false;
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		res = m_ShapePtrArray[i]->IsEndPoint(point);
		if(res) break;
	}
	return res;
}

bool CfShapeArray::Merge(CfShape *shape)
{
	if (m_ShapePtrArray[0]->GetTypeId() == ID_ELEMENT_LINE)
	{
		vector<wxPoint> points;
		for(size_t i=0;i<m_ShapePtrArray.Count();i++)
		{
			points.push_back(m_ShapePtrArray[i]->GetStartPos());
			points.push_back(m_ShapePtrArray[i]->GetEndPos());
		}
		int *overlap;
		overlap = new int[points.size()];
		for(size_t i=0;i<points.size();i++)
		{
			overlap[i] = 0;
		}
		for(size_t i=0;i<points.size();i++)
		{
			for(size_t j=i + 1;j<points.size();j++)
			{
				if((points[i] == points[j]) && (overlap[i] != -1))
				{
					overlap[i] ++;
					overlap[j] = -1;
				}
			}
		}
		for(size_t i=0;i<points.size();i++)
		{
			if(points[i] == shape->GetStartPos() || points[i] == shape->GetEndPos())
				if(overlap[i] > 0)	{delete []overlap;return false;}
		}

		for(size_t i=0;i<m_ShapePtrArray.Count();i++)
		{
			wxPoint i_sp, i_ep, s_sp, s_ep;
			i_sp = m_ShapePtrArray[i]->GetStartPos();
			i_ep = m_ShapePtrArray[i]->GetEndPos();
			s_sp = shape->GetStartPos();
			s_ep = shape->GetEndPos();
			if((i_sp.x == i_ep.x && i_ep.x == s_sp.x && s_sp.x == s_ep.x) && (i_sp == s_sp || i_sp == s_ep || i_ep == s_sp || i_ep == s_ep))
			{
				m_ShapePtrArray[i]->Merge(shape);
				delete []overlap;
				return true;
			}
			else if((i_sp.y == i_ep.y && i_ep.y == s_sp.y && s_sp.y == s_ep.y) && (i_sp == s_sp || i_sp == s_ep || i_ep == s_sp || i_ep == s_ep))
			{
				m_ShapePtrArray[i]->Merge(shape);
				delete []overlap;
				return true;
			}
		}
		delete []overlap;
		return false;
	}
}

void CfShapeArray::Draw(wxDC& dc,const CfScreenView &view,bool highlight)
{
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		m_ShapePtrArray[i]->Draw(dc,view,highlight);
	}
	if (m_ShapePtrArray[0]->GetTypeId() == ID_ELEMENT_LINE)
	{
		vector<wxPoint> points;
		for(size_t i=0;i<m_ShapePtrArray.Count();i++)
		{
			points.push_back(m_ShapePtrArray[i]->GetStartPos());
			points.push_back(m_ShapePtrArray[i]->GetEndPos());
		}
		int *overlap;
		overlap = new int[points.size()];
		for(size_t i=0;i<points.size();i++)
		{
			overlap[i] = 0;
		}
		for(size_t i=0;i<points.size();i++)
		{
			for(size_t j=i + 1;j<points.size();j++)
			{
				if((points[i] == points[j]) && (overlap[i] != -1))
				{
					overlap[i] ++;
					overlap[j] = -1;
				}
			}
		}
		for(size_t i=0;i<points.size();i++)
		{
			if(overlap[i] > 1)
			{
				wxPoint sp,ep;
				sp = wxPoint(points[i].x - 4, points[i].y - 4);
				ep = wxPoint(points[i].x + 4, points[i].y + 4);
				wxRect rc(sp,ep);
				if(highlight)
					dc.SetBrush(wxBrush(cfColorList[ID_COLOR_HIGHLIGHT],wxSOLID));
				else 
					dc.SetBrush(wxBrush(m_ShapePtrArray[0]->GetLineColor(),wxSOLID));
				dc.DrawRectangle(rc);
			}
			else if(highlight && overlap[i] == 0)
			{					
				dc.SetBrush(wxBrush(cfColorList[ID_COLOR_HIGHLIGHT],wxSOLID));

				dc.DrawCircle(points[i].x, points[i].y, 4);
			}	
		}
		delete []overlap;
	}
}
void CfShapeArray::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_SHAPE);
	fos->Write(line.char_str(),line.size());
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		m_ShapePtrArray[i]->Dump(fos,offset);
	}
	line = wxString::Format(_("[/%s]\n"),cfSTRING_SHAPE);
	fos->Write(line.char_str(),line.size());
}

//Return the start position of the first element
wxPoint CfShapeArray::GetStartPos()
{
	size_t n = m_ShapePtrArray.Count();
	if(n==0) return wxPoint(0,0);
	else
		return m_ShapePtrArray[0]->GetStartPos();
}


//Return the largest end position of elements
wxPoint CfShapeArray::GetEndPos()
{
	int x = 0;
	int y = 0;
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		if(x<m_ShapePtrArray[i]->GetEndPos().x)
			x = m_ShapePtrArray[i]->GetEndPos().x;
		if(y<m_ShapePtrArray[i]->GetEndPos().y)
			y = m_ShapePtrArray[i]->GetEndPos().y;
	}
	return wxPoint(x,y);
}

//Get largest x and largets y
wxSize CfShapeArray::GetSize()
{
	wxSize size(0,0); 
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		size.IncTo(m_ShapePtrArray[i]->GetSize());
	}
	return size;
}

void CfShapeArray::GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication)
{
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		m_ShapePtrArray[i]->GetPropertyUnitPtrArray(properties,check_name_duplication);
	}

}

void CfShapeArray::SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties)
{
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		m_ShapePtrArray[i]->SetPropertyUnitPtrArray(properties);
	}
}

void CfShapeArray::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line = tis.ReadLine().Trim().Trim(true);
	if(line != wxString::Format(_("[%s]"),cfSTRING_SHAPE))
	{
		wxLogError(_("Parsing error in ShapeArray:Looking for %s:Found %s"),cfSTRING_SHAPE,line);
		return;
	}

	CfShape *shape;
	while (fis->CanRead())
	{
		line = tis.ReadLine().Trim().Trim(true);
		if(line == wxString::Format(_("[%s]"),cfSTRING_LINE_SEGMENT))
			shape = new CfLineSegment();
		else if(line == wxString::Format(_("[%s]"),cfSTRING_RECTANGLE))
			shape = new CfRectangle();
		else if(line == wxString::Format(_("[%s]"),cfSTRING_INPUT_PORT))
			shape = new CfInputPort();
		else if(line == wxString::Format(_("[%s]"),cfSTRING_OUTPUT_PORT))
			shape = new CfOutputPort();
		else if(line == wxString::Format(_("[%s]"),cfSTRING_INOUT_PORT))
			shape = new CfInoutPort();
		
		else if(line == wxString::Format(_("[/%s]"),cfSTRING_SHAPE))
			break;
		else
		{
			wxLogError(_("Parsing error in ShapeArray:Found %s"),line);
			break;
		}
		shape->Load(fis,offset);
		m_ShapePtrArray.Add(shape);
	}
}

void CfShapeArray::Move(int dx, int dy)
{
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		m_ShapePtrArray[i]->Move(dx,dy);
	}
}

void CfShapeArray::SetLineColor(wxColor color)
{
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		m_ShapePtrArray[i]->SetLineColor(color);
	}
}

void CfShapeArray::SetLineWidth(int width)
{
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		m_ShapePtrArray[i]->SetLineWidth(width);
	}
}

void CfShapeArray::SetFilledColor(wxColor color)
{
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		m_ShapePtrArray[i]->SetFilledColor(color);
	}
}

wxColor CfShapeArray::GetLineColor()
{
	return m_ShapePtrArray[0]->GetLineColor();
}
int CfShapeArray::GetLineWidth()
{
	return m_ShapePtrArray[0]->GetLineWidth();
}
wxColor CfShapeArray::GetFilledColor()
{
	return m_ShapePtrArray[0]->GetFilledColor();
}