#pragma once

#ifndef _MZHTEXT
#define _MZHTEXT

#include <stdio.h>
#include <mzhBase.h>

_MZH_BEGIN

template<typename TID, typename TDT>
struct TID_TEXT {
			TID		nID;
	const	TDT*	pszText;
};
typedef TID_TEXT<int, CHAR> ID_TEXTA, *PID_TEXTA;
typedef TID_TEXT<int, WCHAR> ID_TEXTW, *PID_TEXTW;
typedef TID_TEXT<int, TCHAR> ID_TEXT, *PID_TEXT;

#define ITEM_ID_TEXTA(_NAME)	{ _NAME,	#_NAME			},
#define ITEM_ID_TEXTW(_NAME)	{ _NAME,	L##_NAME		},
#define ITEM_ID_TEXT(_NAME)		{ _NAME,	TEXT(#_NAME)	},

#define TAB_NAME(_NAME)	TAB_##_NAME
#define DEFINE_TEXTA_BY_MAP(_NAME, _MAP)	\
	const MZH::ID_TEXTA TAB_NAME(_NAME)[] = { _MAP(ITEM_ID_TEXTA) };
#define DEFINE_TEXTW_BY_MAP(_NAME, _MAP)	\
	const MZH::ID_TEXTW TAB_NAME(_NAME)[] = { _MAP(ITEM_ID_TEXT) };
#define DEFINE_TEXT_BY_MAP(_NAME, _MAP)	\
	const MZH::ID_TEXT TAB_NAME(_NAME)[] = { _MAP(ITEM_ID_TEXT) };

//#define MAP_DG(_METHOD)	\
//	_METHOD(DG_CONTROL)	\
//	_METHOD(DG_IMAGE)
//DEFINE_TEXT_BY_MAP(MSG, MAP_MSG);

template<typename TDT, SIZE_T N>
const TDT* ID2TEXT(int nID, const TID_TEXT<int, TDT>(&TAB_TEXT)[N])
{
	for (SIZE_T i(0); i < N; i++)
	{
		if (nID == TAB_TEXT[i].nID)
		{
			return TAB_TEXT[i].pszText;
		}
	}
	return NULL;
}
//#define MSG_TEXT(_MSG)	ID2TEXT(_MSG, TAB_NAME(MSG))

class CTextFormatA : public CAutoPtrT<CHAR>
{
public:
	explicit CTextFormatA(LPCSTR pszFmt, ...)
	{
		va_list args;
		va_start(args, pszFmt);
		int len = _vscprintf(pszFmt, args);
		if (len >= 0)
		{
			len += 1;
			_Ptr = (PSTR)malloc(len);
			vsprintf_s(_Ptr, len, pszFmt, args);
		}
		va_end(args);
	}

protected:
	using CAutoPtrT<CHAR>::operator CHAR*;
public:
	operator LPCSTR () const { return _Ptr; }
};

class CTextFormatW : public CAutoPtrT<WCHAR>
{
public:
	explicit CTextFormatW(LPCWSTR pszFmt, ...)
	{
		va_list args;
		va_start(args, pszFmt);
		int len = _vscwprintf(pszFmt, args);
		if (len >= 0)
		{
			len += 1;
			_Ptr = (PWSTR)malloc(sizeof(WCHAR) * len);
			vswprintf_s(_Ptr, len, pszFmt, args);
		}
		va_end(args);
	}

protected:
	using CAutoPtrT<WCHAR>::operator WCHAR*;
public:
	operator LPCWSTR () const { return _Ptr; }
};

#ifdef _UNICODE
typedef CTextFormatW CTextFormat;
#else
typedef CTextFormatA CTextFormat;
#endif

_MZH_END

#endif
