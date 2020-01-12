#pragma once

#ifndef _MZH_CONSOLE
#define _MZH_CONSOLE

#include <mzhBase.h>

_MZH_BEGIN

template<bool t_bManaged>
class CConsoleScreenBufferT : public CHandleT<t_bManaged, INVALID_HANDLE_VALUE>
{
public:
	CConsoleScreenBufferT(HANDLE h = INVALID_HANDLE_VALUE)
		: CHandleT<t_bManaged, INVALID_HANDLE_VALUE>(h)
	{ }

public:
	BOOL Create(DWORD dwDesiredAccess, DWORD dwShareMode = 0,
				const SECURITY_ATTRIBUTES* lpSecurityAttributes = NULL,
				DWORD dwFlags = CONSOLE_TEXTMODE_BUFFER,
				LPVOID lpScreenBufferData = NULL)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateConsoleScreenBuffer(dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwFlags, lpScreenBufferData));
	}

public:
	BOOL SetConsoleActiveScreenBuffer() const
	{
		ASSERT(IsValid());
		return ::SetConsoleActiveScreenBuffer(*this);
	}

	BOOL GetConsoleScreenBufferInfo(CONSOLE_SCREEN_BUFFER_INFO& csbi) const
	{
		ASSERT(IsValid());
		return ::GetConsoleScreenBufferInfo(*this, &csbi);
	}

	BOOL SetConsoleScreenBufferSize(COORD dwSize) const
	{
		ASSERT(IsValid());
		return ::SetConsoleScreenBufferSize(*this, dwSize);
	}
	BOOL SetConsoleScreenBufferInfoEx(CONSOLE_SCREEN_BUFFER_INFOEX& csbi) const
	{
		ASSERT(IsValid());
		return ::SetConsoleScreenBufferInfoEx(*this, &csbi);
	}
};




_MZH_END

#endif
