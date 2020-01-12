#pragma once

#ifndef _MZHBASE
#define _MZHBASE

#include <mzhDbg.h>
#include <mzhType.h>

_MZH_BEGIN

template<typename T>
T ASSIGN(T& var, T val)
{ return var = val; }


//#define OVERRIDE_ASSIGN(cls, type)	\
//	cls& operator = (type v) {		\
//		Attach(v);					\
//		return *this;				\
//	}

template<typename T, bool t_bManaged, bool t_bDelNotFree = false, bool t_bArray = false>
class CPtrT
{
	explicit CPtrT(const CPtrT&) {}	// not support
public:
	explicit CPtrT(T* ptr = NULL)
		: _Ptr(ptr)
	{ }
	virtual ~CPtrT()
	{
		if(t_bManaged)
			Release();
	}

protected:
	T* _Ptr;

public:
	T* Ptr() const			{ return _Ptr; }
	operator T*() const		{ return _Ptr; }

public:
	T* operator ->() const	{ return _Ptr; }
	T* operator =(T* p)
	{
		Attach(p);
		return _Ptr;
	}
public:
	virtual void Release()
	{
		if(_Ptr)
		{
			if(!t_bDelNotFree)
				free(_Ptr);
			else if(t_bArray)
				delete [] _Ptr;
			else
				delete _Ptr;
			_Ptr = NULL;
		}
	}
	void Swap(CPtrT& tPtr)
	{
		T* tmp = _Ptr;
		_Ptr = tPtr._Ptr;
		tPtr._Ptr = tmp;
	}
	void Attach(T* ptr)
	{
		if(ptr != _Ptr)
		{
			if(t_bManaged)
				Release();
			_Ptr = ptr;
		}
	}
	T* Detach()
	{
		T* ptr = _Ptr;
		_Ptr = NULL;
		return ptr;
	}
};

template<typename T, bool t_bDelNotFree = false, bool t_bArray = false>
class CAutoPtrT : public CPtrT<T, true, t_bDelNotFree, t_bArray>
{
	explicit CAutoPtrT(const CAutoPtrT&) { }	// not support
public:
	explicit CAutoPtrT(T* p = NULL)
		: CPtrT<T, true, t_bDelNotFree, t_bArray>(p)
	{ }
	using CPtrT<T, true, t_bDelNotFree, t_bArray>::operator =;
};


template<typename T, bool t_bManaged = false, T TNULL = NULL>
class CHObjT
{
	explicit CHObjT(const CHObjT&) {}	// not support
public:
	explicit CHObjT(T hObj = TNULL)
		: _hObj(hObj)
	{ }
	virtual ~CHObjT()
	{
		if(t_bManaged)
			Release();
	}

protected:
	union
	{
		T	_hObj;
		T	_hDC;
		T	_hFile;
		T	_hFind;
	};

public:
	operator T() const		{ return _hObj; }
	operator bool() const	{ return _hObj && true; }
	T operator = (T h)
	{
		Attach(h);
		return _hObj;
	}

public:
	T GetSafeHandle() const	{ return this ? _hObj : NULL; }
	virtual BOOL IsValid() const	{ return TNULL != _hObj; }
	virtual BOOL Close()
	{
		_hObj = TNULL;
		return TRUE;
	}
	virtual void Release()
	{
		if(TNULL != _hObj)
		{
			VERIFY(Close());
			_hObj = TNULL;
		}
	}
	virtual void Attach(T hObj)
	{
		if(hObj != _hObj)
		{
			if(t_bManaged)
				Release();
			_hObj = hObj;
		}
	}
	virtual T Detach()
	{
		T hObj = _hObj;
		_hObj = TNULL;
		return hObj;
	}
protected:
	BOOL VALID_ATTACH(T h)
	{
		if(TNULL == h)
			return FALSE;
		Attach(h);
		return TRUE;
	}
};

template<bool t_bManaged, HANDLE TNULL = NULL>
class CHandleT : public CHObjT<HANDLE, t_bManaged, TNULL>
{
public:
	CHandleT(HANDLE h = TNULL)
		: CHObjT<HANDLE, t_bManaged, TNULL>(h)
	{ }
	virtual ~CHandleT()
	{
		if(t_bManaged)
			Release();
	}
public:
	using CHObjT<HANDLE, t_bManaged, TNULL>::Release;
public:
	virtual BOOL Close()
	{
		ASSERT(__super::IsValid());
		if(!::CloseHandle(__super::_hObj))
			return FALSE;
		__super::_hObj = TNULL;
		return TRUE;
	}
};
typedef CHandleT<true> CHandleNull;
typedef CHandleT<true, INVALID_HANDLE_VALUE> CHandleNegative;

_MZH_END

#endif
