#pragma once
#include <algorithm>
#include <cmath>
#include <float.h>

#define PI (3.14159265359f)
#define MAX_FLOAT (FLT_MAX)
#define EPSILON (FLT_EPSILON)
#define DEG2RAD PI / 180.f
#define RAD2DEG 180.f / PI

namespace Forth
{

	inline float Sqrt(float x)
	{
		return std::sqrt(x);
	}

	// https://stackoverflow.com/a/1349572
	inline float InvSqrt(float x)
	{
		float xhalf = 0.5f * x;
		int i = *(int *)&x;				// get bits for floating value
		i = 0x5f375a86 - (i >> 1);		// gives initial guess y0
		x = *(float *)&i;				// convert bits back to float
		x = x * (1.5f - xhalf * x * x); // Newton step, increases accuracy
		return x;
	}

	/// <summary> Return absolute value </summary>
	template <class T>
	inline T Abs(T x)
	{
		return std::abs(x);
	}

	template <class T>
	inline T Min(T a, T b)
	{
		return a < b ? a : b;
	}

	template <class T>
	inline T Max(T a, T b)
	{
		return a > b ? a : b;
	}

	template <class T>
	inline T Floor(T v)
	{
		return std::floor(v);
	}

	template <class T>
	inline T Ceil(T v)
	{
		return std::ceil(v);
	}

	template <class T>
	inline T Clamp(T low, T high, T t)
	{
		return Max(low, Min(t, high));
	}

	template <class T>
	inline T Clamp01(T a)
	{
		return Max(T(0), Min(a, T(1)));
	}

	inline float Repeat(float t, float length)
	{
		return Clamp(t - Floor(t / length) * length, 0.f, length);
	}

	inline float DeltaAngle(float current, float target)
	{
		float num = Repeat(target - current, 360.f);
		if (num > 180.f)
		{
			num -= 360.f;
		}
		return num;
	}

	inline float LerpAngle(float a, float b, float t)
	{
		return a == b ? a : a + DeltaAngle(a, b) * t;
	}

	template <class T>
	inline T Sign(const T x)
	{
		return T(T(0) < x) - T(T(x) < T(0));
	}

	template <class T>
	inline T Lerp(const T a, const T b, const T t)
	{
		return a + (b - a) * t;
	}

	inline float Invert(float f)
	{
		return 1.f / f;
	}

} // namespace Forth
