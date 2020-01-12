#pragma once

#ifndef _MZH_COMPLEX
#define _MZH_COMPLEX

#include <mzhBase.h>

_MZH_BEGIN

template<typename T>
struct ComplexT {
	union
	{
		T real;
		T x;
	};
	union
	{
		T imag;
		T y;
	};
};

template<typename T>
class CComplexT : public ComplexT < T >
{
public:
	CComplexT()
	{
		real = 0;
		imag = 0;
	}
	CComplexT(T realVal, T imagVal)
	{
		real = realVal;
		imag = imagVal;
	}
	CComplexT(const ComplexT& val)
	{
		real = val.real;
		imag = val.imag;
	}
	~CComplexT()
	{
	}

public:
	T length2() { return real * real + imag * imag; }
	T length() { return sqrt(length2()); }

public:
	CComplexT& operator = (const CComplexT& v)
	{
		real = v.real;
		imag = v.imag;
		return (*this);
	}
	CComplexT& operator += (const CComplexT& v)
	{
		real += v.real;
		imag += v.imag;
		return (*this);
	}
	CComplexT& operator -= (const CComplexT& v)
	{
		real -= v.real;
		imag -= v.imag;
		return (*this);
	}
	CComplexT& operator *= (const CComplexT& v)
	{
		T val = real * v.real - imag * v.imag;
		imag = real * v.imag + imag * v.real;
		real = val;
		return (*this);
	}
	CComplexT& operator /= (const CComplexT& v)
	{
		T val = v.real * v.real - v.imag * v.imag;
		T r = (real * v.real + imag * v.imag) / val;
		imag = (imag * v.real - real * v.imag) / val;
		real = r;
		return (*this);
	}

public:
	friend const CComplexT operator + (const CComplexT& left, const CComplexT& right)
	{ return CComplexT(left.real + right.real, left.imag + right.imag); }
	friend const CComplexT operator - (const CComplexT& left, const CComplexT& right)
	{ return CComplexT(left.real - right.real, left.imag - right.imag); }
	friend const CComplexT operator * (const CComplexT& left, const CComplexT& right)
	{ return CComplexT(left.real * right.real - left.imag * right.imag, left.real * right.imag + left.imag * right.real); }
	friend const CComplexT operator / (const CComplexT& left, const CComplexT& right)
	{
		T val = right.real * left.real - right.imag * right.imag;
		return CComplexT((left.real * right.real + left.imag * right.imag) / val, (left.imag * right.real - left.real * right.imag) / val);
	}
};

typedef ComplexT<float> ComplexF;
typedef CComplexT<float> CComplexF;

typedef ComplexT<double> ComplexD;
typedef CComplexT<double> CComplexD;

namespace FComplex
{
	typedef ComplexF Complex;
	typedef CComplexF CComplex;
}
namespace DComplex
{
	typedef ComplexD Complex;
	typedef CComplexD CComplex;
}

_MZH_END

#endif
