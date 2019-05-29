#pragma once

#include "Math.h"

namespace Forth
{
	// Vector3 struct
	struct Vector3
	{
		float x, y, z;

		Vector3() : x(0), y(0), z(0) { }
		Vector3(float xyz) : x(xyz), y(xyz), z(xyz) { }
		Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
		Vector3(float *arr) : x(arr[0]), y(arr[1]), z(arr[2]) { }

		inline float& operator[](int i)
		{
			return (&x)[i];
		}

		inline float operator[](int i) const
		{
			return (&x)[i];
		}

		void Set(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}

		void SetAll(float a)
		{
			x = y = z = a;
		}

		Vector3& operator+=(const Vector3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		Vector3& operator-=(const Vector3& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		Vector3& operator*=(float f)
		{
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}

		Vector3& operator/=(float f)
		{
			f = 1.F / f;
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}

		const Vector3 operator-(void) const
		{
			return Vector3(-x, -y, -z);
		}

		const Vector3 operator+(const Vector3& rhs) const
		{
			return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
		}

		const Vector3 operator-(const Vector3& rhs) const
		{
			return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
		}

		const Vector3 operator*(float f) const
		{
			return Vector3(x * f, y * f, z * f);
		}

		const Vector3 operator/(float f) const
		{
			f = 1.F / f;
			return Vector3(x * f, y * f, z * f);
		}

		const bool operator<(const Vector3& rhs)
		{
			return x < rhs.x && y < rhs.y && z < rhs.z;
		}

		const bool operator>(const Vector3& rhs)
		{
			return x > rhs.x && y > rhs.y && z > rhs.z;
		}
	};

	// Compute the dot-product of two vectors.
	inline float Dot(const Vector3& a, const Vector3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	// Compute the cross-product of two vectors.
	inline Vector3 Cross(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}

	// Compute the determinant of a matrix whose columns are three given vectors.
	// Useful property: det(a, b, c) = det(c, a, b) = det(b, c, a).
	inline float Det(const Vector3& a, const Vector3& b, const Vector3& c)
	{
		return Dot(a, Cross(b, c));
	}

	// Compute the length of a vector.
	inline float Length(const Vector3& v)
	{
		return Sqrt(Dot(v,v));
	}

	// Compute the squared length of a vector.
	inline float LengthSq(const Vector3& v)
	{
		return Dot(v,v);
	}

	// Compute the normalized vector of a (non-zero!) vector.
	inline Vector3 Normalize(const Vector3& v)
	{
		float length = Length(v);
		if (length > EPSILON)
		{
			float s = 1.0f / length;
			return v * s;
		}
		return v;
	}

	// Compute the euclidean distance between two points.
	inline float Distance(const Vector3& a, const Vector3& b)
	{
		return Length(a - b);
	}

	// Compute the squared distance between two points.
	inline float DistanceSq(const Vector3& a, const Vector3& b)
	{
		Vector3 v = a - b;
		return LengthSq(v);
	}

	// Compute the triangle area.
	inline float Area(const Vector3& a, const Vector3& b, const Vector3& c)
	{
		return Length(Cross(b - a, c - a));
	}

	// Compute the squared triangle area.
	inline float AreaSq(const Vector3& a, const Vector3& b, const Vector3& c)
	{
		return LengthSq(Cross(b - a, c - a));
	}

	// Compute the tetrahedron volume.
	inline float Volume(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d)
	{
		float volume = Det(b - a, c - a, d - a);
		// Force a positive volume.
		float sign = Sign(volume);
		const float inv6 = 1.0f / 6.0f;
		return sign * inv6 * volume;
	}

	// Compute the squared tetrahedron volume.
	inline float VolumeSq(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d)
	{
		float volume = Volume(a, b, c, d);
		return volume * volume;
	}

	// Compute the minimum vector between two vector (per-element).
	inline Vector3 Min(const Vector3& a, const Vector3& b)
	{
		return Vector3(Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z));
	}

	// Compute the maximum vector between two vector (per-element).
	inline Vector3 Max(const Vector3& a, const Vector3& b)
	{
		return Vector3(Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z));
	}

	// Find a perpendicular vector to a vector.
	inline Vector3 Perp(const Vector3& v)
	{
		// Box2D
		// Suppose vector a has all equal components and is a unit vector: a = (s, s, s)
		// Then 3*s*s = 1, s = sqrt(1/3) = 0.57735. This means that at least one component of a
		// unit vector must be greater or equal to 0.57735.
		if (Abs(v.x) >= 0.57735027f)
		{
			return Vector3(v.y, -v.x, 0.f);
		}

		return Vector3(0.0f, v.z, -v.y);
	}


	inline float MaxPerElem(const Vector3& v)
	{
		return Max(v.x, Max(v.y, v.z));
	}

	inline float MinPerElem(const Vector3& v)
	{
		return Min(v.x, Min(v.y, v.z));
	}
}
