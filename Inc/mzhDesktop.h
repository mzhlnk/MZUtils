#pragma once

#ifndef _MZH_DESKTOP
#define _MZH_DESKTOP

#include <mzhBase.h>

_MZH_BEGIN

template<bool t_bManaged>
class CDesktopT : public CHObjT<HDESK, t_bManaged>
{
public:
	CDesktopT(HDESK hDesk = NULL)
		: CHObjT<HDESK, t_bManaged>(hDesk)
	{ }
	virtual ~CDesktopT()
	{
		if (t_bManaged)
			Release();
	}

public:
	virtual BOOL Close()
	{
		ASSERT(IsValid());
		if(!::CloseDesktop(_hObj))
			return FALSE;
		_hObj = NULL;
		return TRUE;
	}

public:
	static HDESK GetThreadDesktop(DWORD dwThreadId)
	{
		return ::GetThreadDesktop(dwThreadId);
	}
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
	BOOL SetThreadDesktop()
	{
		ASSERT(IsValid());
		return ::SetThreadDesktop(*this);
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
	BOOL EnumDesktopWindows(WNDENUMPROC lpfn, LPARAM lParam)
	{
		ASSERT(IsValid());
		return ::EnumDesktopWindows(*this, lpfn, lParam);
	}

public:
	BOOL SwitchDesktop()
	{
		ASSERT(IsValid());
		return ::SwitchDesktop(*this);
	}
};

typedef CDesktopT<false> CDesktopHandle;

class CDesktop : public CDesktopT<true>
{
public:
	CDesktop(HDESK h = NULL)
		: CDesktopT<true>(h)
	{ }

public:
	BOOL Create(LPCTSTR lpszDesktop, LPCTSTR lpszDevice, LPDEVMODE pDevmode, DWORD dwFlags, ACCESS_MASK dwDesiredAccess, LPSECURITY_ATTRIBUTES lpsa)
	{
		ASSERT(!IsValid());
		HDESK hDesk = ::CreateDesktop(lpszDesktop, lpszDevice, pDevmode, dwFlags, dwDesiredAccess, lpsa);
		if(!hDesk)
			return FALSE;
		Attach(h);
		return TRUE;
	}
#if WINVER >= 0x0600
	BOOL CreateEx(LPCTSTR lpszDesktop, LPCTSTR lpszDevice, LPDEVMODE pDevmode, DWORD dwFlags,
		ACCESS_MASK dwDesiredAccess, LPSECURITY_ATTRIBUTES lpsa, ULONG ulHeapSize, PVOID pvoid)
	{
		ASSERT(!IsValid());
		HDESK hDesk = ::CreateDesktopEx(lpszDesktop, lpszDevice, pDevmode, dwFlags, dwDesiredAccess, lpsa, ulHeapSize, pvoid);
		if(!hDesk)
			return FALSE;
		Attach(h);
		return TRUE;
	}
#endif
	BOOL Open(LPTSTR lpszDesktop, DWORD dwFlags, BOOL bInherit, ACCESS_MASK dwDesiredAccess)
	{
		ASSERT(!IsValid());
		HDESK hDesk = ::OpenDesktop(lpszDesktop, dwFlags, bInherit, dwDesiredAccess);
		if(!hDesk)
			return FALSE;
		Attach(h);
		return TRUE;
	}
	BOOL OpenInputDesktop(DWORD dwFlags, BOOL bInherit, ACCESS_MASK dwDesiredAccess)
	{
		ASSERT(!IsValid());
		HDESK hDesk = ::OpenInputDesktop(dwFlags, bInherit, dwDesiredAccess);
		if(!hDesk)
			return FALSE;
		Attach(h);
		return TRUE;
	}
};

_MZH_END

#endif
