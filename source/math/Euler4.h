#pragma once

#include "Vector3.h"
#include "Vector4.h"

namespace Forth
{
	/// <summary>
	/// Rotation unit on each plane in 4D.
	/// </summary>
	/// <remarks>
	/// Euler4 is basically a vector with 6 axis (plane), and can be manipulated like other vector.
	/// In most cases when dealing with 4D rotations, you'll use Euler4, since it can be
	/// represented as an euler rotation, or angular velocity of an object.
	/// </remarks>
	struct Euler4
	{
		/// <summary>
		/// Axes of each Rotation plane.
		/// </summary>
		float x, y, z, t, u, v;

		/// <summary>
		/// Create empty euler.
		/// </summary>
		Euler4(void) : x(0), y(0), z(0), t(0), u(0), v(0) {}

		/// <summary>
		/// Create new euler with given individual values.
		/// </summary>
		Euler4(float x, float y, float z, float t, float u, float v)
			: x(x), y(y), z(z), t(t), u(u), v(v) {}

		/// <summary>
		/// Create new euler with given individual values from a pair of Vector3.
		/// </summary>
		Euler4(Vector3 xyz, Vector3 tuv)
			: x(xyz.x), y(xyz.y), z(xyz.z), t(tuv.x), u(tuv.y), v(tuv.z) {}

		/// <summary>
		/// Create new euler with a value for given axis index.
		/// </summary>
		Euler4(int axis, float value)
		{
			x = y = z = t = u = v = 0;
			(&x)[axis] = value;
		}

		/// <summary>
		/// Get an axis value of specified index.
		/// </summary>
		float operator[](int i) const
		{
			return (&x)[i];
		}

		/// <summary>
		/// Set an axis value of specified index.
		/// </summary>
		float &operator[](int i)
		{
			return (&x)[i];
		}

		void Set(float _x, float _y, float _z, float _t, float _u, float _v)
		{
			x = _x;
			y = _y;
			z = _z;
			t = _t;
			u = _u;
			v = _v;
		}

		void SetAll(float a)
		{
			x = y = z = t = u = v = a;
		}

		void operator+=(const Euler4 &rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			t += rhs.t;
			u += rhs.u;
			v += rhs.v;
		}

		void operator-=(const Euler4 &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			t -= rhs.t;
			u -= rhs.u;
			v -= rhs.v;
		}

		void operator*=(float f)
		{
			x *= f;
			y *= f;
			z *= f;
			t *= f;
			u *= f;
			v *= f;
		}

		void operator/=(float f)
		{
			f = 1.F / f;
			x *= f;
			y *= f;
			z *= f;
			t *= f;
			u *= f;
			v *= f;
		}

		/// <summary>
		/// Negate each euler axis
		/// </summary>
		const Euler4 operator-(void) const
		{
			return Euler4(-x, -y, -z, -t, -u, -v);
		}

		/// <summary>
		/// Axis-wisely add two euler values
		/// </summary>
		const Euler4 operator+(const Euler4 &rhs) const
		{
			return Euler4(x + rhs.x, y + rhs.y, z + rhs.z, t + rhs.t, u + rhs.u, v + rhs.v);
		}

		/// <summary>
		/// Axis-wisely subtract two euler values
		/// </summary>
		const Euler4 operator-(const Euler4 &rhs) const
		{
			return Euler4(x - rhs.x, y - rhs.y, z - rhs.z, t - rhs.t, u - rhs.u, v - rhs.v);
		}

		/// <summary>
		/// Axis-wisely scale both euler values.
		/// </summary>
		const Euler4 operator*(const Euler4 &rhs) const
		{
			return Euler4(x * rhs.x, y * rhs.y, z * rhs.z, t * rhs.t, u * rhs.u, v * rhs.v);
		}

		/// <summary>
		/// Axis-wisely scale an euler values with a number
		/// </summary>
		const Euler4 operator*(float f) const
		{
			return Euler4(x * f, y * f, z * f, t * f, u * f, v * f);
		}

		/// <summary>
		/// Axis-wisely divide an euler values with a number
		/// </summary>
		const Euler4 operator/(float f) const
		{
			f = 1.F / f;
			return Euler4(x * f, y * f, z * f, t * f, u * f, v * f);
		}

		/// <summary>
		/// Get XYZ part of the euler
		/// </summary>
		const Vector3 xyz(void) const
		{
			return Vector3(x, y, z);
		}

		/// <summary>
		/// Get TUV part if the euler
		/// </summary>
		const Vector3 tuv(void) const
		{
			return Vector3(t, u, v);
		}

		static Euler4 zero()
		{
			return Euler4();
		}
	};

	/// <summary>
	/// Get the dot product of the euler
	/// </summary>
	inline float Dot(const Euler4 &a, const Euler4 &b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.t * b.t + a.u * b.u + a.v * b.v;
	}

	/// <summary>
	/// Get the squared length of the euler
	/// </summary>
	inline float LengthSq(const Euler4 &v)
	{
		return Dot(v, v);
	}

	/// <summary>
	/// Interpolate euler rotation degree from A to B by T in each axes
	/// </summary>
	/// <remarks> This method does loop back from 360 to zero if that's necessary. The interpolation is not clampled. </remarks>
	inline Euler4 LerpAngle(Euler4 a, Euler4 b, float t)
	{
		return Euler4(LerpAngle(a.x, b.x, t), LerpAngle(a.y, b.y, t), LerpAngle(a.z, b.z, t),
					  LerpAngle(a.t, b.t, t), LerpAngle(a.u, b.u, t), LerpAngle(a.v, b.v, t));
	}

	/// <summary>
	/// Creates an euler rotation from two vector
	/// </summary>
	/// <remarks>
	/// This method is equivalent to the wedge operation between vector and vector in Geometry Algebra. It is used by internal physics engine.
	/// </remarks>
	inline Euler4 Cross(Vector4 a, Vector4 b)
	{
		return Euler4(
			(a.y * b.z) - (b.y * a.z),
			(a.z * b.x) - (b.z * a.x),
			(a.x * b.y) - (b.x * a.y),
			(a.x * b.w) - (b.x * a.w),
			(a.y * b.w) - (b.y * a.w),
			(a.z * b.w) - (b.z * a.w));
	}

	/// <summary>
	/// Rotates the vector by euler rotation
	/// </summary>
	/// <remarks>
	/// This method is equivalent to the wedge operation between vector and vector in Geometry Algebra. It is used by internal physics engine.
	/// </remarks>
	inline Vector4 Cross(const Euler4& a, const Vector4& b)
	{
		return Vector4(
			(a.y * b.z) - (a.z * b.y) - (a.t * b.w),
			(a.z * b.x) - (a.x * b.z) - (a.u * b.w),
			(a.x * b.y) - (a.y * b.x) - (a.v * b.w),
			(a.t * b.x) + (a.u * b.y) + (a.v * b.z));
	}
} // namespace Forth
