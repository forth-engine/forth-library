#pragma once


#include "Vector4.h"

#define SWAP(a, b, c) c=a;a=b;b=c;

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

		Matrix4() { }

		Matrix4(const Vector4& _diag)
		{
			ex.x = _diag.x;
			ey.y = _diag.y;
			ez.z = _diag.z;
			ew.w = _diag.w;
		}

		Matrix4(const Vector4& _ex, const Vector4& _ey, const Vector4& _ez, const Vector4& _ew)
			: ex(_ex), ey(_ey), ez(_ez), ew(_ew) { }

		Matrix4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p)
			: ex(a, b, c, d), ey(e,f,g,h), ez(i,j,k,l), ew(m,n,o,p) { }

		Vector4& operator[](int i)
		{
			return (&ex)[i];
		}

		Vector4 operator[](int i) const
		{
			return (&ex)[i];
		}

		const Vector4 Column(int i) const
		{
			return Vector4(ex[i], ey[i], ez[i], ew[i]);
		}

		const Vector4 Column0() const
		{
			return Vector4(ex.x, ey.x, ez.x, ew.x);
		}

		const Vector4 Column1() const
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

		void Column(int i, const Vector4& v)
		{
			ex[i] = v.x;
			ey[i] = v.y;
			ez[i] = v.z;
			ew[i] = v.w;
		}

		void Set(const Vector4& _ex, const Vector4& _ey, const Vector4& _ez, const Vector4& _ew)
		{
			ex = _ex;
			ey = _ey;
			ez = _ez;
			ew = _ew;
		}


		void operator=(const Matrix4& rhs)
		{
			// guarantee mutability
			ex = rhs.ex;
			ey = rhs.ey;
			ez = rhs.ez;
			ew = rhs.ew;
		}

		void operator+=(const Matrix4& rhs)
		{
			ex += rhs.ex;
			ey += rhs.ey;
			ez += rhs.ez;
			ew += rhs.ew;
		}

		void operator-=(const Matrix4& rhs)
		{
			ex -= rhs.ex;
			ey -= rhs.ey;
			ez -= rhs.ez;
			ew -= rhs.ew;
		}

		void operator*=(const Matrix4& rhs)
		{
			Matrix4 m = Matrix4(
				*this * rhs.Column0(), *this * rhs.Column1(),
				*this * rhs.Column2(), *this * rhs.Column3());
			ex = m.Column0();
			ey = m.Column1();
			ez = m.Column2();
			ew = m.Column3();
		}

		const Matrix4 operator+(const Matrix4& rhs) const
		{
			return Matrix4(ex + rhs.ex, ey + rhs.ey, ez + rhs.ez, ew + rhs.ew);
		}

		const Matrix4 operator-(const Matrix4& rhs) const
		{
			return Matrix4(ex - rhs.ex, ey - rhs.ey, ez - rhs.ez, ew - rhs.ew);
		}

		const Vector4 operator*(const Vector4& rhs) const
		{
			return Vector4(Dot(ex, rhs), Dot(ey, rhs), Dot(ez, rhs), Dot(ew, rhs));
		}

		const Matrix4 operator*(const Matrix4& rhs) const
		{
			Matrix4 m = Matrix4(
				*this * rhs.Column0(), *this * rhs.Column1(),
				*this * rhs.Column2(), *this * rhs.Column3());
			m.Transpose();
			return m;
		}

		/// <summary>
		/// Get a 4x4 identity matrix
		/// </summary>
		static Matrix4 identity()
		{
			Matrix4 m = Matrix4();
			m.ex.x = m.ey.y = m.ez.z = m.ew.w = 1;
			return m;
		}

	private:

		void Transpose()
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
	inline Matrix4 Transpose(const Matrix4& m)
	{
		return Matrix4(m.Column0(), m.Column1(), m.Column2(), m.Column3());
	}

	/// <summary>
	/// Perform a sandwich operation on B by A
	/// </summary>
	/// <remarks> The product is a rotation of B that oriented relative to A coordinate axes </remarks>
	inline Matrix4 Transform(const Matrix4& a, const Matrix4& b)
	{
		return Transpose(a) * b * a;
	}

	/// <summary>
	/// Returns the absolute version of the matrix.
	/// </summary>
	inline Matrix4 Abs(const Matrix4& m)
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
	inline Matrix4 LookAt(const Vector4& overward)
	{
		Vector4 a = Normalize(overward);

		// Fastest solution
		return Matrix4(
			a.w, -a.z, a.y, -a.x,
			a.z, a.w, -a.x, -a.y,
			-a.y, a.x, a.w, -a.z,
			a.x, a.y, a.z, a.w
		);
	}

	/// <summary>
	/// Create rotation matrix that rotates *from* direction with *to* direction
	/// </summary>
	inline Matrix4 Delta(Vector4 from, Vector4 to)
	{
		return Transpose(LookAt(from)) * (LookAt(to));
	}
}
