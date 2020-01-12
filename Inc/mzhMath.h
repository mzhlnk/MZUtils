#pragma once

#ifndef _MZH_MATH
#define _MZH_MATH

#include "mzhBase.h"

_MZH_BEGIN

inline float InvSqrt(float x) restrict(cpu, amp)
{
	float xhalf = .5f * x;
	int i = *(int*)&x;				// get bits for floating value
	i = 0x5f3759df - (i >> 1);		// gives initial guess y0
	x = *(float*)&i;				// convert bits back to float
	x = x * (1.5f - xhalf * x * x);	// Newton step, repeating increasess accuracy
	return x;
}

template<typename T>
inline T Max(T a, T b) restrict(cpu, amp)
{
	return a > b ? a : b;
}
template<typename T>
inline T Min(T a, T b) restrict(cpu, amp)
{
	return a < b ? a : b;
}
template<typename T>
inline T Max(T a, T b, T c) restrict(cpu, amp)
{
	return a > b ? (a > c ? a : c) : (b > c ? b : c);
}
template<typename T>
inline T Min(T a, T b, T c) restrict(cpu, amp)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}
template<typename T>
inline T Abs(T x) restrict(cpu, amp)
{
	return x >= 0 ? x : -x;
}
template<typename T>
T Clamp(T val, T min, T max) restrict(cpu, amp)
{
	if (val <= min) return min;
	if (val >= max) return max;
	return val;
}

// smoothstep(x) = 3x^2 - 2x^3
template<typename T>
inline T SmoothStep(T x) restrict(cpu, amp)
{
	// Scale, bias and saturate x to 0..1 range
	x = Clamp(x, (T)(0), (T)(1));
	// Evaluate polynomial
	return x * x * (3 - 2 * x);
}
template<typename T>
T SmoothStep(T edge0, T edge1, T x) restrict(cpu, amp)
{
	// Scale, bias and saturate x to 0..1 range
	x = Clamp((x - edge0) / (edge1 - edge0), (T)(0), (T)(1));
	// Evaluate polynomial
	return x * x * (3 - 2 * x);
}

// smootherstep(x) = 6x^5 - 15x^4 + 10x^3
template<typename T>
inline T SmootherStep(T x) restrict(cpu, amp)
{
	// Scale, and clamp x to 0..1 range
	x = Clamp(x, (T)(0), (T)(1));
	// Evaluate polynomial
	return x * x * x * (x * (x * 6 - 15) + 10);
}
template<typename T>
T SmootherStep(T edge0, T edge1, T x) restrict(cpu, amp)
{
	// Scale, and clamp x to 0..1 range
	x = Clamp((x - edge0) / (edge1 - edge0), (T)(0), (T)(1));
	// Evaluate polynomial
	return x * x * x * (x * (x * 6 - 15) + 10);
}

template<typename T>
T Lerp(T min, T max, T s) restrict(cpu, amp)
{
	return min + (max - min) * s;
}


_MZH_END
#endif
