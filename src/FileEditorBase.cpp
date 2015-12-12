#include "FileEditorBase.h"

WX_DEFINE_LIST(FileReaderList);

CfExternalFileReader::CfExternalFileReader()
{
}
CfExternalFileReader::~CfExternalFileReader()
{
}

cfReaderID CfExternalFileReader::GetReaderId() const
{
	return FR_EXTERNAL_READER;
}

bool CfExternalFileReader::LoadFile()
{
	return LoadFile(m_FilePath);
}

bool CfExternalFileReader::LoadFile(const wxString &file_path)
{
	m_Message = "Open file - " + file_path;
	return true;
}

void CfExternalFileReader::ProcessEvent(wxCommandEvent &event)
{

}