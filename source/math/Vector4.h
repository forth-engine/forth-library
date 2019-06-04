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
		Vector4(void) : x(0), y(0), z(0), w(0) {}

		/// <summary>
		/// Create uniform vector.
		/// </summary>
		Vector4(float scale);

		/// <summary>
		/// Create an euler data with a value for given axis index.
		/// </summary>
		Vector4(int axis, float value);

		/// <summary>
		/// Create new vector with given individual values.
		/// </summary>
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

		Vector4(float *arr);

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
		inline float &operator[](int i)
		{
			return (&x)[i];
		}

		void Set(float _x, float _y, float _z, float _w);

		void SetAll(float a);

		Vector4 &operator+=(const Vector4 &rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}

		Vector4 &operator-=(const Vector4 &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}

		Vector4 &operator*=(float f)
		{
			x *= f;
			y *= f;
			z *= f;
			w *= f;
			return *this;
		}

		Vector4 &operator/=(float f)
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
		const Vector4 operator+(const Vector4 &rhs) const
		{
			return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
		}

		/// <summary>
		/// Axis-wisely subtract two vector values.
		/// </summary>
		const Vector4 operator-(const Vector4 &rhs) const
		{
			return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
		}

		/// <summary>
		/// Axis-wisely multiply two vector values.
		/// </summary>
		const Vector4 operator*(const Vector4 &rhs) const
		{
			return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
		}

		/// <summary>
		/// Axis-wisely scale vector values with a number.
		/// </summary>
		const Vector4 operator*(const float f) const
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
		bool operator<(const Vector4 &rhs) const
		{
			return x < rhs.x && y < rhs.y && z < rhs.z && w < rhs.w;
		}

		/// <summary>
		/// Compare if A is smaller than B in all axes.
		/// </summary>
		bool operator>(const Vector4 &rhs) const
		{
			return x > rhs.x && y > rhs.y && z > rhs.z && w > rhs.w;
		}

		bool operator==(const Vector4 &t) const;

		const Vector3 ToVec3(void) const;

		/// <summary>
		/// Vector with values of one.
		/// </summary>
		static const Vector4 one;

		/// <summary>
		/// Vector with values of one.
		/// </summary>
		static const Vector4 zero;
	};

	/// <summary>
	/// Get the dot operation between two vector.
	/// </summary>
	inline float Dot(const Vector4 &a, const Vector4 &b)
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
	Vector4 Cross(const Vector4 &a, const Vector4 &b, const Vector4 &c);

	/// <summary>
	/// Interpolate two vector by T and clamp.
	/// </summary>
	/// <remarks> The interpolation is clamped between zero to one </remarks>
	Vector4 Lerp(const Vector4 &a, const Vector4 &b, const float t);

	/// <summary>
	/// Interpolate two vector by T.
	/// </summary>
	/// <remarks> The interpolation is **not** clamped. </remarks>
	Vector4 LerpUnclamped(const Vector4 &a, const Vector4 &b, const float t);

	/// <summary>
	/// Get the length (magnitude) of the vector.
	/// </summary>
	float Length(const Vector4 &v);

	/// <summary>
	/// Get the squared length of the vector.
	/// </summary>
	float LengthSq(const Vector4 &v);

	/// <summary>
	/// Get the distance between two vector.
	/// </summary>
	float Distance(const Vector4 &a, const Vector4 &b);

	/// <summary>
	/// Get the squared distance between two vector.
	/// </summary>
	float DistanceSq(const Vector4 &a, const Vector4 &b);

	/// <summary>
	/// Normalize the vector.
	/// </summary>
	Vector4 Normalize(const Vector4 &v);

	/// <summary>
	/// Get the absolute version of the vector.
	/// </summary>
	Vector4 Abs(const Vector4 &v);

	/// <summary>
	/// Get the sign version of the vector.
	/// </summary>
	Vector4 Sign(const Vector4 &n);

	/// <summary>
	/// Clamp vector T between A and B.
	/// </summary>
	Vector4 Clamp(const Vector4 &a, const Vector4 &b, const Vector4 &t);

	/// <summary>
	/// Axis-wisely inverse the vector.
	/// </summary>
	Vector4 Invert(const Vector4 &v);

	/// <summary>
	/// Axis-wisely choose the smallest range of two vector.
	/// </summary>
	Vector4 Min(const Vector4 &a, const Vector4 &b);

	/// <summary>
	/// Axis-wisely choose the largest range of two vector.
	/// </summary>
	Vector4 Max(const Vector4 &a, const Vector4 &b);

	/// <summary>
	/// Return the smallest axis value in the vector.
	/// </summary>
	float MinPerElem(const Vector4 &v);

	/// <summary>
	/// Return the largest axis value in the vector.
	/// </summary>
	float MaxPerElem(const Vector4 &v);

	/// <summary>
	/// Return the axis index that has smallest value in the vector.
	/// </summary>
	int MinPerElemIdx(const Vector4 &a);

	/// <summary>
	/// Return the axis index that has largest value in the vector.
	/// </summary>
	int MaxPerElemIdx(const Vector4 &a);

} // namespace Forth
