#pragma once

#ifndef _MZH_QUEUE
#define _MZH_QUEUE

#include <queue>
#include <mzhNotify.h>

_MZH_BEGIN

template<class T>
class CQueueMethodT
{
public:
	T Pop(std::queue<T>& queue)
	{
		T val = queue.front();
		queue.pop();
		return val;
	}
	void Empty(std::queue<T>& queue)
	{
		while (!queue.empty())
			queue.pop();
	}
};

template<typename PTR>
class CPtrQueueMethodT
{
public:
	PTR Pop(std::queue<PTR>& queue)
	{
		if (queue.empty())
			return NULL;
		PTR val = queue.front();
		queue.pop();
		return val;
	}
	void Empty(std::queue<PTR>& queue)
	{
		while (!queue.empty())
		{
			ASSERT(queue.front());
			delete queue.front();
			queue.pop();
		}
	}
};

template<typename T, class TBase>
class CQueueImplT : protected TBase
{
public:
	CQueueImplT()
	{ }
	virtual ~CQueueImplT()
	{
		Empty();
	}

protected:
	std::queue<T>			m_queue;
	_MZH CCriticalSection	m_cs;

public:
	BOOL IsEmpty()
	{
		return m_queue.empty();
	}
	size_t GetSize()
	{
		return m_queue.size();
	}

public:
	BOOL Push(T val)
	{
		m_cs.Lock();
		m_queue.push(val);
		m_cs.Unlock();
		return TRUE;
	}

	T Pop()
	{
		CSingleLock sl(&m_cs, TRUE);
		return TBase::Pop(m_queue);
	}
	BOOL Pop(T& val)
	{
		CSingleLock sl(&m_cs, TRUE);
		if (m_queue.empty())
			return FALSE;
		val = m_queue.front();
		m_queue.pop();
		return TRUE;
	}

public:
	void Empty()
	{
		TBase::Empty(m_queue);
	}
};

template<typename T> 
class CQueueT : public CQueueImplT<T, CQueueMethodT<T>>
{};

template<class T>
class CPtrQueueT : public CQueueImplT<T, CPtrQueueMethodT<T>>
{};


template<typename T, class TBase, class CIN = _MZH INotify, class CON = _MZH INotify>
class CFIFOImplT : public TBase
{
public:
	CFIFOImplT(_MZH INotify* pPushNotify = NULL, _MZH INotify* pPopNotify = NULL)
		: m_snPush(pPushNotify)
		, m_snPop(pPopNotify)
	{ }

	virtual ~CFIFOImplT()
	{
		Empty();
	}

protected:
	CIN* m_snPush;
	CON* m_snPop;

public:
	BOOL Push(T val)
	{
		if (!TBase::Push(val))
			return FALSE;
		if(m_snPush)
			m_snPush->PostNotify(1);
		return TRUE;
	}
	T Pop()
	{
		T val = TBase::Pop();
		if (m_snPop)
			m_snPop->PostNotify(-1);
		return val;
	}
	BOOL Pop(T& val)
	{
		if (!TBase::Pop(val))
			return FALSE;
		if (m_snPop)
			m_snPop->PostNotify(-1);
		return TRUE;
	}

public:
	void Empty()
	{
		if (!IsEmpty())
		{
			TBase::Empty();
			if (m_snPop)
				m_snPop->PostNotify(-1);
		}
	}
};

template<typename T, class CIN = _MZH CNotify, class CON = _MZH CNotify>
class CFIFOT : public CFIFOImplT<T, CQueueT<T>, CIN, CON>
{
public:
	CFIFOT(_MZH INotify* pPushNotify = NULL, _MZH INotify* pPopNotify = NULL)
		: CFIFOImplT(pPushNotify, pPopNotify)
	{ }
};

template<class T, class CIN = _MZH INotify, class CON = _MZH INotify>
class CPtrFIFOT : public CFIFOImplT<T, CPtrQueueT<T>, CIN, CON>
{
public:
	CPtrFIFOT(_MZH INotify* pPushNotify = NULL, _MZH INotify* pPopNotify = NULL)
		: CFIFOImplT(pPushNotify, pPopNotify)
	{ }
};

_MZH_END

#endif
