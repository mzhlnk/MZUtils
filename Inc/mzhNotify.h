#pragma once

#ifndef _MZH_NOTIFY
#define _MZH_NOTIFY

#include <set>
#include <mzhSync.h>

_MZH_BEGIN

class INotify
{
public:
	INotify() { }
	virtual ~INotify() = 0 { }

public:
	virtual BOOL PostNotify(WPARAM wParam = 0, LPARAM lParam = 0) = 0;
	virtual LRESULT SendNotify(WPARAM wParam = 0, LPARAM lParam = 0) = 0;
};

class CWndNotify : public INotify
{
public:
	CWndNotify(HWND hWnd, UINT uMsg)
		: m_hWnd(hWnd)
		, m_uMsg(uMsg)
	{ }
	virtual ~CWndNotify() { }

protected:
	HWND	m_hWnd;
	UINT	m_uMsg;

public:
	BOOL PostNotify(WPARAM wParam = 0, LPARAM lParam = 0)
	{
		ASSERT(IsWindow(m_hWnd));
		return PostMessage(m_hWnd, m_uMsg, wParam, lParam);
	}
	LRESULT SendNotify(WPARAM wParam = 0, LPARAM lParam = 0)
	{
		ASSERT(IsWindow(m_hWnd));
		return SendMessage(m_hWnd, m_uMsg, wParam, lParam);
	}
};

class CThreadNotify : public INotify
{
public:
	CThreadNotify(DWORD dwTid, UINT uMsg)
		: m_dwTid(dwTid)
		, m_uMsg(uMsg)
	{ }
	virtual ~CThreadNotify() { }

protected:
	DWORD		m_dwTid;
	UINT		m_uMsg;

public:
	BOOL PostNotify(WPARAM wParam = 0, LPARAM lParam = 0)
	{
		return PostThreadMessage(m_dwTid, m_uMsg, wParam, lParam);
	}

private:
	using INotify::SendNotify;
	LRESULT SendNotify(WPARAM wParam = 0, LPARAM lParam = 0)
	{ return PostNotify(wParam, lParam); }
};

class CEventNotify : public INotify
{
public:
	CEventNotify(HANDLE hEvent)
		: m_notify(hEvent)
	{ }
	virtual ~CEventNotify() { }

protected:
	_MZH CEventHandle m_notify;

public:
	BOOL PostNotify(WPARAM wParam = 0, LPARAM lParam = 0)
	{ return m_notify.SetEvent(); }

private:
	using INotify::SendNotify;
	LRESULT SendNotify(WPARAM wParam = 0, LPARAM lParam = 0)
	{ return PostNotify(wParam, lParam); }
};

class CSingleNotify
{
public:
	explicit CSingleNotify(HWND hWnd, UINT uMsg)
	{
		m_pNotify = new CWndNotify(hWnd, uMsg);
	}
	explicit CSingleNotify(DWORD dwTid, UINT uMsg)
	{
		m_pNotify = new CThreadNotify(dwTid, uMsg);
	}
	explicit CSingleNotify(HANDLE hEvent)
	{
		m_pNotify = new CEventNotify(hEvent);
	}
	virtual ~CSingleNotify()
	{
		DELETE_OBJ(m_pNotify)
	}

protected:
	INotify* m_pNotify;

public:
	void PostNotify(WPARAM wParam = 0, LPARAM lParam = 0)
	{
		m_pNotify->PostNotify(wParam, lParam);
	}
};

class CMultiNotify
{
public:
	CMultiNotify()
	{ }
	virtual ~CMultiNotify()
	{ }
protected:
	_MZH CCriticalSection m_cs;
	std::set<INotify*> m_notify;

public:
	BOOL AddNotify(INotify* pNotify)
	{
		ASSERT(pNotify);
		if (!pNotify)
			return FALSE;
		m_cs.Lock();
		m_notify.insert(pNotify);
		m_cs.Unlock();
		return TRUE;
	}
	void RemoveNotify(INotify* pNotify)
	{
		m_cs.Lock();
		m_notify.erase(pNotify);
		m_cs.Unlock();
	}

public:
	void PostNotify(WPARAM wParam = 0, LPARAM lParam = 0)
	{
		m_cs.Lock();
		for (auto ptr : m_notify)
		{ ptr->PostNotify(wParam, lParam); }
		m_cs.Unlock();
	}
};

_MZH_END

#endif
