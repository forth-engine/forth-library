#pragma once
#include "Euler4.h"
#include "Matrix4.h"
#include "Transform4.h"

namespace Forth
{
	/// <summary>
	/// 6x6 matrix to represet Tensor inertia of an object in 4D.
	/// </summary>
	/// <remarks>
	/// Because its immense computation size, this matrix is intended
	/// only for holding inertia data for the internal physics engine.
	/// The elements is stored in row-major order.
	/// </remarks>
	struct Tensor4
	{
		/// <summary> First row of the matrix </summary>
		Euler4 ex;
		/// <summary> Second row of the matrix </summary>
		Euler4 ey;
		/// <summary> Third row of the matrix </summary>
		Euler4 ez;
		/// <summary> Fourth row of the matrix </summary>
		Euler4 et;
		/// <summary> Fifth row of the matrix </summary>
		Euler4 eu;
		/// <summary> Sixth row of the matrix </summary>
		Euler4 ev;

		Tensor4() : ex(Euler4()), ey(Euler4()), ez(Euler4()), et(Euler4()), eu(Euler4()), ev(Euler4()) {}
		/// <summary>
		/// Create a diagonally uniform tensor matrix.
		/// </summary>
		Tensor4(const float scale)
		{
			ex = Euler4(scale, 0, 0, 0, 0, 0);
			ey = Euler4(0, scale, 0, 0, 0, 0);
			ez = Euler4(0, 0, scale, 0, 0, 0);
			et = Euler4(0, 0, 0, scale, 0, 0);
			eu = Euler4(0, 0, 0, 0, scale, 0);
			ev = Euler4(0, 0, 0, 0, 0, scale);
		}

		/// <summary>
		/// Create a diagonal tensor matrix.
		/// </summary>
		Tensor4(const Euler4 &scale)
		{
			ex = Euler4(scale.x, 0, 0, 0, 0, 0);
			ey = Euler4(0, scale.y, 0, 0, 0, 0);
			ez = Euler4(0, 0, scale.z, 0, 0, 0);
			et = Euler4(0, 0, 0, scale.t, 0, 0);
			eu = Euler4(0, 0, 0, 0, scale.u, 0);
			ev = Euler4(0, 0, 0, 0, 0, scale.v);
		}

		/// <summary>
		/// Create and assign matrix values with rows.
		/// </summary>
		Tensor4(const Euler4 &x, const Euler4 &y, const Euler4 &z, const Euler4 &t, const Euler4 &u, const Euler4 &v)
		{
			ex = x;
			ey = y;
			ez = z;
			et = t;
			eu = u;
			ev = v;
		}

		/// <summary>
		/// Get Nth-row of the matrix
		/// </summary>
		inline Euler4 operator[](int i) const
		{
			return (&ex)[i];
		}

		/// <summary>
		/// Set an axis value of specified index.
		/// </summary>
		inline Euler4 &operator[](int i)
		{
			return (&ex)[i];
		}

		/// <summary>
		/// Get Nth-column of the matrix
		/// </summary>
		Euler4 Column(const int i) const
		{
			return Euler4(ex[i], ey[i], ez[i], et[i], eu[i], ev[i]);
		}

		/// <summary>
		/// Set Nth-column of the matrix
		/// </summary>
		Euler4 &Column(const int i)
		{
			return Euler4(ex[i], ey[i], ez[i], et[i], eu[i], ev[i]);
		}

		/// <summary>
		/// Transform euler rotation by the tensor.
		/// </summary>
		Euler4 operator*(const Euler4 &rhs)
		{
			return Euler4(Dot(ex, rhs), Dot(ey, rhs), Dot(ez, rhs), Dot(et, rhs), Dot(eu, rhs), Dot(ev, rhs));
		}

		/// <summary>
		/// Combine two tensor.
		/// </summary>
		Tensor4 operator*(const Tensor4 &rhs)
		{
			Tensor4 &lhs = *this;
			return Tensor4(lhs * rhs.Column(0), lhs * rhs.Column(1), lhs * rhs.Column(2), lhs * rhs.Column(3), lhs * rhs.Column(4), lhs * rhs.Column(5));
		}

		/// <summary>
		/// Scale the tensor.
		/// </summary>
		Tensor4 operator*(float f)
		{
			return Tensor4(ex * f, ey * f, ez * f, et * f, eu * f, ev * f);
		}

		/// <summary>
		/// Scale the tensor.
		/// </summary>
		Tensor4 operator+(const Tensor4 &rhs)
		{
			return Tensor4(ex + rhs.ex, ey + rhs.ey, ez + rhs.ez, et + rhs.et, eu + rhs.eu, ev + rhs.ev);
		}

		/// <summary>
		/// Element-wisely subract two tensor.
		/// </summary>
		Tensor4 operator-(Tensor4 rhs)
		{
			return Tensor4(
				ex - rhs.ex,
				ey - rhs.ey,
				ez - rhs.ez,
				et - rhs.et,
				eu - rhs.eu,
				ev - rhs.ev);
		}

		Tensor4 operator+=(const Tensor4 &rhs)
		{
			ex += rhs.ex;
			ey += rhs.ey;
			ez += rhs.ez;
			et += rhs.et;
			eu += rhs.eu;
			ev += rhs.ev;
			return *this;
		}

		Tensor4 operator-=(const Tensor4 &rhs)
		{
			ex -= rhs.ex;
			ey -= rhs.ey;
			ez -= rhs.ez;
			et -= rhs.et;
			eu -= rhs.eu;
			ev -= rhs.ev;
			return *this;
		}

		Tensor4 operator*=(const Tensor4 &rhs)
		{
			Tensor4 &lhs = *this;
			ex = lhs * rhs.Column(0);
			ey = lhs * rhs.Column(1);
			ez = lhs * rhs.Column(2);
			et = lhs * rhs.Column(3);
			eu = lhs * rhs.Column(4);
			ev = lhs * rhs.Column(5);
			return *this;
		}

		/// <summary>
		/// Create a tensor with zero values.
		/// </summary>
		inline static Tensor4 zero()
		{
			return Tensor4();
		}

		/// <summary>
		/// Create a tensor as an identity matrix.
		/// </summary>
		inline static Tensor4 identity()
		{
			return Tensor4(1);
		}
	};

	/// <summary>
	/// Create inertia from rotation matrix.
	/// </summary>
	inline Tensor4 Cross(const Matrix4 &t)
	{
		return Tensor4(
			Cross(t.ey, t.ez),
			Cross(t.ez, t.ex),
			Cross(t.ex, t.ey),
			Cross(t.ex, t.ew),
			Cross(t.ey, t.ew),
			Cross(t.ez, t.ew));
	}

	/// <summary>
	/// Transpose the tensor.
	/// </summary>
	inline Tensor4 Transpose(const Tensor4 &m)
	{
		return Tensor4(m.Column(0), m.Column(1), m.Column(2), m.Column(3), m.Column(4), m.Column(5));
	}

	/// <summary>
	/// ? (WIP)
	/// </summary>
	inline Tensor4 OuterProduct(const Vector4 &u, const Vector4 &v)
	{
		//Vector4 a = v * u.x;
		//Vector4 b = v * u.y;
		//Vector4 c = v * u.z;

		return Tensor4();
	}

	/// <summary>
	/// Rotate the tensor orientation by the matrix
	/// </summary>
	inline Tensor4 Transform(const Tensor4 &I, const Matrix4 &r)
	{
		Tensor4 t = Cross(r);
		return t * I * Transpose(t);
	}

	/// <summary>
	/// Create a tensor translation.
	/// </summary>
	inline Tensor4 Transform(const Vector4 &p, float mass)
	{
		return Tensor4(LengthSq(p)) - (OuterProduct(p, p) * mass);
	}

	/// <summary>
	/// Transform the inertia.
	/// </summary>
	inline Tensor4 Transform(const Tensor4 &I, const Transform4 &T, float mass)
	{
		return Transform(I, T.rotation) + Transform(T.position, mass);
	}

	/// <summary>
	/// Inverse the tensor.
	/// </summary>
	/// <remarks>
	/// The operation is working, but unoptimized, yet expensive.
	/// </remarks>
	Tensor4 Inverse(const Tensor4 &m);

} // namespace Forth
