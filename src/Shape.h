#pragma once
#include "StringUtil.h"
#include "IDrawable.h"
#include "IProperty.h"
#include <wx/vector.h>

enum
{
	ID_ELEMENT_RECT = 0
	,ID_ELEMENT_LINE
	,ID_ELEMENT_INPUTPORT
	,ID_ELEMENT_OUTPUTPORT
	,ID_ELEMENT_INOUTPORT
};
class CfShape : public IProperty
{
public:
	CfShape();
	virtual ~CfShape(){};

	virtual void Move(int dx, int dy){};
	virtual void Draw(wxDC& dc,const CfScreenView &view,bool highlight){};
	virtual bool Contains(wxPoint &point){return false;};
	virtual bool Within(wxRect &rect);
	virtual bool IsEndPoint(wxPoint &point){return false;}
	virtual void Merge(CfShape *shape){};
	virtual wxPoint GetStartPos(){return wxPoint(0,0);}
	virtual wxPoint GetEndPos(){return wxPoint(0,0);}
	virtual wxSize GetSize(){return wxSize(0,0);}
	virtual CfShape* Clone()=0;
	//IProperty
	virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication=false){};
	virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties){};
	
	virtual void SetLineColor(wxColor lcolor){};
	virtual void SetLineWidth(int width){};
	virtual void SetFilledColor(wxColor fcolor){};
	virtual int GetLineWidth(){return 1;};
	virtual wxColor GetFilledColor(){return wxColor(0,0,0,wxALPHA_TRANSPARENT);};
	virtual int GetTypeId() const = 0;
	virtual wxColor GetLineColor(){return *wxBLACK;}
};


class CfRectangle : public CfShape
{
public:
	CfRectangle();
	CfRectangle(int x, int y, int width, int height);
	CfRectangle(CfRectangle *rectangle);
	~CfRectangle();

	void Draw(wxDC& dc,const CfScreenView &view,bool highlight);
	void Move(int dx, int dy);
	bool Contains(wxPoint &point);
	wxPoint GetStartPos(){return AbsoluteRect.GetLeftTop();}
	wxPoint GetEndPos(){return AbsoluteRect.GetBottomRight();}
	virtual wxSize GetSize(){return wxSize(AbsoluteRect.GetWidth(),AbsoluteRect.GetHeight());}
	virtual CfShape* Clone();

	CfRectangle& operator= (const CfRectangle& rect);
	void SetStartPosition(wxPoint start_pt);

	//IProperty
	virtual void GetProperty(wxPropertyGridInterface *propertygrid);
	virtual void SetProperty(wxPGProperty *pgproperty);
	virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication=false);
	virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties);
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset);
	virtual void Load(wxFileInputStream *fis, const wxPoint offset);

	void SetLineColor(wxColor lcolor);
	void SetLineWidth(int width);
	void SetFilledColor(wxColor fcolor);

	int GetTypeId()const {return ID_ELEMENT_RECT;}
	wxColor GetLineColor(){return this->LineColor;}
	int GetLineWidth(){return this->LineWidth;}
	wxColor GetFilledColor(){return this->FilledColor;}
private:
	wxRect AbsoluteRect;			// the (0,0) is the orign point of the chip image. NOT the orign point of screen!
									// AbsolutePosition.x = (View.Offset.x + x)/scale
									// AbsoluteSize.Width = Width / scale
	wxColor LineColor;
	wxColor FilledColor;
	int	LineWidth;

};
class CoupledPoint
{
public:
	CoupledPoint(wxPoint sp, wxPoint ep)
	{
		m_StartPoint = sp;
		m_EndPoint = ep;
	}
	wxPoint m_StartPoint;
	wxPoint m_EndPoint;
};
class CfLineSegment : public CfShape
{
public:
	CfLineSegment();
	CfLineSegment(int sx, int sy, int ex, int ey);
	CfLineSegment(CfLineSegment* line);
	~CfLineSegment();

	void Draw(wxDC& dc,const CfScreenView &view,bool highlight);
	void Move(int dx, int dy);
	bool Contains(wxPoint& point);
	bool IsContained(wxPoint sp, wxPoint ep,int d, wxPoint point);
	bool IsEndPoint(wxPoint &point);
	void Merge(CfShape * line);
	wxPoint GetStartPos(){return StartPoint;}
	wxPoint GetEndPos(){return EndPoint;}
	virtual wxSize GetSize(){return wxSize(EndPoint.x - StartPoint.x,EndPoint.y-StartPoint.y);}
	virtual CfShape* Clone();

	//IProperty
	virtual void SetProperty(wxPGProperty *pgproperty);
	virtual void GetProperty(wxPropertyGridInterface *propertygrid);
	virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication=false);
	virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties);
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset);
	virtual void Load(wxFileInputStream *fis, const wxPoint offset);

	void SetLineColor(wxColor lcolor);
	void SetLineWidth(int width);

	int GetTypeId()const {return ID_ELEMENT_LINE;}
	wxColor GetLineColor(){return this->LineColor;}
	int GetLineWidth(){return this->LineWidth;}
private:
	wxVector<CoupledPoint> points;
	wxPoint StartPoint;
	wxPoint EndPoint;
	wxColor LineColor;
	int LineWidth;
	
	double GetPointDistance(wxPoint p1, wxPoint p2);
	float GetNearestDistance(wxPoint PA, wxPoint PB, wxPoint P3);
	void SetChanged(int motion, int item);
	enum 
	{
		Start_x = 0,
		Start_y,
		End_x,
		End_y
	};
};

class CfPortShape : public CfShape
{
public:
	CfPortShape();
	CfPortShape(int x, int y, int width, int height);
	virtual ~CfPortShape();

	virtual void Move(int dx, int dy) = 0;
	virtual void Draw(wxDC& dc,const CfScreenView &view,bool highlight) = 0;
	bool Contains(wxPoint &point);
	wxPoint GetStartPos(){return m_BorderRect.GetLeftTop();}
	wxPoint GetEndPos(){return m_BorderRect.GetBottomRight();}
	wxSize GetSize(){return m_BorderRect.GetSize();}
	virtual CfShape* Clone() = 0;

	virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication=false){};
	virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties){};
	virtual void GetProperty(wxPropertyGridInterface *propertygrid);
	virtual void SetProperty(wxPGProperty *pgproperty);

	void SetLineColor(wxColor lcolor);
	void SetLineWidth(int width);
	void SetFilledColor(wxColor fcolor);
	virtual void InitPoints() = 0;
	virtual int GetTypeId() const = 0;
	wxColor GetLineColor(){return this->LineColor;}
	int GetLineWidth(){return this->LineWidth;}
	wxColor GetFilledColor(){return this->FilledColor;}

protected:
	wxRect m_BorderRect;
	wxColor LineColor;
	wxColor FilledColor;
	int	LineWidth;
	wxPoint m_Center;
};

class CfInputPort : public CfPortShape
{
public:
	CfInputPort();
	CfInputPort(int x, int y, int width, int height);
	CfInputPort(CfInputPort *port);
	 ~CfInputPort();

	void Move(int dx, int dy);
	void Draw(wxDC& dc,const CfScreenView &view,bool highlight);
	CfShape* Clone();
	CfInputPort& CfInputPort::operator = (const CfInputPort& port);
	void InitPoints();
	int GetTypeId() const{return ID_ELEMENT_INPUTPORT;}

	//virtual void SetProperty(wxPGProperty *pgproperty);
	//virtual void GetProperty(wxPropertyGridInterface *propertygrid);
	virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication=false){};
	virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties){};
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset);
	virtual void Load(wxFileInputStream *fis, const wxPoint offset);
private:
	wxPoint m_EndPoint[10];
};

class CfOutputPort : public CfPortShape
{
public:
	CfOutputPort();
	CfOutputPort(int x, int y, int width, int height);
	CfOutputPort(CfOutputPort *port);
	~CfOutputPort();

	void Move(int dx, int dy);
	void Draw(wxDC& dc,const CfScreenView &view,bool highlight);
	CfShape* Clone();

	CfOutputPort& CfOutputPort::operator = (const CfOutputPort& port);
	void InitPoints();
	int GetTypeId() const{return ID_ELEMENT_OUTPUTPORT;}

	//virtual void SetProperty(wxPGProperty *pgproperty);
	//virtual void GetProperty(wxPropertyGridInterface *propertygrid);
	virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication=false){};
	virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties){};
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset);
	virtual void Load(wxFileInputStream *fis, const wxPoint offset);
private:
	wxPoint m_EndPoint[7];
};

class CfInoutPort : public CfPortShape
{
public:
	CfInoutPort();
	CfInoutPort(int x, int y, int width, int height);
	CfInoutPort(CfInoutPort *port);
	~CfInoutPort();

	void Move(int dx, int dy);
	void Draw(wxDC& dc,const CfScreenView &view,bool highlight);
	CfShape* Clone();
	CfInoutPort& CfInoutPort::operator = (const CfInoutPort& port);
	void InitPoints();
	int GetTypeId() const{return ID_ELEMENT_INOUTPORT;}
	
	//virtual void SetProperty(wxPGProperty *pgproperty);
	//virtual void GetProperty(wxPropertyGridInterface *propertygrid);
	virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication=false){};
	virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties){};
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset);
	virtual void Load(wxFileInputStream *fis, const wxPoint offset);
private:
	wxPoint m_EndPoint[10];
};

WX_DEFINE_ARRAY_PTR(CfShape*, CfShapePtrArray);
class CfShapeArray //: public CfShape
{
public:
	CfShapeArray();
	CfShapeArray(const CfShapeArray &shape_array);
	~CfShapeArray();
	void AddShape(CfShape* shape);
	void ClearShape();
	virtual void Move(int dx, int dy);
	virtual void Draw(wxDC& dc,const CfScreenView &view,bool highlight);
	virtual bool Contains(wxPoint &point);
	virtual bool Within(wxRect &rect);
	virtual bool IsEndPoint(wxPoint &point);
	virtual bool Merge(CfShape *shape);
	virtual wxPoint GetStartPos();
	virtual wxPoint GetEndPos();
	virtual wxSize GetSize();
	void Copy(CfShapeArray* shapes);

	//IProperty
	virtual void SetProperty(wxPGProperty *pgproperty);
	virtual void GetProperty(wxPropertyGridInterface *propertygrid);
	virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication =false);
	virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties);
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset=wxPoint(0,0));
	virtual void Load(wxFileInputStream *fis, const wxPoint offset=wxPoint(0,0));

	size_t Count() {return m_ShapePtrArray.Count();};
	CfShape* operator[](size_t pos) const
	{
		return m_ShapePtrArray[pos];
	}

	void SetLineColor(wxColor lcolor);
	void SetLineWidth(int width);
	void SetFilledColor(wxColor fcolor);
	wxColor GetLineColor();
	int GetLineWidth();
	wxColor GetFilledColor();
private:
	CfShapePtrArray m_ShapePtrArray;
};