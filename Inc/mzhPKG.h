#pragma once

#ifndef _MZH_PKG
#define _MZH_PKG

#include <mzhBuffer.h>

_MZH_BEGIN

class CPKG : public _MZH CBuffer
{
public:
	CPKG()
		: CBuffer()
		, m_nPos(0)
	{ }
	CPKG(size_t cb)
		: CBuffer(cb)
		, m_nPos(0)
	{ }
	virtual ~CPKG(void)
	{ }

protected:
	ULONG m_nPos;

public:
	virtual ULONG GetPos(void) const
	{ return m_nPos; }
	virtual BOOL SetPos(ULONG nPos)
	{
		if (nPos > GetSize())
			return FALSE;

		m_nPos = nPos;
		return TRUE;
	}

public:
	virtual BOOL PushData(LPCVOID pData, ULONG cb)
	{
		if(SetData(pData, cb, m_nPos))
		{
			m_nPos += cb;
			return TRUE;
		}
		return FALSE;
	}
	virtual BOOL PopData(LPVOID pData, ULONG cb)
	{
		if(GetData(pData, cb, m_nPos))
		{
			m_nPos += cb;
			return TRUE;
		}
		return FALSE;
	}

public:
	template<typename T>
	BOOL PushValue(const T& val)
	{
		if(SetValue(val, m_nPos))
		{
			m_nPos += sizeof(T);
			return TRUE;
		}
		return FALSE;
	}
	template<typename T>
	BOOL PopValue(T& val)
	{
		if(GetValue(val, m_nPos))
		{
			m_nPos += sizeof(T);
			return TRUE;
		}
		return FALSE;
	}
};

typedef CPKG* PPKG;

_MZH_END

#endif
