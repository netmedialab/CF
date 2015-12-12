#include "CfDefines.h"
#include "Image.h"
#include "LogMessageCtrl.h"

CfImage::CfImage(wxEvtHandler *p_event_handler)
:CfDrawingElement(p_event_handler)
{
}

CfImage::CfImage(wxEvtHandler *p_event_handler,const wxString& id, const wxString &name, const wxString& image_path)
:m_sImagePath(image_path)
,CfDrawingElement(p_event_handler,id,name,NULL)
{
}

CfImage::CfImage(CfImage *image)
	:m_sImagePath(image->m_sImagePath)
	,CfDrawingElement(image->m_pEventHandler,image->GetId(),image->GetName(),NULL)
{
}

CfImage::~CfImage(void)
{
}

wxString CfImage::GetImagePath() const
{
	return m_sImagePath;
}

//IProperty
void CfImage::GetPropertyUnitPtrArray(PropertyUnitPtrArray *properties, bool check_name_duplication)
{
	PropertyUnit *punit;
	wxString group= cfSTRING_IMAGE;
	punit = new PropertyUnit(cfSTRING_IMAGE,wxEmptyString,cfSTRING_GROUP);
	properties->Add(punit);
	punit = new PropertyUnit(cfSTRING_ID,m_sId,cfGRID_VALUE_STRING,true);
	properties->Add(punit);
	punit = new PropertyUnit(cfSTRING_NAME,m_sName,cfGRID_VALUE_STRING,true);
	properties->Add(punit);
	punit = new PropertyUnit(cfSTRING_PATH,m_sImagePath,cfGRID_VALUE_STRING,true);
	properties->Add(punit);

	//image property
	punit = new PropertyUnit(cfSTRING_DEPTH,wxString::Format(_("%d"),m_Bitmap.GetDepth()),cfGRID_VALUE_STRING,true);
	properties->Add(punit);
	punit = new PropertyUnit(cfSTRING_WIDTH,wxString::Format(_("%d"),m_Bitmap.GetWidth()),cfGRID_VALUE_STRING,true);
	properties->Add(punit);
	punit = new PropertyUnit(cfSTRING_HEIGHT,wxString::Format(_("%d"),m_Bitmap.GetHeight()),cfGRID_VALUE_STRING,true);
	properties->Add(punit);
}

void CfImage::SetPropertyUnitPtrArray(PropertyUnitPtrArray *properties)
{
	PropertyUnit *punit;
	for(size_t i=0;i<properties->Count();i++)
	{
		punit = (*properties)[i];
		if(punit->Name == cfSTRING_ID) m_sId = punit->Value;
		else if(punit->Name == cfSTRING_PATH) m_sImagePath = punit->Value;
	}

}

void CfImage::Dump(wxFileOutputStream *fos, const wxPoint offset)
{
	wxString line;
	line = wxString::Format(_("[%s]\n"),cfSTRING_IMAGE);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("%s=%s\n"),cfSTRING_ID,m_sId);
	fos->Write(line.char_str(),line.size());
	//line = wxString::Format(_("%s=%s\n"),cfSTRING_NAME,m_sImagePath.AfterLast('\\'));
	line = wxString::Format(_("%s=%s\n"),cfSTRING_NAME,m_sImagePath);
	fos->Write(line.char_str(),line.size());
	line = wxString::Format(_("[/%s]\n"),cfSTRING_IMAGE);
	fos->Write(line.char_str(),line.size());

	this->SetModified(false);
}

void CfImage::Load(wxFileInputStream *fis, const wxPoint offset)
{
	wxTextInputStream tis(*fis);
	wxString name,value;
	wxString line;
	//= tis.ReadLine().Trim().Trim(true);
	//if(line == wxString::Format(_("[%s]"),cfSTRING_IMAGE))
	{
		while (fis->CanRead())
		{
			line = tis.ReadLine().Trim().Trim(true);
			if(line == wxString::Format(_("[/%s]"),cfSTRING_IMAGE))
				break;
			name = line.Before('=');
			value = line.After('=');
			if(name == cfSTRING_ID)
				m_sId = value;
			else if(name == cfSTRING_NAME)
			{
				if(wxFileName::FileExists(value)) 
				{
					m_sImagePath = value;
					m_sName = value.AfterLast('\\');
				}
				//else
				//{
				//	wxString msg = wxString::Format(_("Failed to locate the image file '%s'."),value);
				//	wxMessageDialog dlg(NULL,msg,PROJECT_CAPTION,wxOK| wxOK_DEFAULT| wxICON_WARNING);
				//	dlg.ShowModal();
				//	wxFileDialog dialog
				//		(
				//		NULL,
				//		_("Open File"),
				//		wxEmptyString,
				//		value,
				//		_("Jpeg files(*.jpg)|*.jpg")
				//		);
				//	if (dialog.ShowModal() == wxID_OK)
				//	{
				//		m_sImagePath = dialog.GetPath();
				//		SetModified();
				//	}
				//}
			}
		}
    }
}

bool CfImage::LoadFile()
{
	if (m_sImagePath.IsEmpty()) return false;
	wxFileName fn(m_sImagePath);
    clock_t  clockBegin, clockEnd;
    clockBegin = clock();
	bool res = m_Bitmap.LoadFile(m_sImagePath,wxBITMAP_TYPE_JPEG);
    clockEnd = clock();
	wxString msg = wxString::Format(_("Load %s. [%ld ms]"),m_sImagePath,clockEnd - clockBegin);
	wxLogMessage(msg);
	return res;


	//---------------------------------------------------------------------------------------------
	//Not using for now

	ReadImage();
	Mat *mat1=new Mat(img,true);
	wxImage image((mat1->cols), (mat1->rows),(mat1->data), true);  
	m_Bitmap = wxBitmap(image);
	return true;
}

void CfImage::Draw(wxDC& dc,const CfScreenView &view)
{
	if(m_bDeleted || !m_bShowing) return;
	wxLogDebug(wxString::Format(_("Drawing %s\n"),m_sName));

	dc.SetUserScale(view.Scale,view.Scale);
	if (m_Bitmap.IsOk() || LoadFile())
        dc.DrawBitmap( m_Bitmap, 0, 0 );
	else
		wxLogMessage(_("Failed to load image '%s'"),m_sImagePath); 
	dc.SetUserScale(1,1);

}

bool CfImage::Contains(wxPoint &point)
{
	return (m_Bitmap.GetHeight()>point.y)&&(m_Bitmap.GetWidth()>point.x);
}

void CfImage::Move(int dx, int dy)
{
	//Do not move image
}

wxPoint CfImage::GetStartPos()
{
	return wxPoint(0,0);
}
wxPoint CfImage::GetEndPos()
{
	if (m_Bitmap.IsOk() || LoadFile())
		return wxPoint(m_Bitmap.GetWidth(),m_Bitmap.GetHeight());
	else
		return wxPoint(0,0);

}

wxSize CfImage::GetSize()
{
	if (m_Bitmap.IsOk() || LoadFile())
		return wxSize(m_Bitmap.GetWidth(),m_Bitmap.GetHeight());
	else
		return wxSize(0,0);
}

void CfImage::ReadImage()
{
	struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPARRAY buffer;
	unsigned char *src_test;
	unsigned char *src_end;
	unsigned char *point_end;
    cinfo.err=jpeg_std_error(&jerr);

	jpeg_CreateDecompress((&cinfo), 62,(size_t) sizeof(struct jpeg_decompress_struct));
	FILE * input_file=fopen(m_sImagePath,"rb");
    jpeg_stdio_src(&cinfo,input_file);
    jpeg_read_header(&cinfo,TRUE);
    jpeg_start_decompress(&cinfo);

    unsigned long width=cinfo.output_width;
    unsigned long height=cinfo.output_height;
    unsigned short depth=cinfo.output_components;
	unsigned long width_temp=cinfo.output_width/100;
    unsigned long height_temp=cinfo.output_height/100;
	src_test=new unsigned char[width_temp*height_temp*depth];
	src_end=new unsigned char[width*1000*depth];
	memset(src_test,0,sizeof(unsigned char)*width_temp*height_temp*depth);

    buffer=(*cinfo.mem->alloc_sarray)
        ((j_common_ptr)&cinfo,JPOOL_IMAGE,width*depth,1);
	point_end=src_end;
	int temp=0;
	int i=0;
	int Temp=0;
	int index=0;
	Mat mat_data1=Mat(0,width/4,CV_8UC3,src_test);
	int index_small=0;
	int TT=0;
    while (cinfo.output_scanline<height)
    {
		jpeg_read_scanlines(&cinfo,buffer,1);    //读取一行jpg图像数据到buffer
		/////////////////////////////////////////////////////////////////////////////////
		memcpy(point_end,*buffer,width*depth);
		//cout<<cinfo.output_scanline<<endl;
		if((cinfo.output_scanline+1)%4==0)
		{
			point_end=src_end;
			Mat mat_data=Mat(4,width,CV_8UC3,src_end);
			IplImage iplimage=mat_data;
			IplImage *x=cvCreateImage(cvSize(width/4,1),iplimage.depth,iplimage.nChannels);
            cvResize(&iplimage, x, CV_INTER_LINEAR); //缩放源图像到目标图像
			mat_data1=mergeRows(mat_data1,x);
			Mat mat_data2=Mat(x,0);
			IplImage iplimage1=mat_data1;
		}
		else       
		{
			point_end+=width*depth;
		}
	}
	//Mat mat_data;		//=Mat(height/4,width/4,CV_8UC3,src_test);
	//mat_data=mat_data1;
	IplImage iplimage =mat_data1;
	IplImage *img_converted=cvCreateImage(cvGetSize(&iplimage),iplimage.depth,iplimage.nChannels);
	cvSetImageROI(img_converted, cvRect(0, 0, iplimage.width, iplimage.height));

	cvCvtColor( &iplimage , img_converted , CV_RGBA2RGB);
	img=img_converted;
	//store=new IplImage(*img_converted);
	cvSaveImage("saveImage.jpg",img_converted);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

	delete []src_test;
	delete []src_end;
	//delete input_file;
	fclose(input_file);
}

Mat CfImage::mergeRows(Mat A, Mat B)
{
    int totalRows = A.rows + B.rows;
    Mat mergedDescriptors(totalRows, A.cols, A.type());
    Mat submat = mergedDescriptors.rowRange(0, A.rows);
    A.copyTo(submat);
    submat = mergedDescriptors.rowRange(A.rows, totalRows);
    B.copyTo(submat);
    return mergedDescriptors;
}