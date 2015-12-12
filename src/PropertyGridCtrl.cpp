#include "CfDefines.h"
#include "Notification.h"
#include "PropertyGridCtrl.h"
#include "Port.h"
#include "Wire.h"
#include "Layer.h"
#include "Image.h"
#include "Symbol.h"
#include "Instance.h"
#include "Parameter.h"
#include "SolidColorBackground.h"
#include "DrawingElementFactory.h"
#include <wx/propgrid/advprops.h>

BEGIN_EVENT_TABLE(CfPropertyGridCtrl,wxPropertyGrid)
	EVT_PG_CHANGED(wxID_ANY, CfPropertyGridCtrl::OnGridEvent)
//EVT_GRID_CELL_CHANGING(CfPropertyGridCtrl::OnGridEvent)
END_EVENT_TABLE()

CfPropertyGridCtrl::CfPropertyGridCtrl(wxWindow* parent)
:wxPropertyGrid(parent, ID_ElementProperty,wxDefaultPosition, wxSize(400,600),wxALWAYS_SHOW_SB| wxVSCROLL ,_("Properties"))//240,600
,CfNotificationSender(parent)
,m_pPropertyElement(NULL)
{
}
CfPropertyGridCtrl::~CfPropertyGridCtrl(void)
{
	//ClearProperties();
}

//void CfPropertyGridCtrl::ClearProperties()
//{
//	for(size_t i=0;i<m_Properties.Count();i++)
//	{
//		delete m_Properties[i];
//	}
//	m_Properties.Clear();
//}
void CfPropertyGridCtrl::InitGrid()
{
	this->Clear();
	if(m_pPropertyElement)
		m_pPropertyElement->GetProperty(this); 

}

IProperty* CfPropertyGridCtrl::GetPropertyElement()
{
	return m_pPropertyElement;
}
void CfPropertyGridCtrl::SetPropertyElement(IProperty* element)
{
	m_pPropertyElement = element;
}




void CfPropertyGridCtrl::OnGridEvent( wxPropertyGridEvent& event )
{
    wxPGProperty* property = event.GetProperty();    
	// Do nothing if event did not have associated property    
	if ( !property )        return;    
	// GetValue() returns wxVariant, but it is converted transparently to    // wxAny    
	wxVariant v;
	wxAny value = property->GetValue();    
	// Also, handle the case where property value is unspecified    
	if ( value.IsNull() )        return;    
	// Handle changes in values, as needed    

	m_pPropertyElement->SetProperty(property);

	//if the background tyep of layer is changed, refresh grid is required
	wxString name = property->GetName();
	if(name==cfSTRING_BACKGROUND)
	 InitGrid();		// maybe modified later

	if(value.CheckType<wxString>())
	wxLogDebug(value.As<wxString>());    
	else if(value.CheckType<wxColor>())
		wxLogDebug(value.As<wxColor>().GetAsString());    

	//Refresh image editor
	CfNotification *pNotification = new CfNotification(cfId_ImageEditor_Refresh);
	AddNotification(pNotification);
	//Update layer view
	CfNotification *pNotification2 = new CfNotification(cfId_LayerView_Refresh);
	AddNotification(pNotification2);
} 

//CfParameterDialogAdapter
CfParameterDialogAdapter::CfParameterDialogAdapter(CfParameter *param)
	:m_pParameter(param)
{
}
CfParameterDialogAdapter::~CfParameterDialogAdapter()
{
}


bool CfParameterDialogAdapter::DoShowDialog( wxPropertyGrid* WXUNUSED(propGrid),
                               wxPGProperty* WXUNUSED(property) )
{
	CfParameterDialogCtrl param_dlg(NULL, m_pParameter);
	if(param_dlg.ShowModal() == wxID_OK)
	{
		SetValue(m_pParameter->GetParamNames());
        return true;
    }
    return false;
}


//parameterProperty
CfParameterProperty::CfParameterProperty( 
CfParameter *param, 
const wxString& label,
const wxString& name,
const wxString& value
)
: wxStringProperty(label, name, value)
,m_pParameter(param)
{

}

void CfInstance::SetProperty(wxPGProperty *pgproperty)
{
	wxString name = pgproperty->GetName();
	wxAny value = ((wxAny) pgproperty->GetValue());

	if( name == cfSTRING_ID) m_sId = value.As<wxString>();
	else if(name == cfSTRING_NAME) m_sName = value.As<wxString>();
	else if(name == cfSTRING_LIBRARY) m_sLibraryName = value.As<wxString>();
	else if(name == cfSTRING_PATH) m_sFilePath = value.As<wxString>();

	m_pShape->SetProperty(pgproperty);

	this->SetModified();

}
void CfLayer::SetProperty(wxPGProperty *pgproperty)
{
	wxString str_background;
	int bkgd_idx = 0;
	wxString name = pgproperty->GetName();
	wxAny value = ((wxAny) pgproperty->GetValue());

	if( name == cfSTRING_ID) m_sId = value.As<wxString>();
	else if(name == cfSTRING_NAME) m_sName = value.As<wxString>();
	else if(name == cfSTRING_BACKGROUND) 
	{
		bkgd_idx = pgproperty->GetChoiceSelection();
		if(bkgd_idx == 0) //Image
		{
			if(m_pBackground && (m_pBackground->GetTypeId() == ID_ELEMENT_IMAGE)) return;
			
			delete m_pBackground;
			m_pBackground = new CfImage(this->m_pEventHandler);
		}
		else if(bkgd_idx == 1) //color
		{
			if(m_pBackground && (m_pBackground->GetTypeId() == ID_ELEMENT_SOLIDCOLORBACKGROUND)) return;
			
			delete m_pBackground;
			m_pBackground = new CfSolidColorBackground(this->m_pEventHandler,*wxWHITE);
		}
		else
		{
			if(m_pBackground) 
			{
				delete m_pBackground;
				m_pBackground = NULL;
			}
		}
		CfNotification *pNotification = new CfNotification(cfId_PropertyView_ShowElement);
		pNotification->SetElement(this);
		AddNotification(pNotification);
	}
	else if(name == cfSTRING_IMAGE) 
	{
		if(m_pBackground) delete m_pBackground;
		m_pBackground = DoCreateDrawingElement(this->m_pEventHandler,ID_ELEMENT_IMAGE,NULL,value.As<wxString>());
	}
	else if(name == cfSTRING_FILLED_COLOR) 
	{
		if(m_pBackground) delete m_pBackground;
		m_pBackground = new CfSolidColorBackground(this->m_pEventHandler,value.As<wxColor>());
	}
	this->SetModified();
}

void CfPort::SetProperty(wxPGProperty *pgproperty)
{
	wxString name = pgproperty->GetName();
	wxAny value = ((wxAny) pgproperty->GetValue());

	if( name == cfSTRING_ID) m_sId = value.As<wxString>();
	else if(name == cfSTRING_NAME) m_sName = value.As<wxString>();
	else if(name == cfSTRING_DIRECTION)
	{
		if(m_sDirection.compare(pgproperty->GetDisplayedString()))
			this->m_bDirectionChanged = true;
		m_sDirection= pgproperty->GetDisplayedString();
	}
	m_pShape->SetProperty(pgproperty);

	this->SetModified();
}
void CfWire::SetProperty(wxPGProperty *pgproperty)
{
	wxString name = pgproperty->GetName();
	wxAny value = ((wxAny) pgproperty->GetValue());

	if( name == cfSTRING_ID) m_sId = value.As<wxString>();
	else if(name == cfSTRING_NAME) m_sName = value.As<wxString>();

	m_pShape->SetProperty(pgproperty);

	this->SetModified();
}
void CfRectangle::SetProperty(wxPGProperty *pgproperty)
{
	wxString name = pgproperty->GetName();
	wxAny value = pgproperty->GetValue();
	if(name == cfSTRING_X_POS) AbsoluteRect.x = value.As<int>();
	else if(name == cfSTRING_Y_POS) AbsoluteRect.y = value.As<int>();
	else if(name == cfSTRING_WIDTH) AbsoluteRect.width = value.As<int>();
	else if(name == cfSTRING_HEIGHT) AbsoluteRect.height = value.As<int>();
	else if(name == cfSTRING_LINE_COLOR) LineColor = value.As<wxColor>();
	//else if(name == cfSTRING_FILLED_COLOR) FilledColor = value.As<wxColor>();
	else if(name == cfSTRING_LINE_WIDTH) LineWidth = value.As<int>();
}
void CfSymbol::SetProperty(wxPGProperty *pgproperty)
{
	wxString name = pgproperty->GetName();
	wxAny value = ((wxAny) pgproperty->GetValue());

	if( name == cfSTRING_ID) m_sId = value.As<wxString>();
	else if(name == cfSTRING_NAME) m_sName = value.As<wxString>();

	m_pShape->SetProperty(pgproperty);

	this->SetModified();

}
void CfPortShape::SetProperty(wxPGProperty *pgproperty)
{
	wxString name = pgproperty->GetName();
	wxAny value = pgproperty->GetValue();
	if(name == cfSTRING_X_POS) {m_BorderRect.x = value.As<int>();this->InitPoints();}
	else if(name == cfSTRING_Y_POS) {m_BorderRect.y = value.As<int>();this->InitPoints();}
	else if(name == cfSTRING_WIDTH) {m_BorderRect.width = value.As<int>();this->InitPoints();}
	else if(name == cfSTRING_HEIGHT) {m_BorderRect.height = value.As<int>();this->InitPoints();}
	
	else if(name == cfSTRING_LINE_COLOR) LineColor = value.As<wxColor>();
	//else if(name == cfSTRING_FILLED_COLOR) FilledColor = value.As<wxColor>();
	else if(name == cfSTRING_LINE_WIDTH) LineWidth = value.As<int>();
}

void CfLineSegment::SetProperty(wxPGProperty *pgproperty)
{
	wxString name = pgproperty->GetName();
	wxAny value = pgproperty->GetValue();
	int motion = 0;
	bool changed = false;
	if(name == cfSTRING_START_X_POS) {motion = value.As<int>() - StartPoint.x;SetChanged(motion, Start_x);}
	else if(name == cfSTRING_START_Y_POS) {motion = value.As<int>() - StartPoint.y;SetChanged(motion, Start_y);}
	else if(name == cfSTRING_END_X_POS) {motion = value.As<int>() - EndPoint.x;SetChanged(motion, End_x);}
	else if(name == cfSTRING_END_Y_POS) {motion = value.As<int>() - EndPoint.y;SetChanged(motion, End_y);}
	//
	//else if(name == cfSTRING_LINE_COLOR) LineColor = value.As<wxColor>();
	////else if(name == cfSTRING_FILLED_COLOR) FilledColor = value.As<wxColor>();
	//else if(name == cfSTRING_LINE_WIDTH) LineWidth = value.As<int>();
}
//void CfInoutPort::SetProperty(wxPGProperty *pgproperty)
//{
//}
//void CfOutputPort::SetProperty(wxPGProperty *pgproperty)
//{
//}
//void CfInputPort::SetProperty(wxPGProperty *pgproperty)
//{
//}
void CfShapeArray::SetProperty(wxPGProperty *pgproperty)
{
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		m_ShapePtrArray[i]->SetProperty(pgproperty);
	}
}
void CfInstance::GetProperty(wxPropertyGridInterface *pg)
{
	pg->Append( new wxPropertyCategory(wxT("General"),wxPG_LABEL) );
	pg->Append( new wxStringProperty(cfSTRING_ID,wxPG_LABEL,m_sId));
	pg->DisableProperty(cfSTRING_ID);
	pg->Append( new wxStringProperty(cfSTRING_SYMBOL,wxPG_LABEL,m_sSymbolId));
	pg->DisableProperty(cfSTRING_SYMBOL);
	pg->Append( new wxStringProperty(cfSTRING_NAME,wxPG_LABEL,m_sName));
	pg->Append( new wxStringProperty(cfSTRING_LIBRARY,wxPG_LABEL,m_sLibraryName));
	pg->Append( new wxStringProperty(cfSTRING_PATH,wxPG_LABEL,m_sFilePath));

	m_pShape->GetProperty(pg);

}
void CfLayer::GetProperty(wxPropertyGridInterface *pg)
{
	pg->Append( new wxPropertyCategory(wxT("General"),wxPG_LABEL) );
	pg->Append( new wxStringProperty(cfSTRING_ID,wxPG_LABEL,m_sId));
	pg->DisableProperty(cfSTRING_ID);
	pg->Append( new wxStringProperty(cfSTRING_NAME,wxPG_LABEL,m_sName));
	wxPGProperty *pgpro = pg->Append( new wxEnumProperty(cfSTRING_BACKGROUND,wxPG_LABEL,cfBackgroundTypeList));
	if(!m_pBackground) 
	{
		pgpro->SetValue(cfSTRING_NONE);
		return;
	}

	int type = m_pBackground->GetTypeId();
	if(type == ID_ELEMENT_IMAGE)
	{
		pgpro->SetValue(cfSTRING_IMAGE);
		pgpro = pg->Append(new wxFileProperty(cfSTRING_IMAGE, wxPG_LABEL));

		pgpro->SetAttribute(wxPG_FILE_WILDCARD,wxT("Image Files (*.jpg)|*.jpg"));
		pgpro->SetAttribute(wxPG_FILE_DIALOG_TITLE,wxT("Choose Image File"));
		pgpro->SetAttribute(wxPG_FILE_SHOW_FULL_PATH,false);
		//pgpro->SetAttribute(wxPG_FILE_SHOW_RELATIVE_PATH,((CfImage*)m_pBackground)->GetImagePath());
		pgpro->SetValue(m_pBackground->GetName());
	}
	else if(type == ID_ELEMENT_SOLIDCOLORBACKGROUND)
	{
		pgpro->SetValue(cfSTRING_COLOR);
		pg->Append( new wxColourProperty(cfSTRING_FILLED_COLOR,wxPG_LABEL,((CfSolidColorBackground*)m_pBackground)->GetColor()));
		pg->SetPropertyEditor(cfSTRING_FILLED_COLOR, wxPGEditor_ComboBox );
	}


}
void CfPort::GetProperty(wxPropertyGridInterface *pg)
{
	pg->Append( new wxPropertyCategory(wxT("General"),wxPG_LABEL) );
	pg->Append( new wxStringProperty(cfSTRING_ID,wxPG_LABEL,m_sId));
	pg->DisableProperty(cfSTRING_ID);
	pg->Append( new wxStringProperty(cfSTRING_NAME,wxPG_LABEL,m_sName));
	wxPGProperty *pgpro = pg->Append( new wxEnumProperty(cfSTRING_DIRECTION,wxPG_LABEL,cfPortStringList));
	pgpro->SetValue(m_sDirection);
	//pg->Append( new wxIntProperty(cfSTRING_PORT_WIDTH,wxPG_LABEL,m_iWidth));

	m_pShape->GetProperty(pg);
	//m_pShape->GetPropertyUnitPtrArray(properties,true);

}
void CfWire::GetProperty(wxPropertyGridInterface *pg)
{
	pg->Append( new wxPropertyCategory(wxT("General"),wxPG_LABEL) );
	pg->Append( new wxStringProperty(cfSTRING_ID,wxPG_LABEL,m_sId));
	pg->DisableProperty(cfSTRING_ID);
	pg->Append( new wxStringProperty(cfSTRING_NAME,wxPG_LABEL,m_sName));
	
	//pg->Append( new wxIntProperty(cfSTRING_PORT_WIDTH,wxPG_LABEL,m_iWidth));

	m_pShape->GetProperty(pg);
}
void CfRectangle::GetProperty(wxPropertyGridInterface *pg)
{
	//Appearance
	pg->Append( new wxPropertyCategory(cfSTRING_APPEARANCE,wxPG_LABEL) );
	pg->Append( new wxColourProperty(cfSTRING_LINE_COLOR,wxPG_LABEL,LineColor) );
	pg->SetPropertyEditor(cfSTRING_LINE_COLOR, wxPGEditor_ComboBox );
//	pg->Append( new wxColourProperty(cfSTRING_FILLED_COLOR,wxPG_LABEL,wxTransparentColour) );
//	pg->SetPropertyEditor(cfSTRING_FILLED_COLOR, wxPGEditor_ComboBox );
	pg->Append( new wxIntProperty(cfSTRING_LINE_WIDTH,wxPG_LABEL,LineWidth) );

	//Position
	pg->Append( new wxPropertyCategory(wxT("Position"),wxPG_LABEL) );
	pg->Append( new wxIntProperty(cfSTRING_X_POS,wxPG_LABEL,AbsoluteRect.x) );
    pg->SetPropertyAttribute(cfSTRING_X_POS, wxPG_ATTR_UNITS, wxT("Pixels") );
    pg->SetPropertyHelpString(cfSTRING_X_POS, wxT("x of origin") );
	pg->Append( new wxIntProperty(cfSTRING_Y_POS,wxPG_LABEL,AbsoluteRect.y) );
    pg->SetPropertyAttribute(cfSTRING_Y_POS, wxPG_ATTR_UNITS, wxT("Pixels") );
    pg->SetPropertyHelpString(cfSTRING_Y_POS, wxT("y of origin") );

	pg->Append( new wxIntProperty(cfSTRING_HEIGHT,wxPG_LABEL,AbsoluteRect.height) );
    pg->SetPropertyAttribute(cfSTRING_HEIGHT, wxPG_ATTR_MIN, 0L );
    pg->SetPropertyAttribute(cfSTRING_HEIGHT, wxPG_ATTR_UNITS, wxT("Pixels") );
    //pg->SetPropertyValueUnspecified(cfSTRING_HEIGHT);
    pg->SetPropertyAttribute(cfSTRING_HEIGHT, wxPG_ATTR_HINT,"Enter new height" );
    pg->SetPropertyHelpString(cfSTRING_HEIGHT,"New height");

	pg->Append( new wxIntProperty(cfSTRING_WIDTH,wxPG_LABEL,AbsoluteRect.width) );
	pg->SetPropertyAttribute(cfSTRING_WIDTH, wxPG_ATTR_MIN, 0L );
    pg->SetPropertyAttribute(cfSTRING_WIDTH, wxPG_ATTR_UNITS, wxT("Pixels") );
    //pg->SetPropertyValueUnspecified(cfSTRING_WIDTH );
    pg->SetPropertyAttribute(cfSTRING_WIDTH, wxPG_ATTR_HINT,"Enter new width" );
    pg->SetPropertyHelpString(cfSTRING_WIDTH,"New width");

}

void CfLineSegment::GetProperty(wxPropertyGridInterface *pg)
{
	//Appearance
	pg->Append( new wxPropertyCategory(cfSTRING_APPEARANCE,wxPG_LABEL) );

	pg->Append( new wxColourProperty(cfSTRING_LINE_COLOR,wxPG_LABEL,LineColor) );
	pg->SetPropertyEditor(cfSTRING_LINE_COLOR, wxPGEditor_ComboBox );
	pg->Append( new wxIntProperty(cfSTRING_LINE_WIDTH,wxPG_LABEL,LineWidth) );


	//Position
	pg->Append( new wxPropertyCategory(wxT("Position"),wxPG_LABEL) );

	pg->Append( new wxIntProperty(cfSTRING_START_X_POS,wxPG_LABEL,this->StartPoint.x) );
    pg->SetPropertyAttribute(cfSTRING_START_X_POS, wxPG_ATTR_UNITS, wxT("Pixels") );
    pg->SetPropertyHelpString(cfSTRING_START_X_POS, wxT("x of origin") );

	pg->Append( new wxIntProperty(cfSTRING_START_Y_POS,wxPG_LABEL,StartPoint.y) );
    pg->SetPropertyAttribute(cfSTRING_START_Y_POS, wxPG_ATTR_UNITS, wxT("Pixels") );
    pg->SetPropertyHelpString(cfSTRING_START_Y_POS, wxT("y of origin") );

	pg->Append( new wxIntProperty(cfSTRING_END_X_POS,wxPG_LABEL,EndPoint.x) );
    pg->SetPropertyAttribute(cfSTRING_END_X_POS, wxPG_ATTR_MIN, 0L );
    pg->SetPropertyAttribute(cfSTRING_END_X_POS, wxPG_ATTR_UNITS, wxT("Pixels") );
    //pg->SetPropertyValueUnspecified(cfSTRING_HEIGHT);
    pg->SetPropertyAttribute(cfSTRING_END_X_POS, wxPG_ATTR_HINT,"Enter new height" );
    pg->SetPropertyHelpString(cfSTRING_END_X_POS,"New height");

	pg->Append( new wxIntProperty(cfSTRING_END_Y_POS,wxPG_LABEL,EndPoint.y) );
	pg->SetPropertyAttribute(cfSTRING_END_Y_POS, wxPG_ATTR_MIN, 0L );
    pg->SetPropertyAttribute(cfSTRING_END_Y_POS, wxPG_ATTR_UNITS, wxT("Pixels") );
    //pg->SetPropertyValueUnspecified(cfSTRING_WIDTH );
    pg->SetPropertyAttribute(cfSTRING_END_Y_POS, wxPG_ATTR_HINT,"Enter new width" );
	pg->SetPropertyHelpString(cfSTRING_END_Y_POS,"New width");
}

void CfPortShape::GetProperty(wxPropertyGridInterface *pg)
{
	//Appearance
	pg->Append( new wxPropertyCategory(cfSTRING_APPEARANCE,wxPG_LABEL) );

	pg->Append( new wxColourProperty(cfSTRING_LINE_COLOR,wxPG_LABEL,LineColor) );
	pg->SetPropertyEditor(cfSTRING_LINE_COLOR, wxPGEditor_ComboBox );
	//pg->Append( new wxColourProperty(cfSTRING_FILLED_COLOR,wxPG_LABEL,FilledColor) );
	//pg->SetPropertyEditor(cfSTRING_FILLED_COLOR, wxPGEditor_ComboBox );
	pg->Append( new wxIntProperty(cfSTRING_LINE_WIDTH,wxPG_LABEL,LineWidth) );


	//Position
	pg->Append( new wxPropertyCategory(wxT("Position"),wxPG_LABEL) );

	pg->Append( new wxIntProperty(cfSTRING_X_POS,wxPG_LABEL,m_BorderRect.x) );
    pg->SetPropertyAttribute(cfSTRING_X_POS, wxPG_ATTR_UNITS, wxT("Pixels") );
    pg->SetPropertyHelpString(cfSTRING_X_POS, wxT("x of origin") );

	pg->Append( new wxIntProperty(cfSTRING_Y_POS,wxPG_LABEL,m_BorderRect.y) );
    pg->SetPropertyAttribute(cfSTRING_Y_POS, wxPG_ATTR_UNITS, wxT("Pixels") );
    pg->SetPropertyHelpString(cfSTRING_Y_POS, wxT("y of origin") );

	pg->Append( new wxIntProperty(cfSTRING_HEIGHT,wxPG_LABEL,m_BorderRect.height) );
    pg->SetPropertyAttribute(cfSTRING_HEIGHT, wxPG_ATTR_MIN, 0L );
    pg->SetPropertyAttribute(cfSTRING_HEIGHT, wxPG_ATTR_UNITS, wxT("Pixels") );
    //pg->SetPropertyValueUnspecified(cfSTRING_HEIGHT);
    pg->SetPropertyAttribute(cfSTRING_HEIGHT, wxPG_ATTR_HINT,"Enter new height" );
    pg->SetPropertyHelpString(cfSTRING_HEIGHT,"New height");

	pg->Append( new wxIntProperty(cfSTRING_WIDTH,wxPG_LABEL,m_BorderRect.width) );
	pg->SetPropertyAttribute(cfSTRING_WIDTH, wxPG_ATTR_MIN, 0L );
    pg->SetPropertyAttribute(cfSTRING_WIDTH, wxPG_ATTR_UNITS, wxT("Pixels") );
    //pg->SetPropertyValueUnspecified(cfSTRING_WIDTH );
    pg->SetPropertyAttribute(cfSTRING_WIDTH, wxPG_ATTR_HINT,"Enter new width" );
	pg->SetPropertyHelpString(cfSTRING_WIDTH,"New width");
}
//void CfInputPort::GetProperty(wxPropertyGridInterface *pg)
//{
//}
//void CfOutputPort::GetProperty(wxPropertyGridInterface *pg)
//{
//}
//
//void CfInoutPort::GetProperty(wxPropertyGridInterface *pg)
//{
//}
void CfShapeArray::GetProperty(wxPropertyGridInterface *pg)
{
	for(size_t i=0;i<m_ShapePtrArray.Count();i++)
	{
		m_ShapePtrArray[i]->GetProperty(pg);
	}
}

void CfSymbol::GetProperty(wxPropertyGridInterface *pg)
{
	pg->Append( new wxPropertyCategory(wxT("General"),wxPG_LABEL) );
	pg->Append( new wxStringProperty(cfSTRING_ID,wxPG_LABEL,m_sId));
	pg->DisableProperty(cfSTRING_ID);
	pg->Append( new wxStringProperty(cfSTRING_NAME,wxPG_LABEL,m_sName));

	m_Parameter.GetProperty(pg);
	m_pShape->GetProperty(pg);

}


void CfParameter::GetProperty(wxPropertyGridInterface *pg)
{
	pg->Append( new CfParameterProperty(this,cfSTRING_PARAMETER,wxPG_LABEL,GetParamNames()));
	//pg->DisableProperty(cfSTRING_ID);
}
void CfParameter::SetProperty(wxPGProperty *pgproperty)
{
}