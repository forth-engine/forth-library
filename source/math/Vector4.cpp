#include "Vector4.h"

namespace Forth
{
	/// <summary>
	/// Create an euler data with a value for given axis index.
	/// </summary>

	/// <summary>
	/// Create uniform vector.
	/// </summary>

	Vector4::Vector4(float scale) : x(scale), y(scale), z(scale), w(scale) {}

	Vector4::Vector4(int axis, float value)
	{
		x = y = z = w = 0;
		(&x)[axis] = value;
	}
	Vector4::Vector4(float *arr) : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3]) {}

	void Vector4::Set(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	void Vector4::SetAll(float a)
	{
		x = y = z = w = a;
	}

	const Vector3 Vector4::ToVec3(void) const
	{
		return *(Vector3 *)&x;
	}

	bool Vector4::operator==(const Vector4 &t) const
	{
		return DistanceSq(*this, t) <= EPSILON;
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
	Vector4 Cross(const Vector4 &a, const Vector4 &b, const Vector4 &c)
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
	Vector4 Lerp(const Vector4 &a, const Vector4 &b, const float t)
	{
		return a + (b - a) * Clamp01(t);
	}

	/// <summary>
	/// Interpolate two vector by T.
	/// </summary>
	/// <remarks> The interpolation is **not** clamped. </remarks>
	Vector4 LerpUnclamped(const Vector4 &a, const Vector4 &b, const float t)
	{
		return a + (b - a) * t;
	}

	/// <summary>
	/// Get the length (magnitude) of the vector.
	/// </summary>
	float Length(const Vector4 &v)
	{
		return Sqrt(Dot(v, v));
	}

	/// <summary>
	/// Get the squared length of the vector.
	/// </summary>
	float LengthSq(const Vector4 &v)
	{
		return Dot(v, v);
	}

	/// <summary>
	/// Get the distance between two vector.
	/// </summary>
	float Distance(const Vector4 &a, const Vector4 &b)
	{
		return Length(a - b);
	}

	/// <summary>
	/// Get the squared distance between two vector.
	/// </summary>
	float DistanceSq(const Vector4 &a, const Vector4 &b)
	{
		return LengthSq(a - b);
	}

	/// <summary>
	/// Normalize the vector.
	/// </summary>
	Vector4 Normalize(const Vector4 &v)
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
	Vector4 Abs(const Vector4 &v)
	{
		return Vector4(Forth::Abs(v.x), Abs(v.y), Abs(v.z), Abs(v.w));
	}

	/// <summary>
	/// Get the sign version of the vector.
	/// </summary>
	Vector4 Sign(const Vector4 &n)
	{
		return Vector4(Sign(n.x), Sign(n.y), Sign(n.z), Sign(n.w));
	}

	/// <summary>
	/// Clamp vector T between A and B.
	/// </summary>
	Vector4 Clamp(const Vector4 &a, const Vector4 &b, const Vector4 &t)
	{
		return Vector4(
			Clamp(a.x, b.x, t.x),
			Clamp(a.y, b.y, t.y),
			Clamp(a.z, b.z, t.z),
			Clamp(a.w, b.w, t.w));
	}

	/// <summary>
	/// Axis-wisely inverse the vector.
	/// </summary>
	Vector4 Invert(const Vector4 &v)
	{
		return Vector4(1.0f / v.x, 1.0f / v.y, 1.0f / v.z, 1.0f / v.w);
	}

	/// <summary>
	/// Axis-wisely choose the smallest range of two vector.
	/// </summary>
	Vector4 Min(const Vector4 &a, const Vector4 &b)
	{
		return Vector4(Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z), Min(a.w, b.w));
	}

	/// <summary>
	/// Axis-wisely choose the largest range of two vector.
	/// </summary>
	Vector4 Max(const Vector4 &a, const Vector4 &b)
	{
		return Vector4(Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z), Max(a.w, b.w));
	}

	/// <summary>
	/// Return the smallest axis value in the vector.
	/// </summary>
	float MinPerElem(const Vector4 &v)
	{
		return Min(Min(v.x, v.w), Min(v.y, v.z));
	}

	/// <summary>
	/// Return the largest axis value in the vector.
	/// </summary>
	float MaxPerElem(const Vector4 &v)
	{
		return Max(Max(v.x, v.w), Max(v.y, v.z));
	}

	/// <summary>
	/// Return the axis index that has smallest value in the vector.
	/// </summary>
	int MinPerElemIdx(const Vector4 &a)
	{
		return a.x < a.y && a.x < a.z && a.x < a.w ? 0 : (a.y < a.z && a.y < a.w ? 1 : (a.z < a.w ? 2 : 3));
	}

	/// <summary>
	/// Return the axis index that has largest value in the vector.
	/// </summary>
	int MaxPerElemIdx(const Vector4 &a)
	{
		return a.x > a.y && a.x > a.z && a.x > a.w ? 0 : (a.y > a.z && a.y > a.w ? 1 : (a.z > a.w ? 2 : 3));
	}

	const Vector4 Vector4::one = Vector4(1);
	const Vector4 Vector4::zero = Vector4();

} // namespace Forth