#pragma once

#ifndef _MZH_SYNC
#define _MZH_SYNC

#include <mzhBase.h>

_MZH_BEGIN

#define HandleException(b)	ASSERT(b)

class ISync
{
public:
	ISync() { }
	virtual ~ISync() = 0 { }

public:
	virtual BOOL Lock(DWORD dwTimeout = INFINITE) = 0;
	virtual BOOL Unlock() = 0;
};

template<bool t_bManaged>
class CSyncObjT : public CHandleT<t_bManaged>, public ISync
{
public:
	CSyncObjT(HANDLE h = NULL)
		: CHandleT<t_bManaged>(h)
	{ }

public:
	virtual BOOL Lock(DWORD dwTimeout = INFINITE)
	{
		ASSERT(__super::IsValid());
		DWORD dwVal = WaitForSingleObject(*this, dwTimeout);
		if (WAIT_OBJECT_0 == dwVal)
			return TRUE;
		if(WAIT_TIMEOUT != dwVal)
			TRACE1("Failed to WaitForSingleObject: %d\n", dwVal);
		return FALSE;
	}
};

template<bool t_bManaged>
class CMutexT : public CSyncObjT<t_bManaged>
{
public:
	explicit CMutexT(HANDLE h = NULL)
		: CSyncObjT<t_bManaged>(h)
	{ }

public:
	using CSyncObjT<t_bManaged>::operator=;
public:
	virtual BOOL Unlock(void)
	{ return ReleaseMutex(*this); }
};
typedef CMutexT<false> CMutexHandle;
class CMutex
	: public CMutexT<true>
{
public:
	explicit CMutex(HANDLE h)
		: CMutexT<true>(h)
	{ }
	explicit CMutex(BOOL bInitialOwner = FALSE, LPCTSTR lpName = NULL, LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL)
		: CMutexT<true>(CreateMutex(lpMutexAttributes, bInitialOwner, lpName))
	{ }
};

template<bool t_bManaged>
class CEventT : public CSyncObjT<t_bManaged>
{
public:
	explicit CEventT(HANDLE h = NULL)
		: CSyncObjT<t_bManaged>(h)
	{ }

public:
	using CSyncObjT<t_bManaged>::operator=;
public:
	virtual BOOL Unlock() { return TRUE; }

public:
	BOOL SetEvent(void) { return ::SetEvent(*this); }
	BOOL ResetEvent(void) { return ::ResetEvent(*this); }
	BOOL PulseEvent(void) { return ::PulseEvent(*this); }
};
typedef CEventT<false> CEventHandle;
class CEvent
	: public CEventT<true>
{
public:
	explicit CEvent(HANDLE h)
		: CEventT<true>(h)
	{ }
	explicit CEvent(BOOL bInitiallyOwn = FALSE, BOOL bManualReset = FALSE, LPCTSTR lpName = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL)
		: CEventT<true>(CreateEvent(lpsaAttribute, bManualReset, bInitiallyOwn, lpName))
	{ }
};
class CAutoEvent : public CEvent
{
public:
	explicit CAutoEvent(BOOL bInitiallyOwn = FALSE, LPCTSTR lpName = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL)
		: CEvent(bInitiallyOwn, FALSE, lpName, lpsaAttribute)
	{ }
};
class CManualEvent : public CEvent
{
public:
	explicit CManualEvent(BOOL bInitiallyOwn = FALSE, LPCTSTR lpName = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL)
		: CEvent(bInitiallyOwn, TRUE, lpName, lpsaAttribute)
	{ }
};

//class CSemaphore : public CSyncObject
//{
//public:
//	explicit CSemaphore(LONG lInitialCount = 1, LONG lMaxCount = 1,
//		LPCTSTR pstrName = NULL, LPSECURITY_ATTRIBUTES lpsaAttributes = NULL);
//public:
//	virtual ~CSemaphore();
//public:
//	virtual BOOL Unlock();
//	virtual BOOL Unlock(LONG lCount, LPLONG lprevCount = NULL);
//};

class CCriticalSection : public ISync
{
public:
	CCriticalSection(DWORD dwSpinCount = 0)
	{
		HRESULT hr = Init(dwSpinCount);
		HandleException(SUCCEEDED(hr));
	}
	virtual ~CCriticalSection()
	{ ::DeleteCriticalSection(&m_sect); }

protected:
	CRITICAL_SECTION m_sect;

public:
	operator CRITICAL_SECTION*() { return &m_sect; }

public:
	BOOL SetCriticalSectionSpinCount(DWORD dwSpinCount)
	{ return ::SetCriticalSectionSpinCount(&m_sect, dwSpinCount); }
	BOOL TryLock()
	{ return TryEnterCriticalSection(&m_sect); }

private:
	virtual BOOL Lock(DWORD dwTimeout)
	{
		ASSERT(dwTimeout == INFINITE);
		return Lock();
	}

public:
	virtual BOOL Lock()
	{
//#if (WINVER >= 0x0501) || (_WIN32_WINNT >= 0x0501)
		EnterCriticalSection(&m_sect);
//#else
//		__try
//		{
//			EnterCriticalSection(&m_sect);
//		}
//		__except (STATUS_NO_MEMORY == GetExceptionCode())
//		{
//			//AfxThrowMemoryException();
//			return FALSE;
//		}
//#endif
		return TRUE;
	}
	virtual BOOL Unlock()
	{
		LeaveCriticalSection(&m_sect);
		return TRUE;
	}

private:
	HRESULT Init(DWORD dwSpinCount = 0)
	{
		//#if (WINVER >= 0x0600) || (_WIN32_WINNT >= 0x0600)
		//#else
		//		__try
		//		{
		//			InitializeCriticalSection(&m_sect);
		//		}
		//		__except (STATUS_NO_MEMORY == GetExceptionCode())
		//		{
		//			ZeroMemory(&m_sect, sizeof(m_sect));
		//		}
		//#endif
		if (!InitializeCriticalSectionAndSpinCount(&m_sect, dwSpinCount))
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}
		return S_OK;
	}
};

class CSingleLock
{
public:
	explicit CSingleLock(ISync* pSync, BOOL bInitialLock = FALSE)
		: m_pSync(pSync)
		, m_bAcquired(FALSE)
	{
		if (bInitialLock)
			Lock();
	}
	~CSingleLock()
	{
		Unlock();
	}

protected:
	ISync*	m_pSync;
	BOOL	m_bAcquired;

public:
	BOOL Lock(DWORD dwTimeOut = INFINITE)
	{
		ASSERT(m_pSync && !m_bAcquired);
		m_bAcquired = m_pSync->Lock();
		return m_bAcquired;
	}
	BOOL Unlock()
	{
		ASSERT(m_pSync);
		if (m_bAcquired)
			m_bAcquired = !m_pSync->Unlock();
		return !m_bAcquired;
	}
	//BOOL Unlock(LONG lCount, LPLONG lPrevCount = NULL)
	//{
	//}
	BOOL IsLocked()
	{ return m_bAcquired; }
};

//class CMultiLock
//{
//	// Constructor
//public:
//	CMultiLock(CSyncObject* ppObjects[], DWORD dwCount, BOOL bInitialLock = FALSE);
//
//	// Operations
//public:
//	DWORD Lock(DWORD dwTimeOut = INFINITE, BOOL bWaitForAll = TRUE,
//		DWORD dwWakeMask = 0);
//	BOOL Unlock();
//	BOOL Unlock(LONG lCount, LPLONG lPrevCount = NULL);
//	BOOL IsLocked(DWORD dwItem);
//
//	// Implementation
//public:
//	~CMultiLock();
//
//protected:
//	HANDLE  m_hPreallocated[8];
//	BOOL    m_bPreallocated[8];
//
//	CSyncObject* const * m_ppObjectArray;
//	HANDLE* m_pHandleArray;
//	BOOL*   m_bLockedArray;
//	DWORD   m_dwCount;
//};

_MZH_END

#endif
