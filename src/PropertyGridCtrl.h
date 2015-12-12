#ifndef CF_ELEMENT_PROPERTY_CTRL_H
#define CF_ELEMENT_PROPERTY_CTRL_H
#include "wx/propgrid/propgrid.h"
#include "DrawingElement.h"
#include "Notification.h"
#include "CfParameterDialogCtrl.h"
#include "EnvironmentUtil.h"
#include "Parameter.h"

class CfPropertyGridCtrl : public wxPropertyGrid, public CfNotificationSender
{
public:
	CfPropertyGridCtrl(wxWindow* parent);
	~CfPropertyGridCtrl(void);
	virtual void InitGrid();
	IProperty* GetPropertyElement();
	void SetPropertyElement(IProperty* element);

private:
	IProperty *m_pPropertyElement;
	//PropertyUnitPtrArray m_Properties;
	DECLARE_EVENT_TABLE()

	void ClearProperties();
	void OnGridEvent( wxPropertyGridEvent& event );
};


class CfParameterDialogAdapter: public wxPGEditorDialogAdapter
{
public:
	CfParameterDialogAdapter(CfParameter *param);
	~CfParameterDialogAdapter();

    virtual bool DoShowDialog( wxPropertyGrid* WXUNUSED(propGrid),wxPGProperty* WXUNUSED(property));

private:
	CfParameter* m_pParameter;
};

class CfParameterProperty : public wxStringProperty
{
public:

	CfParameterProperty( CfParameter *param, 
		const wxString& label,
        const wxString& name = wxPG_LABEL,
        const wxString& value = wxEmptyString);

    // Set editor to have button
    virtual const wxPGEditor* DoGetEditorClass() const
    {
        return wxPGEditor_TextCtrlAndButton;
    }

    // Set what happens on button click
    virtual wxPGEditorDialogAdapter* GetEditorDialog() const
    {
		return new CfParameterDialogAdapter(m_pParameter);
    }

protected:
	CfParameter *m_pParameter;
};

#endif	//CF_ELEMENT_PROPERTY_CTRL_H