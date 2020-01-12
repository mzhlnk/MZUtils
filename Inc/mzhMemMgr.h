#pragma once

#ifndef _MZH_MEMMGR
#define _MZH_MEMMGR

#include <mzhBase.h>

_MZH_BEGIN

class IMemMgr
{
public:
	IMemMgr(void) { }
	virtual ~IMemMgr(void) { }

public:
	virtual LPVOID Alloc(SIZE_T cb) = 0;
	virtual LPVOID ReAlloc(LPVOID lp, SIZE_T cb) = 0;
	virtual void Free(LPVOID lp) = 0;
};

class CMalloc : public IMemMgr
{
public:
	CMalloc() {}
	virtual ~CMalloc() {}

public:
	virtual LPVOID Alloc(SIZE_T cb)
	{
		return malloc(cb);
	}
	virtual LPVOID ReAlloc(LPVOID lp, SIZE_T cb)
	{
		return realloc(lp, cb);
	}
	virtual void Free(LPVOID lp)
	{
		free(lp);
	}
};

template<typename T>
class IMemMgrT
{
public:
	IMemMgrT(void) { }
	virtual ~IMemMgrT(void) { }

public:
	virtual T* Alloc(SIZE_T nCount) = 0;
	virtual T* ReAlloc(T* ptr, SIZE_T nNewCount) = 0;
	virtual void Free(T* lp) = 0;
};

template<typename T>
class CMallocT : public IMemMgrT<T>
{
public:
	CMallocT() {}
	virtual ~CMallocT() {}

public:
	virtual T* Alloc(SIZE_T nCount)
	{
		return (T*)malloc(sizeof(T) * nCount);
	}
	virtual T* ReAlloc(T* ptr, SIZE_T nNewCount)
	{
		return (T*)realloc(ptr, sizeof(T) * nNewCount);
	}
	virtual void Free(T* ptr)
	{
		free(ptr);
	}
};

_MZH_END

#endif
