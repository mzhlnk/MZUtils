#pragma once

#ifndef _MZH_BUFFER
#define _MZH_BUFFER

#include <vector>
#include <mzhMemMgr.h>
#include <mzhNotify.h>

_MZH_BEGIN

const SIZE_T KB = (1 << 10);
const SIZE_T MB = (1 << 20);
const SIZE_T GB = (1 << 30);

template<typename T>
class CSimpleBufferT : public CAutoPtrT<T>
{
private:
	CSimpleBufferT(CSimpleBufferT&) {}	// not impl
public:
	CSimpleBufferT()
		: CAutoPtrT<T>()
		, _Size(0)
	{}
	CSimpleBufferT(SIZE_T nSize, const T* pData = NULL)
		: CAutoPtrT<T>()
		, _Size(0)
	{
		ASSERT(nSize);
		if(nSize)
		{
			_Ptr = (T*)malloc(nSize * sizeof(T));
			ASSERT(_Ptr);
			if(_Ptr)
			{
				_Size = nSize;
				if(pData)
					memcpy(_Ptr, pData, _Size * sizeof(T));
			}
		}
	}
	virtual ~CSimpleBufferT()
	{
		Release();
	}

protected:
	SIZE_T	_Size;

public:
	SIZE_T	Size() const	{ return _Size; }
	SIZE_T	Bytes() const	{ return _Size * sizeof(T); }

public:
	T* ReAlloc(SIZE_T nSize)
	{
		ASSERT(nSize);
		T* ptr = (T*)realloc(_Ptr, nSize * sizeof(T));
		if(ptr)
		{
			_Ptr = ptr;
			_Size = nSize;
		}
		return ptr;
	}
	virtual void Release()
	{
		__super::Release();
		_Size = 0;
	}
	BOOL SetData(const T* pData, SIZE_T nSize)
	{
		ASSERT(pData && nSize);
		T* ptr = (T*)malloc(nSize * sizeof(T));
		ASSERT(ptr);
		if(!ptr)
			return FALSE;
		memcpy(ptr, pData, nSize * sizeof(T));
		Attach(ptr);
		_Size = nSize;
		return TRUE;
	}
};

class CBuffer : protected _MZH CMalloc
{
public:
	CBuffer()
		: m_pData(NULL)
		, m_nUsed(0)
		, m_nSize(0)
	{ }
	CBuffer(SIZE_T cb)
		: m_nUsed(0)
	{
		if (m_pData = (LPBYTE)Alloc(cb))
			m_nSize = cb;
	}
	virtual ~CBuffer()
	{
		ReleaseBuffer();
	}

protected:
	LPBYTE	m_pData;	// buffer pointer
	SIZE_T	m_nUsed;	// bytes of data
	SIZE_T	m_nSize;	// bytes of buffer

public:
	operator LPBYTE() const { return m_pData; }
public:
	LPBYTE GetBuffer()
	{
		return m_pData;
	}
	SIZE_T GetSize() const
	{
		return m_nUsed;
	}
	LPBYTE GetBuffer(SIZE_T cb, SIZE_T offset = 0)
	{
		SIZE_T size = offset + cb;
		if (size > m_nSize)
		{
			LPBYTE pData = (LPBYTE)ReAlloc(m_pData, size);
			if (!pData)
				return NULL;
			memset(pData + m_nUsed, 0, size - m_nUsed);
			m_pData = pData;
			m_nSize = size;
		}
		return (m_pData + offset);
	}
	SIZE_T GetBufferLength() const
	{
		return m_nSize;
	}
	void ReleaseBuffer()
	{
		if (m_pData)
		{
			Free(m_pData);
			m_pData = NULL;
			m_nUsed = 0;
			m_nSize = 0;
		}
	}
	BOOL ReleaseBufferSetLength(SIZE_T cb)
	{
		if (cb == m_nSize)
		{
			m_nUsed = m_nSize;
		}
		else if (0 == cb)
		{
			ReleaseBuffer();
		}
		else if (cb != m_nSize)
		{
			ASSERT(m_pData);
			LPBYTE pData = (LPBYTE)ReAlloc(m_pData, cb);
			if (!pData)
				return FALSE;
			if (cb > m_nSize)
			{
				SIZE_T nInc = cb - m_nSize;
				memset(pData + m_nSize, 0, nInc);
			}
			m_pData = pData;
			m_nSize = cb;
			m_nUsed = cb;
		}

		return TRUE;
	}

public:
	BOOL GetData(LPVOID lpBuffer, SIZE_T cb, SIZE_T offset = 0)
	{
		ASSERT(lpBuffer && cb);
		if (offset + cb > m_nUsed)
			return FALSE;
		ASSERT(m_pData);
		memcpy(lpBuffer, m_pData + offset, cb);
		return TRUE;
	}
	template<typename T>
	BOOL GetValue(T& val, SIZE_T offset = 0)
	{
		if (offset + sizeof(T) > m_nUsed)
			return FALSE;
		ASSERT(m_pData);
		val = *((T*)(m_pData + offset));
		return TRUE;
	}

public:
	BOOL SetData(LPCVOID lpData, SIZE_T cb, SIZE_T offset = 0)
	{
		ASSERT(lpData && cb);
		LPBYTE pBuffer = GetBuffer(cb, offset);
		if (!pBuffer)
			return FALSE;
		memcpy(pBuffer, lpData, cb);
		offset += cb;
		if (m_nUsed < offset)
			m_nUsed = offset;
		return TRUE;
	}
	BOOL AppendData(LPCVOID lpData, SIZE_T cb)
	{
		return SetData(lpData, cb, m_nUsed);
	}

public:
	BOOL Delete(int offset, int bytes = -1)
	{
		if (m_pData)
		{
			if (offset < (int)m_nUsed && bytes)
			{
				int idx = bytes < 0 ? (int)m_nUsed : offset + bytes;
				if (idx >= (int)m_nUsed)
				{
					m_nUsed = offset;
				}
				else
				{
					size_t len = m_nUsed - idx;
					memcpy(m_pData + offset, m_pData + idx, len);
					m_nUsed = offset + len;
				}
			}
			return TRUE;
		}
		return FALSE;
	}

public:
	template<typename T>
	BOOL SetValue(const T& val, SIZE_T offset)
	{
		T* ptr = (T*)GetBuffer(sizeof(val), offset);
		if (!ptr)
			return FALSE;
		*ptr = val;
		offset += sizeof(T);
		if (m_nUsed < offset)
			m_nUsed = offset;
		return TRUE;
	}
	template<typename T>
	BOOL AppendValue(const T& val)
	{
		return SetValue(val, m_nUsed);
	}
};


template<typename T, class BaseT = _MZH CMallocT<T>>
class CBufferT : protected BaseT
{
public:
	CBufferT()
		: m_ptr(NULL)
		, m_nSize(0)
		, m_nUsed(0)
	{
	}
	CBufferT(SIZE_T nSize = MB)
		: m_ptr(Alloc(nSize))
		, m_nSize(nSize)
		, m_nUsed(0)
	{
	}
	virtual ~CBufferT()
	{
		ReleaseBuffer();
	}

protected:
	T*		m_ptr;
	SIZE_T	m_nSize;
	SIZE_T	m_nUsed;

public:
	operator T* () const
	{ return m_ptr; }
public:
	BOOL IsEmpty() const
	{
		return (!m_ptr || !m_nSize);
	}
	SIZE_T GetSize() const
	{ return m_nUsed; }
	SIZE_T GetBufferLength() const
	{ return m_nSize; }
	BOOL SetBufferLength(SIZE_T nSize)
	{
		if (nSize == 0)
		{
			ReleaseBuffer();
		}
		else if (nSize != m_nSize)
		{
			T* ptr = ReAlloc(m_ptr, nSize);
			if (!ptr)
				return FALSE;
			m_ptr = ptr;
			m_nSize = nSize;
		}
		return TRUE;
	}
	const T operator [](int n)
	{
		ASSERT(n < m_nUsed);
		T val = m_ptr[n];
		return val;
	}

public:
	void ReleaseBuffer()
	{
		if (m_ptr)
		{
			Free(m_ptr);
			m_ptr = NULL;
			m_nSize = 0;
			m_nUsed = 0;
		}
	}

public:
	BOOL Append(const T* pData, SIZE_T nCount)
	{
		ASSERT(pData && nCount);
		if (m_nUsed + nCout > m_nSize &&
			!SetSize(m_nUsed + nCount))
			return FALSE;
		memcpy(m_ptr + m_nUsed, pData, nCount);
		m_nUsed += nCount;
		return TRUE;
	}
	SIZE_T GetData(T* pBuffer, SIZE_T nBufLen, SIZE_T nStart = 0)
	{
		ASSERT(pBuffer && nBufLen);
		SIZE_T nRead = m_nUsed - nStart;
		if (nRead > nBufLen)
			nRead = nBufLen;
		memcpy(pBuffer, m_ptr + nStart, nRead);
		return nRead;
	}
	BOOL Delete(int offset, int count = -1)
	{
		if (m_pData)
		{
			if (offset < (int)m_nUsed && count)
			{
				int idx = count < 0 ? m_nUsed : offset + count;
				if (idx >= (int)m_nUsed)
				{
					m_nUsed = offset;
				}
				else
				{
					size_t len = m_nUsed - idx;
					memcpy(m_pData + offset, m_pData + idx, sizeof(T) * len);
					m_nUsed = offset + len;
				}
			}
			return TRUE;
		}
		return FALSE;
	}
};

template<typename T>
class CSharedBufferT
	: protected CBufferT<T>
	, public _MZH CSingleNotify
{
public:
	CSharedBufferT(INotify* pNotify = NULL, SIZE_T nSize = MB)
		: CBufferT(nSize)
		, CSingleNotify(pNotify)
	{
	}
	virtual ~CSharedBufferT()
	{
		ReleaseBuffer();
	}

protected:
	_MZH CCriticalSection m_cs;

public:
	using CBufferT::IsEmpty;
	using CBufferT::GetSize;
	const T operator [](int n)
	{
		m_cs.Lock();
		T val = __super::operator[](n);
		m_cs.Unlock();
		return val;
	}

public:
	void ReleaseBuffer()
	{
		m_cs.Lock();
		__super::ReleaseBuffer();
		m_cs.Unlock();
	}

public:
	BOOL Append(const T* pData, SIZE_T nCount)
	{
		ASSERT(pData && nCount);
		m_cs.Lock();
		__super::Append(pData, nCount);
		m_cs.Unlock();
		PostNotify(nCount);
		return TRUE;
	}
	SIZE_T GetData(T* pBuffer, SIZE_T nBufLen, SIZE_T nStart = 0)
	{
		m_cs.Lock();
		SIZE_T nRead = __super::GetData(pBuffer, nBufLen, nStart);
		m_cs.Unlock();
		return nRead;
	}
};


template<typename T, class BaseT = _MZH CMallocT<T>>
class CBlockBufferT : protected BaseT
{
public:
	CBlockBufferT(SIZE_T nBlockSize = MB)
		: m_nBlockSize(nBlockSize)
		, m_nFreeCount(0)
	{
	}
	virtual ~CBlockBufferT()
	{
		CBlockBufferT::ReleaseAll();
	}

protected:
	SIZE_T m_nBlockSize;
	SIZE_T m_nFreeCount;
	std::vector<T*> m_vtr;

public:
	BOOL IsEmpty() const
	{ return m_vtr.empty(); }
	SIZE_T GetBlockSize() const
	{ return m_vtr.size(); }
	SIZE_T GetSize()
	{
		SIZE_T n = (m_vtr.size() * m_nBlockSize - m_nFreeCount);
		return n;
	}
	const T operator [](int n)
	{
		ASSERT(n < GetSize());
		T val = m_vtr[n / m_nBlockSize][n % m_nBlockSize];
		return val;
	}

public:
	BOOL ReleaseBlock(SIZE_T idx)
	{
		if (idx < m_vtr.size())
		{
			Free(m_vtr[idx]);
			m_vtr.erase(idx);
			if (m_vtr.size() == idx)
				m_nFreeCount = 0;
			return TRUE;
		}
		return FALSE;
	}
	virtual void ReleaseAll()
	{
		for(auto var : m_vtr)
		{
			Free(var);
		}
		m_vtr.clear();
		m_nFreeCount = 0;
	}

public:
	BOOL Append(const T* pData, SIZE_T nCount)
	{
		ASSERT(pData && nCount);
		T* pDest = m_vtr.empty() ? NULL : (m_vtr.back() + m_nBlockSize - m_nFreeCount);
		while (nCount)
		{
			if (m_nFreeCount < 1)
			{
				pDest = Alloc(m_nBlockSize);
				m_nFreeCount = m_nBlockSize;
				m_vtr.push_back(pDest);
			}
			if (m_nFreeCount >= nCount)
			{
				memcpy(pDest, pData, nCount);
				m_nFreeCount -= nCount;
				break;
			}
			else
			{
				memcpy(pDest, pData, m_nFreeCount);
				m_nFreeCount = 0;
				nCount -= m_nFreeCount;
				pData += m_nFreeCount;
			}
		}
		return TRUE;
	}
	SIZE_T GetData(T* pBuffer, SIZE_T nBufLen, SIZE_T nStart = 0)
	{
		ASSERT(pBuffer && nBufLen);
		SIZE_T nRead = 0;
		SIZE_T idx1 = nStart / m_nBlockSize;
		SIZE_T idx2 = nStart % m_nBlockSize;
		if (idx1 < m_vtr.size())
		{
			T* pData = m_vtr[idx1++];
			SIZE_T nLen = m_vtr.back() == pData ? m_nBlockSize - m_nFreeCount : m_nBlockSize;
			pData += idx2;
			nLen -= idx2;
			nRead = min(nBufLen, nLen);
			memcpy(pBuffer, pData, nRead);
			while (nRead < nBufLen && idx1 < m_vtr.size())
			{
				pData = m_vtr[idx1++];
				SIZE_T nLen = m_vtr.back() == pData ? m_nBlockSize - m_nFreeCount : m_nBlockSize;
				SIZE_T n = min(nBufLen, nLen);
				memcpy(pBuffer + nRead, pData, n);
				nRead += n;
			}
		}
		return nRead;
	}
};

template<typename T>
class CSharedBlockBufferT
	: protected CBlockBufferT<T>
	, public _MZH CSingleNotify
{
public:
	CSharedBlockBufferT(INotify* pNotify = NULL, SIZE_T nBlockSize = MB)
		: CBlockBufferT(nBlockSize)
		, CSingleNotify(pNotify)
	{
	}
	virtual ~CSharedBlockBufferT()
	{
		CSharedBlockBufferT::ReleaseAll();
	}

protected:
	_MZH CCriticalSection m_cs;

public:
	using CBlockBufferT::IsEmpty;
	using CBlockBufferT::GetBlockSize;
	SIZE_T GetSize()
	{
		m_cs.Lock();
		SIZE_T n = __super::GetSize();
		m_cs.Unlock();
		return n;
	}
	const T operator [](int n)
	{
		m_cs.Lock();
		T val = __super::operator[](n);
		m_cs.Unlock();
		return val;
	}

public:
	BOOL DeleteBlock(SIZE_T idx)
	{
		m_cs.Lock();
		BOOL bRet = _super::DeleteBlock(idx);
		m_cs.Unlock();
		return bRet;
	}
	void ReleaseAll()
	{
		m_cs.Lock();
		__super::ReleaseAll();
		m_cs.Unlock();
	}

public:
	BOOL Append(const T* pData, SIZE_T nCount)
	{
		ASSERT(pData && nCount);
		m_cs.Lock();
		__super::Append(pData, nCount);
		m_cs.Unlock();
		PostNotify(nCount);
		return TRUE;
	}
	SIZE_T GetData(T* pBuffer, SIZE_T nBufLen, SIZE_T nStart = 0)
	{
		m_cs.Lock();
		SIZE_T nRead = __super::GetData(pBuffer, nBufLen, nStart);
		m_cs.Unlock();
		return nRead;
	}
};

_MZH_END

#endif
