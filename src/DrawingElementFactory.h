#pragma once
#include "DrawingElement.h"

#define DoCreateDrawingElement2(event_handler,type_id,data) \
	CfDrawingElementFactory::GetDrawingElementFactory()->CreateDrawingElement(event_handler,type_id,data)
#define DoCreateDrawingElement(event_handler,type_id,shape,path) \
	CfDrawingElementFactory::GetDrawingElementFactory()->CreateDrawingElement(event_handler,type_id,shape,path)
#define DoSetElementIdCount(type_id,count) \
	CfDrawingElementFactory::GetDrawingElementFactory()->SetElementIdCount(type_id,count)
#define DoCopyDrawingElement(element) \
	CfDrawingElementFactory::GetDrawingElementFactory()->CopyDrawingElement(element)

class CfDrawingElementFactory
{
public:
	CfDrawingElementFactory(void);
	~CfDrawingElementFactory(void);

	CfDrawingElement* CreateDrawingElement(wxEvtHandler *p_event_handler,int type_id, CfShape *shape,const wxString& path);
	CfDrawingElement* CreateDrawingElement(wxEvtHandler *p_event_handler,int type_id, void * data);
	//Copy will create a new element with new ID and new name (name_copy)
	CfDrawingElement* CopyDrawingElement(CfDrawingElement *source_element); 
	//Duplicate will create a new element with same ID and same name
	CfDrawingElement* DuplicateDrawingElement(CfDrawingElement *source_element); 
	void SetElementIdCount(int type_id, int count);
	static CfDrawingElementFactory *GetDrawingElementFactory();
	static void DestroyDrawingElementFactory();

private:
	static CfDrawingElementFactory *m_pDrawingElementFactory;
	int m_ElementCountArray[ID_ELEMENT_COUNT];
	
};

