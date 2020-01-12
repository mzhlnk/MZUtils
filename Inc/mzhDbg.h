#pragma once

#ifndef _MZH_DBG
#define _MZH_DBG

#include <stdio.h>
#include <mzhDef.h>

#ifndef DEBUG_ONLY
#ifdef _DEBUG
#define DEBUG_ONLY(f)	(f)
#else
#define DEBUG_ONLY(f)	((void)0)
#endif
#endif

#ifdef _DEBUG
#define DBG_IF_CALL(b, x)	if(b) x
#else
#define DBG_IF_CALL(b, x)
#endif

inline int AssertMessage(LPCTSTR _Expression, LPCTSTR _File, unsigned _Line)
{
	int len = 1 + _sctprintf(TEXT("Debug Assertion Failed!\r\n\r\nExpression: %s\r\nFile: %s\r\nLine: %d"), _Expression, _File, _Line);
	LPTSTR pszMessage = new TCHAR[len];
	_stprintf_s(pszMessage, len, TEXT("Debug Assertion Failed!\r\n\r\nExpression: %s\r\nFile: %s\r\nLine: %d"), _Expression, _File, _Line);
	int ret = MessageBox(NULL, pszMessage, TEXT("MZASSERT"), MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION);
	delete[] pszMessage;
	if (ret == IDABORT)
		ExitProcess(-1);
	return ret;
}

#ifndef ASSERT
#ifdef _DEBUG
//#define ASSERT(f)	for(int x = IDRETRY; !(f) && x != IDIGNORE; x = MessageBox(NULL, _MZH CTextFormat(TEXT("Debug Assertion Failed!\r\n\r\nExpression: %s\r\nFile: %s\r\nLine: %d"), TEXT(# f), TEXT(__FILE__), __LINE__), TEXT("MZASSERT"), MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION)) if(x == IDABORT) ExitProcess(-1);
#define ASSERT(f)	while(!(f) && AssertMessage(TEXT(# f), TEXT(__FILE__), __LINE__) == IDRETRY);
#else
#define ASSERT(f)	((void)0)
#endif
#endif

#ifndef VERIFY
#ifdef _DEBUG
#define VERIFY(f) ASSERT(f)
#else
#define VERIFY(f)	f
#endif
#endif

#ifndef TRACE
#ifdef _DEBUG
inline void DebugPrint(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	int cch = _vscprintf(pszFmt, args);
	if (cch > 0)
	{
		char* pszBuffer = new char[cch + 1];
		if (_vsnprintf_s(pszBuffer, cch + 1, cch, pszFmt, args) == -1)
		{
			ASSERT(0);
		}
		else
		{
			OutputDebugStringA(pszBuffer);
		}
		delete[] pszBuffer;
	}
	va_end(args);
}
inline void DebugPrint(const wchar_t* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	int cch = _vscwprintf(pszFmt, args);
	if (cch > 0)
	{
		wchar_t* pszBuffer = new wchar_t[cch + 1];
		if (_vsnwprintf_s(pszBuffer, cch + 1, cch, pszFmt, args) == -1)
		{
			ASSERT(0);
		}
		else
		{
			OutputDebugStringW(pszBuffer);
		}
		delete[] pszBuffer;
	}
	va_end(args);
}

#define TRACE(...) DebugPrint(__VA_ARGS__)	//{ TCHAR szText[1024] = {0}; _stprintf_s(szText, fmt, __VA_ARGS__); OutputDebugString(szText); }
#else
#define TRACE(...)
#endif
#endif

#ifndef TRACE0
#define TRACE0(sz)	TRACE(_T("%s"), _T(sz))
#endif
#ifndef TRACE1
#define TRACE1(sz, p1)	TRACE(_T(sz), p1)
#endif
#ifndef TRACE2
#define TRACE2(sz, p1, p2) TRACE(_T(sz), p1, p2)
#endif
#ifndef TRACE3
#define TRACE3(sz, p1, p2, p3) TRACE(_T(sz), p1, p2, p3)
#endif

#ifdef _DEBUG
class CTraceFunc
{
public:
	CTraceFunc(LPCTSTR lpszBegin, LPCTSTR lpszEnd)
		: m_lpszEnd(lpszEnd)
	{ TRACE1("%s", lpszBegin); }
	~CTraceFunc()
	{ TRACE1("%s", m_lpszEnd); }
	LPCTSTR m_lpszEnd;
};
#define TRACE_FUNC()	CTraceFunc _tf__(TEXT("-> ") TEXT(__FUNCTION__) TEXT(" ->\n"), TEXT("<- ") TEXT(__FUNCTION__) TEXT(" <-\n"))
#else
#define TRACE_FUNC()
#endif

#endif
