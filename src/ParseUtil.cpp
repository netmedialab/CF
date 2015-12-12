/////////////////////////////////////////////////////////////////////////////////
//
// Name        :ParseUtil.cpp
// Author      :James
// Created     :2010-01-28 ¿ÀÈÄ 8:11:51
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "CfWx.h"
#include "CfDefines.h"
#include "ParseUtil.h"
#include <cstdlib>
#include <sstream>
#include <iostream>
using namespace std;




bool str2bool(tstring s, bool def) {
	return s == _("true");
}

long str2long(tstring s, long def) {
	long r = 0;
	long f = 1;
	for(int i=s.length()-1;i>=0;i--) {
		if('0' <= s[i] && s[i] <= '9') 
			r += (s[i] - '0') * f;
		else
			return def;
		f *= 10;
	}
	return r;
}

int str2int(tstring s, int def) {
	return (int)str2long(s, def);
}

tstring int2str(int v) {
	return long2str(v);
}

tstring long2str(long v) {
	tstringstream ss;
	ss << v;
	return ss.str();
}

tstring bool2str(bool v) {
	return v?_("true"):_("false");
}

void string2vector(tstring src, vector<tstring>& lines) {
	lines.clear();
	int from = 0;
	while(true) {
		size_t p = (int)src.find(_("\n"),from);
		if(p==tstring::npos)
			break;
			
		int num = p-from;
		if(p > 0 && src[p-1] == '\r')
			num--;
			
		lines.push_back(src.substr(from,num));
		from = p+1;                        
	}
	if(from < (int)src.length())
		lines.push_back(src.substr(from));
}

vector<tstring> string2vector(tstring s) {
	vector<tstring> t;
	string2vector(s,t);
	return t;
}

//*******************************
//** wstring <=> string verison 1
//*******************************

//Converting a WChar string to a Ansi string
std::string WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen<= 0) return std::string("");
	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");
	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen -1] = 0;
	std::string strTemp(pszDst);
	delete [] pszDst;
	return strTemp;
}

//*******************************
//** wstring <=> string verison 2
//*******************************

string ws2s(const wstring& ws)
{
    string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_ALL, "Korean");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    std::string result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}

std::wstring s2ws(const std::string& s)
{
    setlocale(LC_ALL, "Korean");  // "chs" for Chinese
    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    std::wstring result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, "C");
    return result;
}

//*******************************
//** wstring <=> string verison 3
//*******************************

tstring ToUnicodeWstring(const string& s)
{
#ifdef UNICODE
	wstring ret(s.length(),' ');
	copy(s.begin(), s.end(), ret.begin());
	return ret;
#else
	return s;
#endif

}

string ToAsciiString(const tstring& ws)
{
#ifdef UNICODE
	return ws2s(ws);
#else
	return ws;
#endif

	//return WChar2Ansi(ws.c_str());
	//string ret(s.length(),' ');
	//copy(s.begin(), s.end(), ret.begin());
	//return ret;
}




//
//CString string2CString(tstring &s)
//{
//	return s.c_str();
//}
//
//tstring CString2string(CString &cs)
//{
//#ifdef UNICODE___
//	return ToAsciiString(cs.GetBuffer(0));
//#else
//	return cs.GetBuffer(0);
//#endif
//}
//
//tstring CString2wstring(CString &cs)
//{
//	return cs.GetString();
//}
//
//LPCTSTR ReplaceSpace(LPCTSTR lp_str)
//{
//	CString cs(lp_str);
//	cs.Replace(_(" "),_("%20"));
//	return cs;
//	
//}