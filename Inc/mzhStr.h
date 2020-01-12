#pragma once

#ifndef _MZH_STRING
#define _MZH_STRING

#include <mzhBase.h>

#include <string>
#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

_MZH_BEGIN

template<class ContainerT>
void Split(ContainerT& container, LPCSTR text, LPCSTR delimiter)
{
	PSTR ptmp = strdup(text);
	for (PSTR pnext = NULL, pstr = strtok_s(ptmp, delimiter, &pnext); pstr; pstr = strtok_s(NULL, delimiter, &pnext))
		container.push_back(pstr);
	free(ptmp);
}

template<class ContainerT>
void Split(ContainerT& container, LPCWSTR text, LPCWSTR delimiter)
{
	PWSTR ptmp = wcsdup(text);
	for (PWSTR pnext = NULL, pstr = wcstok_s(ptmp, delimiter, &pnext); pstr; pstr = wcstok_s(NULL, delimiter, &pnext))
		container.push_back(pstr);
	free(ptmp);
}

template<class T, class VTR>
T Join(const T& separator, const VTR& vtr)
{
	T str;
	BOOL bFirst = TRUE;
	for (const auto& v : vtr)
	{
		if (bFirst)
		{
			str = v;
			bFirst = FALSE;
		}
		else
		{
			str += separator;
			str += v;
		}
	}
	return str;
}

template<class BaseT>
class CStringT : public BaseT
{
public:
	CStringT() {}
	CStringT(const char* _Ptr) : BaseT(_Ptr) {}
	CStringT(const char* _Ptr, size_t count) : BaseT(_Ptr, count) {}
	CStringT(const wchar_t* _Ptr) : BaseT(_Ptr) {}
	CStringT(const wchar_t* _Ptr, size_t count) : BaseT(_Ptr, count) {}
	CStringT(const CStringT& obj) : BaseT(obj) {}
	~CStringT() {}

public:
	operator LPCTSTR () const { return BaseT::c_str(); }
	//CStringT& operator = (const TCHAR* _Ptr)
	//{
	//	((BaseT*)this) = _Ptr;
	//	return *this;
	//}
};

typedef CStringT<std::string> CStringA;
typedef CStringT<std::wstring> CStringW;

#ifdef _UNICODE
typedef CStringW CString;
#else
typedef CStringA CString;
#endif

_MZH_END

#endif
