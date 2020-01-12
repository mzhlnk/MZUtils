#pragma once

#ifndef _MZH_WND_STATION
#define _MZH_WND_STATION

#include <mzhBase.h>

_MZH_BEGIN

template<bool t_bManaged>
class CWindowStationT : public CHObjT<HWINSTA, t_bManaged>
{
public:
	CWindowStationT(HWINSTA hWinSta = NULL)
		: CHObjT<HWINSTA, t_bManaged>(hWinSta)
	{ }

	virtual ~CWindowStationT(void)
	{
		if(t_bManaged)
			Release();
	}

public:
	virtual BOOL Close()
	{
		ASSERT(IsValid());
		if(!::CloseWindowStation(_hObj))
			return FALSE;
		_hObj = NULL;
		return TRUE;
	}

public:
	BOOL Create(LPCTSTR lpszWinSta, DWORD dwFlags, ACCESS_MASK dwDesiredAccess, LPSECURITY_ATTRIBUTES lpsa)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateWindowStation(lpszWinSta, dwFlags, dwDesiredAccess, lpsa));
	}
	HWINSTA Open(LPCTSTR lpszWinSta, BOOL bInherit, ACCESS_MASK dwDesiredAccess)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::OpenWindowStation(lpszWinSta, bInherit, dwDesiredAccess));
	}

public:
	static HWINSTA GetProcessWinowStation()
	{ return ::GetProcessWindowStation(); }
	BOOL GetUserObjectInformation(int nIndex, PVOID pvInfo, DWORD nLength, LPDWORD lpnLengthNeeded)
	{
		ASSERT(IsValid());
		return ::GetUserObjectInformation(*this, nIndex, pvInfo, DWORD nLength, lpnLengthNeeded);
	}
	BOOL GetUserObjectSecurity(PSECURITY_INFORMATION pSIRequested, PSECURITY_DESCRIPTOR pSD, DWORD nLength, LPDWORD lpnLengthNeeded)
	{
		ASSERT(IsValid());
		return ::GetUserObjectSecurity(*this, pSIRequested, pSD, nLength, lpnLengthNeeded);
	}

public:
	BOOL SetProcessWindowStation()
	{
		ASSERT(IsValid());
		return ::SetProcessWindowStation(*this);
	}
	BOOL SetUserObjectInformation(int nIndex, PVOID pvInfo, DWORD nLength)
	{
		ASSERT(IsValid());
		return ::SetUserObjectInformation(*this, nIndex, pvInfo, nLength);
	}
	BOOL SetUserObjectSecurity(PSECURITY_INFORMATION pSIRequested, PSECURITY_DESCRIPTOR pSID)
	{
		ASSERT(IsValid());
		return ::SetUserObjectSecurity(*this, pSIRequested, pSID);
	}

public:
	BOOL EnumDesktops(DESKTOPENUMPROC lpEnumFunc, LPARAM lParam)
	{
		ASSERT(IsValid());
		return ::EnumDesktops(*this, lpEnumFunc, lParam);
	}
};

typedef CWindowStationT<true> CWindowStation;
typedef CWindowStationT<false> CWindowStationHandle;

_MZH_END

#endif
