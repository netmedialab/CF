#include "StringUtil.h"

wxColor GetColorFromStr(wxString& str)
{
	for(int i=0;i<ID_COLOR_COUNT;i++)
	{
		if(str == cfColorStringList[i])
			return cfColorList[i];
	}
	return cfColorList[0];
}