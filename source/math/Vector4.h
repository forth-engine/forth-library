#pragma once

#include "Math.h"
#include "Vector3.h"

namespace Forth
{
	/// <summary>
	/// Vector in 4D.
	/// </summary>
	/// <remarks>
	/// Not to be confused with UnityEngine's Vector4.
	/// For all operation in Forth, Vector4 refers to this struct.
	/// We create the duplicate because UnityEngine's variant is lack of static utilities.
	/// </remarks>
	struct Vector4
	{
		/// <summary>
		/// axes of the vector.
		/// </summary>
		float x, y, z, w;

		/// <summary>
		/// create empty vector.
		/// </summary>
		Vector4(void) : x(0), y(0), z(0), w(0) { }

		/// <summary>
		/// Create uniform vector.
		/// </summary>
		Vector4(float scale) : x(scale), y(scale), z(scale), w(scale) { }

		/// <summary>
		/// Create an euler data with a value for given axis index.
		/// </summary>
		Vector4(int axis, float value)
		{
			x = y = z = w = 0;
			(&x)[axis] = value;
		}

		/// <summary>
		/// Create new vector with given individual values.
		/// </summary>
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

		Vector4(float *arr) : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3]) { }


		/// <summary>
		/// Get an axis value of specified index.
		/// </summary>
		inline float operator[](int i) const
		{
			return (&x)[i];
		}

		/// <summary>
		/// Set an axis value of specified index.
		/// </summary>
		inline float& operator[](int i)
		{
			return (&x)[i];
		}

		void Set(float _x, float _y, float _z, float _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}

		void SetAll(float a)
		{
			x = y = z = w = a;
		}

		Vector4& operator+=(const Vector4& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}

		Vector4& operator-=(const Vector4& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}

		Vector4& operator*=(float f)
		{
			x *= f;
			y *= f;
			z *= f;
			w *= f;
			return *this;
		}

		Vector4& operator/=(float f)
		{
			f = 1.F / f;
			x *= f;
			y *= f;
			z *= f;
			w *= f;
			return *this;
		}

		/// <summary>
		/// Negate each vector axis.
		/// </summary>
		const Vector4 operator-(void) const
		{
			return Vector4(-x, -y, -z, -w);
		}

		/// <summary>
		/// Axis-wisely add two vector values.
		/// </summary>
		const Vector4 operator+(const Vector4& rhs) const
		{
			return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
		}

		/// <summary>
		/// Axis-wisely subtract two vector values.
		/// </summary>
		const Vector4 operator-(const Vector4& rhs) const
		{
			return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
		}

		/// <summary>
		/// Axis-wisely multiply two vector values.
		/// </summary>
		const Vector4 operator*(const Vector4& rhs) const
		{
			return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
		}

		/// <summary>
		/// Axis-wisely scale vector values with a number.
		/// </summary>
		const Vector4 operator*(float f) const
		{
			return Vector4(x * f, y * f, z * f, w * f);
		}

		/// <summary>
		/// Axis-wisely divide vector values with a number.
		/// </summary>
		const Vector4 operator/(float f) const
		{
			f = 1.F / f;
			return Vector4(x * f, y * f, z * f, w * f);
		}

		/// <summary>
		/// Compare if A is greater than B in all axes.
		/// </summary>
		const bool operator<(const Vector4& rhs) const
		{
			return x < rhs.x && y < rhs.y && z < rhs.z && w < rhs.w;
		}

		/// <summary>
		/// Compare if A is smaller than B in all axes.
		/// </summary>
		const bool operator>(const Vector4& rhs) const
		{
			return x > rhs.x && y > rhs.y && z > rhs.z && w > rhs.w;
		}

		const Vector3 ToVec3(void) const
		{
			return *(Vector3*)&x;
		}

		/// <summary>
		/// Vector with values of one.
		/// </summary>
		inline static Vector4 one(void)
		{
			return Vector4(1);
		}
	};

	/// <summary>
	/// Get the dot operation between two vector.
	/// </summary>
	inline float Dot(const Vector4& a, const Vector4& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	/// <summary>
	/// Get the cross operation between three vectors.
	/// </summary>
	/// <remarks>
	/// <para>
	/// This cross operation finds another vector which perpendicular to those three vectors.
	/// The convention used such that A and B is equivalent to the left hand rule if C is positive overward.
	/// </para>
	/// </remarks>
	inline Vector4 Cross(const Vector4& a, const Vector4& b, const Vector4& c)
	{
		float A, B, C, D, E, F; // Intermediate Values

		Vector4 r;
		A = (b.x * c.y) - (b.y * c.x);
		B = (b.x * c.z) - (b.z * c.x);
		C = (b.x * c.w) - (b.w * c.x);
		D = (b.y * c.z) - (b.z * c.y);
		E = (b.y * c.w) - (b.w * c.y);
		F = (b.z * c.w) - (b.w * c.z);

		r.x = (a.y * F) - (a.z * E) + (a.w * D);
		r.y = -(a.x * F) + (a.z * C) - (a.w * B);
		r.z = (a.x * E) - (a.y * C) + (a.w * A);
		r.w = -(a.x * D) + (a.y * B) - (a.z * A);

		return r;
	}

	/// <summary>
	/// Interpolate two vector by T and clamp.
	/// </summary>
	/// <remarks> The interpolation is clamped between zero to one </remarks>
	inline Vector4 Lerp(const Vector4& a, const Vector4& b, float t)
	{
		return a + (b - a) * Clamp01(t);
	}

	/// <summary>
	/// Interpolate two vector by T.
	/// </summary>
	/// <remarks> The interpolation is **not** clamped. </remarks>
	inline Vector4 LerpUnclamped(const Vector4& a, const Vector4& b, float t)
	{
		return a + (b - a) * t;
	}

	/// <summary>
	/// Get the length (magnitude) of the vector.
	/// </summary>
	inline float Length(const Vector4& v)
	{
		return Sqrt(Dot(v, v));
	}

	/// <summary>
	/// Get the squared length of the vector.
	/// </summary>
	inline float LengthSq(const Vector4& v)
	{
		return Dot(v, v);
	}

	/// <summary>
	/// Get the distance between two vector.
	/// </summary>
	inline float Distance(const Vector4& a, const Vector4& b)
	{
		return Length(a - b);
	}

	/// <summary>
	/// Get the squared distance between two vector.
	/// </summary>
	inline float DistanceSq(const Vector4& a, const Vector4& b)
	{
		Vector4 v = a - b;
		return LengthSq(v);
	}

	/// <summary>
	/// Normalize the vector.
	/// </summary>
	inline Vector4 Normalize(const Vector4& v)
	{
		float length = LengthSq(v);
		if (length > EPSILON)
		{
			return v * InvSqrt(length);
		}
		return v;
	}

	/// <summary>
	/// Get the absolute version of the vector.
	/// </summary>
	inline Vector4 Abs(const Vector4& v)
	{
		return Vector4(Forth::Abs(v.x), Abs(v.y), Abs(v.z), Abs(v.w));
	}

	/// <summary>
	/// Get the sign version of the vector.
	/// </summary>
	inline Vector4 Sign(const Vector4& n)
	{
		return Vector4(Sign(n.x), Sign(n.y), Sign(n.z), Sign(n.w));
	}

	/// <summary>
	/// Clamp vector T between A and B.
	/// </summary>
	inline Vector4 Clamp(const Vector4& a, const Vector4& b, const Vector4& t)
	{
		return Vector4(
			Clamp(a.x, b.x, t.x),
			Clamp(a.y, b.y, t.y),
			Clamp(a.z, b.z, t.z),
			Clamp(a.w, b.w, t.w)
		);
	}

	/// <summary>
	/// Axis-wisely inverse the vector.
	/// </summary>
	inline Vector4 Invert(const Vector4& v)
	{
		return Vector4(1.0f / v.x, 1.0f / v.y, 1.0f / v.z, 1.0f / v.w);
	}

	/// <summary>
	/// Axis-wisely choose the smallest range of two vector.
	/// </summary>
	inline Vector4 Min(const Vector4& a, const Vector4& b)
	{
		return Vector4(Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z), Min(a.w, b.w));
	}

	/// <summary>
	/// Axis-wisely choose the largest range of two vector.
	/// </summary>
	inline Vector4 Max(const Vector4& a, const Vector4& b)
	{
		return Vector4(Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z), Max(a.w, b.w));
	}

	/// <summary>
	/// Return the smallest axis value in the vector.
	/// </summary>
	inline float MinPerElem(const Vector4& v)
	{
		return Min(Min(v.x, v.w), Min(v.y, v.z));
	}

	/// <summary>
	/// Return the largest axis value in the vector.
	/// </summary>
	inline float MaxPerElem(const Vector4& v)
	{
		return Max(Max(v.x, v.w), Max(v.y, v.z));
	}

	/// <summary>
	/// Return the axis index that has smallest value in the vector.
	/// </summary>
	inline int MinPerElemIdx(const Vector4& a)
	{
		return a.x < a.y && a.x < a.z && a.x < a.w ? 0 : (a.y < a.z && a.y < a.w ? 1 : (a.z < a.w ? 2 : 3));
	}

	/// <summary>
	/// Return the axis index that has largest value in the vector.
	/// </summary>
	inline int MaxPerElemIdx(const Vector4& a)
	{
		return a.x > a.y && a.x > a.z && a.x > a.w ? 0 : (a.y > a.z && a.y > a.w ? 1 : (a.z > a.w ? 2 : 3));
	}

}
