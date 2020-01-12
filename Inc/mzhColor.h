#pragma once

#ifndef _MZH_COLOR
#define _MZH_COLOR

#include <mzhBase.h>
#include <mzhMath.h>

_MZH_BEGIN

struct sRGB {
	union
	{
		COLORREF	clr;
		RGBQUAD		rgb;
		struct
		{
			BYTE b;
			BYTE g;
			BYTE r;
			BYTE a;
		};
	};
};

template<typename T>
struct HSBT {
	T h;	// hue (0,360)
	T s;	// saturation (0,1)
	T b;	// brightness (0,1)
};


template<typename T>
inline COLORREF GetColorValue(T rv, T gv, T bv) restrict(cpu, amp)
{
	COLORREF r = (COLORREF)(rv * 255 + .5f) & 0xFF;
	COLORREF g = (COLORREF)(gv * 255 + .5f) & 0xFF;
	COLORREF b = (COLORREF)(bv * 255 + .5f) & 0xFF;
	return r | (g << 8) | (b << 16);
}

template<typename T>
COLORREF HSB2RGB(const HSBT<T>& hsb) restrict(cpu, amp)
{
	T h = ((int)hsb.h % 360 + (hsb.h - (int)hsb.h));
	if (h < 0) h += 360;
	int i = (int)h / 60;
	T f = (h / 60) - i;
	T p = hsb.b * (1 - hsb.s);
	T q = hsb.b * (1 - f * hsb.s);
	T t = hsb.b * (1 - (1 - f) * hsb.s);

	switch (i)
	{
		case 0:
			return GetColorValue(hsb.b, t, p);
		case 1:
			return GetColorValue(q, hsb.b, p);
		case 2:
			return GetColorValue(p, hsb.b, t);
		case 3:
			return GetColorValue(p, q, hsb.b);
		case 4:
			return GetColorValue(t, p, hsb.b);
		case 5:
			return GetColorValue(hsb.b, p, q);
		default:
			break;
	}

	return COLORREF(0);
}
template<typename T>
void RGB2HSB(COLORREF clr, HSBT<T>& hsb) restrict(cpu, amp)
{
	int r = clr & 0xFF;
	int g = (clr >> 8) & 0xFF;
	int b = (clr >> 16) & 0xFF;
	int max = Max(r, g, b);
	int min = Min(r, g, b);
	int dv = max - min;
	hsb.b = (T)(max) / 255;
	hsb.s = max ? (T)(max - min) / max : 0;
	if (max == min)
	{
		hsb.h = 0;
	}
	else if (max == r)
	{
		if (g >= b)
		{
			hsb.h = (T)((g - b) * 60) / dv;
		}
		else
		{
			hsb.h = (T)((g - b) * 60) / dv + 360;
		}
	}
	else if (max == g)
	{
		hsb.h = (T)((b - r) * 60) / dv + 120;
	}
	else if (max == b)
	{
		hsb.h = (T)(((int)r - g) * 60) / dv + 240;
	}
}

template<typename T>
class CHSBT : public HSBT < T >
{
public:
	CHSBT()
	{
		h = s = b = 0;
	}
	CHSBT(T H, T S, T B)
	{
		h = H, s = S, b = B;
	}
	CHSBT(COLORREF clr)
	{
		RGB2HSB(clr, *this);
	}
	operator COLORREF() const
	{
		return HSB2RGB(*this);
	}
};
namespace FColor
{
	typedef HSBT<float> sHSB;
	typedef CHSBT<float> CHSB;
}
namespace DColor
{
	typedef HSBT<double> sHSB;
	typedef CHSBT<double> CHSB;
}

_MZH_END

#endif
