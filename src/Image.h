#pragma once
#include "DrawingElement.h"
#include <wx/bitmap.h>

#include <opencv2\opencv.hpp>
extern "C"
{
	#include "jpeglib.h"
};
using namespace std;
using namespace cv;

class CfImage : public CfDrawingElement
{
public:
	CfImage(wxEvtHandler *p_event_handler);
	CfImage(wxEvtHandler *p_event_handler,const wxString& id, const wxString &name, const wxString& image_path);
	CfImage(CfImage *image);
	~CfImage(void);
	int GetTypeId()const {return ID_ELEMENT_IMAGE;}

	bool LoadFile();
	void ReadImage();
	//DrawingElement
	virtual void Move(int dx, int dy);
	virtual bool Contains(wxPoint &point);
	virtual wxPoint GetStartPos();
	virtual wxPoint GetEndPos();
	virtual wxSize GetSize();
	//IDrawable
	void Draw(wxDC& dc,const CfScreenView &view);
	virtual bool Within(wxRect &rect) {return false;};

	//IProperty
	virtual void GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication);
	virtual void SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties);
	virtual void Dump(wxFileOutputStream *fos, const wxPoint offset=wxPoint(0,0));
	virtual void Load(wxFileInputStream *fis, const wxPoint offset=wxPoint(0,0));

	wxString GetImagePath() const;
private:
	void OnMouseEvent(wxMouseEvent& event);
	Mat mergeRows(Mat A, Mat B);
	wxBitmap m_Bitmap;
	wxString m_sImagePath;
	IplImage* img;
	//IplImage* store;
};

