#pragma once

#include "../extras/Utils.h"
#include "Euler4.h"
#include "Math.h"
#include "Vector4.h"
#include <cmath>

namespace Forth
{

	/// <summary>
	/// A 4x4 matrix to describe 4D rotations.
	/// </summary>
	/// <remarks>
	/// The matrix is represented in row major order.
	/// Most operation that available requires the matrix to be orthogonal.
	/// This should not be confused with Unity's Matrix4x4.
	/// </remarks>
	struct Matrix4
	{

		Vector4 ex, ey, ez, ew;

		Matrix4(void)
		{
			ex = Vector4();
			ey = Vector4();
			ez = Vector4();
			ew = Vector4();
		}

		Matrix4(const Vector4 &_diag)
		{
			ex.x = _diag.x;
			ey.y = _diag.y;
			ez.z = _diag.z;
			ew.w = _diag.w;
		}

		Matrix4(const Vector4 &_ex, const Vector4 &_ey, const Vector4 &_ez, const Vector4 &_ew)
			: ex(_ex), ey(_ey), ez(_ez), ew(_ew) {}

		Matrix4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p)
			: ex(a, b, c, d), ey(e, f, g, h), ez(i, j, k, l), ew(m, n, o, p) {}

		inline Vector4 &operator[](int i)
		{
			return (&ex)[i];
		}

		inline Vector4 operator[](int i) const
		{
			return (&ex)[i];
		}

		const Vector4 Column(int i) const
		{
			return Vector4(ex[i], ey[i], ez[i], ew[i]);
		}

		const Vector4 Column0(void) const
		{
			return Vector4(ex.x, ey.x, ez.x, ew.x);
		}

		const Vector4 Column1(void) const
		{
			return Vector4(ex.y, ey.y, ez.y, ew.y);
		}

		const Vector4 Column2() const
		{
			return Vector4(ex.z, ey.z, ez.z, ew.z);
		}

		const Vector4 Column3() const
		{
			return Vector4(ex.w, ey.w, ez.w, ew.w);
		}

		void Column(int i, const Vector4 &v)
		{
			ex[i] = v.x;
			ey[i] = v.y;
			ez[i] = v.z;
			ew[i] = v.w;
		}

		void Set(const Vector4 &_ex, const Vector4 &_ey, const Vector4 &_ez, const Vector4 &_ew)
		{
			ex = _ex;
			ey = _ey;
			ez = _ez;
			ew = _ew;
		}

		void operator=(const Matrix4 &rhs)
		{
			// guarantee mutability
			ex = rhs.ex;
			ey = rhs.ey;
			ez = rhs.ez;
			ew = rhs.ew;
		}

		void operator+=(const Matrix4 &rhs)
		{
			ex += rhs.ex;
			ey += rhs.ey;
			ez += rhs.ez;
			ew += rhs.ew;
		}

		void operator-=(const Matrix4 &rhs)
		{
			ex -= rhs.ex;
			ey -= rhs.ey;
			ez -= rhs.ez;
			ew -= rhs.ew;
		}

		void operator*=(const Matrix4 &rhs)
		{
			Matrix4 m = Matrix4(
				*this * rhs.Column0(), *this * rhs.Column1(),
				*this * rhs.Column2(), *this * rhs.Column3());
			ex = m.Column0();
			ey = m.Column1();
			ez = m.Column2();
			ew = m.Column3();
		}

		void operator*=(float rhs)
		{
			ex *= rhs;
			ey *= rhs;
			ez *= rhs;
			ew *= rhs;
		}

		bool operator==(const Matrix4 &t) const
		{
			return ex == t.ex && ey == t.ey && ez == t.ez && ew == t.ew;
		}

		Matrix4 operator+(const Matrix4 &rhs) const
		{
			return Matrix4(ex + rhs.ex, ey + rhs.ey, ez + rhs.ez, ew + rhs.ew);
		}

		Matrix4 operator-(const Matrix4 &rhs) const
		{
			return Matrix4(ex - rhs.ex, ey - rhs.ey, ez - rhs.ez, ew - rhs.ew);
		}

		Vector4 operator*(const Vector4 &rhs) const
		{
			return Vector4(Dot(ex, rhs), Dot(ey, rhs), Dot(ez, rhs), Dot(ew, rhs));
		}

		Matrix4 operator*(const Matrix4 &rhs) const
		{
			Matrix4 m = Matrix4(
				*this * rhs.Column0(), *this * rhs.Column1(),
				*this * rhs.Column2(), *this * rhs.Column3());
			m.Transpose();
			return m;
		}

		Matrix4 operator/(const Matrix4 &r) const;

		/// <summary>
		/// Get a 4x4 identity matrix
		/// </summary>
		static const Matrix4 identity;

		static const Matrix4 zero;

	  private:
		void Transpose(void)
		{
			float tmp;
			SWAP(ex.y, ey.x, tmp);
			SWAP(ex.z, ez.x, tmp);
			SWAP(ex.w, ew.x, tmp);
			SWAP(ey.z, ez.y, tmp);
			SWAP(ey.w, ew.y, tmp);
			SWAP(ez.w, ew.z, tmp);
		}
	};

	/// <summary>
	/// Returns the transposed version of the matrix.
	/// </summary>
	/// <remarks> When the matrix is orthogonal, it's equivalent as the inversed version of the matrix </remarks>
	inline Matrix4 Transpose(const Matrix4 &m)
	{
		return Matrix4(m.Column0(), m.Column1(), m.Column2(), m.Column3());
	}

	/// <summary>
	/// Perform a sandwich operation on B by A
	/// </summary>
	/// <remarks> The product is a rotation of B that oriented relative to A coordinate axes </remarks>
	inline Matrix4 Transform(const Matrix4 &a, const Matrix4 &b)
	{
		return Transpose(a) * b * a;
	}

	/// <summary>
	/// Returns the absolute version of the matrix.
	/// </summary>
	inline Matrix4 Abs(const Matrix4 &m)
	{
		return Matrix4(Abs(m.ex), Abs(m.ey), Abs(m.ez), Abs(m.ew));
	}

	/// <summary>
	/// Create rotation matrix that rotates *from* matrix with *to* matrix
	/// </summary>
	inline Matrix4 Delta(Matrix4 from, Matrix4 to)
	{
		return Transpose(from) * to;
	}

	/// <summary>
	/// Get rotation matrix that rotates identity object to given overward axis
	/// </summary>
	inline Matrix4 LookAt(const Vector4 &overward)
	{
		Vector4 a = Normalize(overward);

		// Fastest solution
		return Matrix4(
			a.w, -a.z, a.y, -a.x,
			a.z, a.w, -a.x, -a.y,
			-a.y, a.x, a.w, -a.z,
			a.x, a.y, a.z, a.w);
	}

	/// <summary>
	/// Create rotation matrix that rotates *from* direction with *to* direction
	/// </summary>
	inline Matrix4 Delta(Vector4 from, Vector4 to)
	{
		return Transpose(LookAt(from)) * (LookAt(to));
	}

	/// <summary>
	/// Convert degree euler to orthogonal matrix rotation individually.
	/// </summary>
	/// <remarks>
	/// This creates a rotation matrix that rotates a point by Y, Z, X, T, U, then V. In that order.
	/// </remarks>
	Matrix4 Euler(float x, float y, float z, float t, float u, float v);

	Matrix4 Euler(const Euler4 &e);
	/// <summary>
	/// Convert given degree rotation in given axis to orthogonal matrix rotation.
	/// </summary>
	/// <remarks>This method is much optimized than Euler(new Euler4(axis, degree))</remarks>
	Matrix4 Euler(int axis, float degree);

	/// <summary>
	/// Multiply or rotate a vector by the inversed version of this matrix
	/// </summary>
	static Vector4 operator/(const Vector4 &v, const Matrix4 &r)
	{
		return Transpose(r) * v;
	}

	inline Matrix4 ComputeBasis(const Vector4 &forward)
	{

		// Suppose Vector4 a has all equal components and is a unit Vector4: a = (s, s, s, s)
		// Then 4*s*s = 1, s = sqrt(1/4) = 0.5. This means that at least one component of a
		// unit Vector4 must be greater or equal to 0.5.

		const Vector4 &a = forward;
		Vector4 b, c;

		if (Abs(a.x) >= 0.5)
		{
			b = Vector4(a.y, -a.x, a.z, a.w);
			c = Vector4(a.w, a.z, -a.x, a.y);
		}
		else if (Abs(a.y) > 0.5)
		{
			b = Vector4(-a.y, a.x, a.z, a.w);
			c = Vector4(a.w, a.z, -a.y, a.x);
		}
		else
		{
			// X & Y cone
			b = Vector4(a.x, a.y, a.w, -a.z);
			c = Vector4(a.w, -a.z, a.x, a.y);
		}

		return Matrix4(b, c, forward, Cross(a, b, c));
	}

	inline void ComputeBasis(Vector4 *vector)
	{
		const Matrix4 m = LookAt(vector[0]);
		vector[1] = m.ex;
		vector[2] = m.ey;
		vector[3] = m.ez;
	}
} // namespace Forth
