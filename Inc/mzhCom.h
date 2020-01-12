#pragma once


#ifndef _MZH_COM
#define _MZH_COM

#include <mzhBase.h>

_MZH_BEGIN


class CFakeCritSection
{
public:
	HRESULT Lock()		{ return S_OK; }
	HRESULT Unlock()	{ return S_OK; }
	HRESULT Init()		{ return S_OK; }
	HRESULT Term()		{ return S_OK; }
};

class CCritSection
{
public:
	CCritSection() {
		ZeroMemory(&m_sec, sizeof(m_sec));
	}
	~CCritSection() { }

protected:
	CRITICAL_SECTION m_sec;

public:
	operator CRITICAL_SECTION*() { return &m_sec; }

public:
	HRESULT Lock() {
		EnterCriticalSection(&m_sec);
		return S_OK;
	}
	HRESULT Unlock() {
		LeaveCriticalSection(&m_sec);
		return S_OK;
	}
	HRESULT Init() {
		HRESULT hr = S_OK;
		if (!InitializeCriticalSectionEx(&m_sec, 0, 0))
			hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}
	HRESULT Term() {
		DeleteCriticalSection(&m_sec);
		return S_OK;
	}
	HRESULT TryLock() {
		if (!TryEnterCriticalSection(&m_sec))
			return E_FAIL;
		return S_OK;
	}
};

template<class TLock>
class CLockHelperT
{
public:
	CLockHelperT(TLock lock) : _Lock(lock) {
		lock.Lock();
	}
	~CLockHelperT() {
		_Lock.Unlock();
	}

protected:
	TLock& _Lock;
};

class CSingleThreadModel
{
public:
	static LONG Increment(LPLONG p) {
		return ++(*p);
	}
	static LONG Decrement(LPLONG p) {
		return --(*p);
	}
};

class CMultiThreadModel
{
public:
	static LONG Increment(LPLONG p) {
		return ::InterlockedIncrement(p);
	}
	static LONG Decrement(LPLONG p) {
		return ::InterlockedDecrement(p);
	}
};

template <class _ThreadModel>
class CComBaseNoLockT
{
public:
	CComBaseNoLockT() : m_dwRef(0) {}
	~CComBaseNoLockT() {}

protected:
	long	m_dwRef;

public:
	ULONG InternalAddRef() {
		return _ThreadModel::Increment(&m_dwRef);
	}
	ULONG InternalRelease() {
		return _ThreadModel::Decrement(&m_dwRef);
	}
	HRESULT FinalConstruct() {
		return S_OK;
	}
	void FinalRelease() { }
};

template <class Base>
class CComObjectNoLockT :
	public Base
{
public:
	CComObjectNoLockT(LPVOID = NULL) { }
	virtual ~CComObjectNoLockT() {
		FinalRelease();
	}

	STDMETHOD_(ULONG, AddRef)() {
		return InternalAddRef();
	}
	STDMETHOD_(ULONG, Release)() {
		ULONG l = InternalRelease();
		if (l == 0)
			delete this;
		return l;
	}
	//if InternalQueryInterface is undefined then you forgot BEGIN_COM_MAP
	STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppvObj)  {
		if (NULL == ppvObj)
			return E_POINTER;
		return InternalQueryInterface(iid, ppvObj);
	}

	static HRESULT CreateInstance(CComObjectNoLockT<Base>** pp) throw()
	{
		ASSERT(pp);
		if (pp == NULL)
			return E_POINTER;
		*pp = NULL;

		HRESULT hr = E_OUTOFMEMORY;
		CComObjectNoLockT<Base>* p = new CComObjectNoLockT<Base>();
		if (p)
		{
			p->InternalAddRef();
			hr = p->FinalConstruct();
			if (hr != S_OK)
			{
				delete p;
				p = NULL;
			}
		}
		*pp = p;
		return hr;
	}
};

template <class _ThreadModel, class TLock = CCritSection>
class CComBaseT
{
public:
	typedef CLockHelperT<TLock> CLockHelper;
public:
	CComBaseT()
		: m_dwRef(0)
	{}
	~CComBaseT() {}

protected:
	long	m_dwRef;
	TLock	m_Lock;

public:
	ULONG InternalAddRef() {
		return _ThreadModel::Increment(&m_dwRef);
	}
	ULONG InternalRelease() {
		return _ThreadModel::Decrement(&m_dwRef);
	}
	HRESULT FinalConstruct() { return m_Lock.Init(); }
	void FinalRelease() { m_Lock.Term(); }
	void Lock() { m_Lock.Lock(); }
	void Unlock() { m_Lock.Unlock(); }
};
#define AUTO_LOCK(lock)	CLockHelper __Lock_(lock);
#define COM_AUTO_LOCK()	CLockHelper __Lock_(m_Lock)
template <class Base>
class CComObjectT : public Base
{
public:
	CComObjectT(LPVOID = NULL)
	{ }
	virtual ~CComObjectT() {
		FinalRelease();
	}
	STDMETHOD_(ULONG, AddRef)() {
		return InternalAddRef();
	}
	STDMETHOD_(ULONG, Release)() {
		ULONG l = InternalRelease();
		if (l == 0)
		{
			COM_AUTO_LOCK();
			delete this;
		}
		return l;
	}
	//if _InternalQueryInterface is undefined then you forgot BEGIN_COM_MAP
	STDMETHOD(QueryInterface)(REFIID iid, void** ppvObj) {
		if (NULL == ppvObj)
			return E_POINTER;
		return InternalQueryInterface(iid, ppvObj);
	}

	template <class Q>
	HRESULT STDMETHODCALLTYPE QueryInterface(Q** pp) {
		return QueryInterface(__uuidof(Q), (void**)pp);
	}

	static HRESULT CreateInstance(CComObjectT<Base>** pp)
	{
		ASSERT(pp);
		if (pp == NULL)
			return E_POINTER;
		*pp = NULL;

		HRESULT hr = E_OUTOFMEMORY;
		CComObjectT<Base>* p = new CComObjectT<Base>();
		if (p)
		{
			p->InternalAddRef();
			hr = p->FinalConstruct();
			if (hr != S_OK)
			{
				delete p;
				p = NULL;
			}
		}
		*pp = p;
		return hr;
	}
};

_MZH_END

#endif
