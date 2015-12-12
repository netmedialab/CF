/////////////////////////////////////////////////////////////////////////////////
//
// Name        :ParseUtil.h
// Author      :James
// Created     :2010-01-28 ¿ÀÈÄ 8:11:51
// Description :
// Copyright   :(C) 2009-1010 System Centroid Inc.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once
//#include "LicenseManagerDefines.h"
//#include "LicenseManagerDefines.h"
#include <vector>
using namespace std;


#ifdef UNICODE
#define tstring wstring
#define tstringstream wstringstream
#define tchar wchar_t
#define tscanf wscanf
#define tprintf wprintf

#else 
#define tstring string
#define tstringstream stringstream
#define tchar char
#define tscanf scanf
#define tprintf printf
#endif


int str2int(tstring s, int def);
bool str2bool(tstring s, bool def);
long str2long(tstring s, long def);

tstring int2str(int v);
tstring long2str(long v);
tstring bool2str(bool v);

void string2vector(tstring s, vector<tstring>& lines);
vector<tstring> string2vector(tstring s);

//CString string2CString(tstring &s);
//tstring CString2string(CString &cs);
//tstring CString2wstring(CString &cs);
//LPCTSTR ReplaceSpace(LPCTSTR lp_str);
string ToAsciiString(const tstring& s);
tstring ToUnicodeWstring(const string& s);
std::wstring s2ws(const std::string& s);
string ws2s(const wstring& ws);