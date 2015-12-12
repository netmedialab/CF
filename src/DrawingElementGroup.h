#pragma once
#include "Shape.h"
#include "Notification.h"
#include "IProperty.h"
#include "DrawingElement.h"
#include "IElement.h"
#include <map>
#include <iostream>
using namespace std;

WX_DEFINE_ARRAY_PTR(CfDrawingElement*, CfDrawingElementArray);


class IElementGroup
{
public:
	IElementGroup(){};
	virtual ~IElementGroup(){};
	virtual size_t Count()=0;

	virtual void Add(CfDrawingElement* element)=0;
	virtual void Clear()=0;
	virtual bool Delete(CfDrawingElement *element, bool delete_element)=0;
	virtual CfDrawingElement* Find(CfDrawingElement *element) const=0;
	virtual CfDrawingElement* Find(wxString &element_id) const=0;
	virtual CfDrawingElement* GetElement(wxPoint &point)=0;
	virtual CfDrawingElement* operator[](size_t index) = 0;

};

class CfDrawingElementGroup : public IElementGroup
{
public:
	CfDrawingElementGroup(bool detached=true);;
	~CfDrawingElementGroup();
	CfDrawingElementArray m_ElementArray;

	size_t Count();
	void Insert(CfDrawingElement * element, size_t pos);
	void Add(CfDrawingElement * element);
	void Add(CfDrawingElementGroup *element_group);
	//void RemoveAt(size_t index);
	virtual void Clear();
	virtual bool Delete(CfDrawingElement *element, bool delete_element);
	virtual CfDrawingElement* Find(CfDrawingElement *element) const;
	virtual CfDrawingElement* Find(wxString &element_id) const;
	virtual CfDrawingElement* GetElement(wxPoint &point);
	void GetContainedElements(wxRect &rect, CfDrawingElementGroup *element_group);

	//action
	void Move(int dx, int dy);
	void Copy(CfDrawingElementGroup* element_group);
	void SetDeleted(bool del);

	//IProperty
	//virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication =false);
	//virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties);
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset=wxPoint(0,0));
	virtual void Load(wxEvtHandler *p_event_handler,wxFileInputStream *fis, const wxPoint offset=wxPoint(0,0));

	CfDrawingElement* operator[](size_t index)
	{
		return m_ElementArray[index];
	}
private:
	bool m_bDetached;
};

class IElementWithChildren 
{
public:
	IElementWithChildren(){};
	virtual ~IElementWithChildren(){};

	virtual IElementGroup* GetElementArray()=0;
	virtual void AddElement(CfDrawingElement* element)=0;
	virtual CfDrawingElement* FindElement(CfDrawingElement *element) const =0;
	virtual CfDrawingElement* FindElement(wxString &element_id) const=0;
};

class CfDrawingElementWithChildren: 
	public IElementWithChildren
//	,public CfDrawingElement 
{
public:
	CfDrawingElementWithChildren();
	~CfDrawingElementWithChildren();

	virtual CfDrawingElementGroup* GetElementArray(){ return &m_ElementArray; };

	//virtual void InsertElement(CfDrawingElement * element, size_t pos);
	virtual void AddElement(CfDrawingElement * element);
	//virtual void AddElement(CfDrawingElementGroup* element_group);
	//virtual void Clear();
	//virtual bool DeleteElement(CfDrawingElement *element, bool delete_element);
	virtual CfDrawingElement* FindElement(CfDrawingElement *element) const { return m_ElementArray.Find(element);};
	virtual CfDrawingElement* FindElement(wxString &element_id) const{ return m_ElementArray.Find(element_id);};
	
	//virtual CfDrawingElement* GetElement(wxPoint &point);
	//virtual void GetContainedElements(wxRect &rect, CfDrawingElementGroup *element_group);

protected:
	CfDrawingElementGroup m_ElementArray;
};