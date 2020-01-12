#pragma once

#ifndef _MZH_TYPE
#define _MZH_TYPE

#include <tchar.h>
#include <Windows.h>
#include <mzhDef.h>

_MZH_BEGIN

typedef char int8_t;
typedef unsigned char uint8_t, byte, Byte;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

template<typename T>
class CTypeIT
{
public:
	virtual ~CTypeIT() {}

public:
	virtual BOOL IsValid() = 0;
	virtual BOOL IsReadOnly() = 0;
	virtual BOOL SetValue(const T) = 0;
	virtual T GetValue() = 0;
};

template<typename T>
class CTypeT : public CTypeIT<T>
{
public:
	CTypeT::CTypeT(T val = 0)
		: _Val(val)
	{ }

protected:
	T	_Val;
public:
	operator const T() const	{ return _Val; }
	operator T&()				{ return _Val; }

public:
	virtual BOOL IsValid() { return TRUE; }
	virtual BOOL IsReadOnly() { return FALSE; }
	virtual BOOL SetValue(const T val) { _Val = val; return TRUE; }
	virtual T GetValue() { return _Val; }
};

typedef CTypeT<bool> CBool;
typedef CTypeT<char> CChar;
typedef CTypeT<unsigned char> CUChar;
typedef CTypeT<short> CShort;
typedef CTypeT<unsigned short> CUShort;
typedef CTypeT<int> CInt;
typedef CTypeT<unsigned int> CUInt;
typedef CTypeT<long> CLong;
typedef CTypeT<unsigned long> CULong;
typedef CTypeT<long long> CLongLong;
typedef CTypeT<unsigned long long> CULongLong;
typedef CTypeT<float> CFloat;
typedef CTypeT<double> CDouble;

typedef CTypeT<BYTE> CByte;
typedef CTypeT<WORD> CWord;
typedef CTypeT<DWORD> CDWord;
typedef CTypeT<BOOL> CBOOL;


template<typename T, bool t_bManaged = false>
class CMethodIT1 : protected CTypeIT<T>
{
public:
	CMethodIT1(CTypeIT<T>* ptr = NULL)
		: _Ptr(ptr)
	{}
	virtual ~CMethodIT1()
	{
		if (t_bManaged)
		{
			delete _Ptr;
			_Ptr = NULL;
		}
	}

protected:
	CTypeIT<T>* _Ptr;

public:
	void Attach(CTypeIT<T>* ptr)
	{
		if (ptr != _Ptr)
		{
			if (t_bManaged)
				delete _Ptr;
			_Ptr = ptr;
		}
	}
	CTypeIT<T>* Detach()
	{
		CTypeIT<T>* ptr = _Ptr;
		_Ptr = NULL;
		return ptr;
	}

public:
	virtual BOOL IsValid() { return _Ptr && _Ptr->IsValid(); }
	virtual BOOL IsReadOnly() { return TRUE; }

protected:
	virtual BOOL SetValue(const T) { return FALSE; }
};

template<typename T, bool t_bManaged = false>
class CMethodIT2 : public CTypeIT<T>
{
public:
	CMethodIT2(CTypeIT<T>* ptr1 = NULL, CTypeIT<T>* ptr2 = NULL)
		: _Ptr1(ptr1)
		, _Ptr2(ptr2)
	{}
	virtual ~CMethodIT2()
	{
		if (t_bManaged)
		{
			delete _Ptr1;
			_Ptr1 = NULL;
			delete _Ptr2;
			_Ptr2 = NULL;
		}
	}

protected:
	CTypeIT<T>* _Ptr1;
	CTypeIT<T>* _Ptr2;

public:
	void Attach1(CTypeIT<T>* ptr)
	{
		if (ptr != _Ptr1)
		{
			if (t_bManaged)
				delete _Ptr1;
			_Ptr1 = ptr;
		}
	}
	CTypeIT<T>* Detach1()
	{
		CTypeIT<T>* ptr = _Ptr1;
		_Ptr1 = NULL;
		return ptr;
	}
	void Attach2(CTypeIT<T>* ptr)
	{
		if (ptr != _Ptr2)
		{
			if (t_bManaged)
				delete _Ptr2;
			_Ptr2 = ptr;
		}
	}
	CTypeIT<T>* Detach2()
	{
		CTypeIT<T>* ptr = _Ptr2;
		_Ptr2 = NULL;
		return ptr;
	}

public:
	virtual BOOL IsValid() { return _Ptr1 && _Ptr1->IsValid() && _Ptr2 && _Ptr2->IsValid(); }
	virtual BOOL IsReadOnly() { return TRUE; }

protected:
	virtual BOOL SetValue(const T) { return FALSE; }
};

#define OP_METHOD_1(name, op)	\
template<typename T, bool t_bManaged = false>		\
class C ##name ##T : public CMethodIT1<T, t_bManaged> {	\
	public:	C ##name ##T(CTypeIT<T>* ptr = NULL) : CMethodIT1<T, t_bManaged>(ptr) {}	\
	public: T GetValue() { if (__super::_Ptr) return op __super::_Ptr->GetValue(); throw ERROR_INVALID_ACCESS; } \
};

OP_METHOD_1(Negative, -)


#define OP_METHOD_2(name, op)	\
template<typename T, bool t_bManaged = false>		\
class C ##name ##T : public CMethodIT2<T, t_bManaged> {	\
	public: C ##name ##T(CTypeIT<T>* ptr1 = NULL, CTypeIT<T>* ptr2 = NULL) : CMethodIT2<T, t_bManaged>(ptr1, ptr2) {}	\
	public: T GetValue() { if (__super::_Ptr1 && __super::_Ptr2) return __super::_Ptr1->GetValue() op __super::_Ptr2->GetValue(); throw ERROR_INVALID_ACCESS; }	\
};

OP_METHOD_2(Plus, +)
OP_METHOD_2(Minus, -)
OP_METHOD_2(Mul, *)
OP_METHOD_2(Div, / )


template<typename T>
struct tagBufferT
{
	T*		_Ptr;
	ULONG	_Size;
};
template<typename T>
struct tagBufferExT
{
	T*		_Ptr;
	ULONG	_Size;
	LONG	_Offset;
};
typedef tagBufferT<VOID> VBuffer, *PVBuffer;
typedef tagBufferExT<VOID> VBufferEx, *PVBufferEx;
typedef tagBufferT<BYTE> BBuffer, *PBBuffer;
typedef tagBufferExT<BYTE> BBufferEx, *PBBufferEx;

_MZH_END

#endif
