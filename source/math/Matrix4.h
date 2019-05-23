#pragma once


#include "Vector4.h"
#include <cmath>

#define SWAP(a, b, c) c=a;a=b;b=c;
#define DEG2RAD PI / 180.f

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

		Matrix4() { ex = Vector4(); ey = Vector4(); ez = Vector4(); ew = Vector4(); }

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

	 /// <summary>
        /// Convert degree euler to orthogonal matrix rotation individually.
        /// </summary>
        /// <remarks>
        /// This creates a rotation matrix that rotates a point by Y, Z, X, T, U, then V. In that order.
        /// </remarks>
        inline Matrix4 Euler(float x, float y, float z, float t, float u, float v)
        {
            // Multiplication matrices, in order
            float y2 = y * DEG2RAD, a = cos(y2), b = sin(y2); // CW
            float z2 = z * DEG2RAD, c = cos(z2), d = sin(z2); // CCW
            float x2 = x * DEG2RAD, f = cos(x2), g = sin(x2); // CCW
            float t2 = t * DEG2RAD, h = cos(t2), j = sin(t2); // CCW
            float u2 = u * DEG2RAD, k = cos(u2), l = sin(u2); // CCW
            float v2 = v * DEG2RAD, m = cos(v2), n = sin(v2); // CCW

            // Premultiplied code
            //var p1 = (b * g + a * d * f);
            //var p2 = (b * d * f + a * g);
            //var p3 = (c * j * k);
            //var p4 = -(d * j * k) + (c * f * l);
            //var p6 = (a * d * g - b * f);
            //var p7 = (a * f - b * d * g);
            //var p8 = (j * l);
            //var p9 = (c * h);
            //var p10 = (c * g);

            // This is corrected version than below (duh)
            return Matrix4(a * c * h,
                 -d * h,
                 b * c * h,
                 -j,
                 k * (a * d * f + b * g) - a * c * j * l,
                 c * f * k + d * j * l,
                 k * (b * d * f - a * g) - b * c * j * l,
                 -h * l,
                 -l * n * (a * d * f + b * g) + m * (a * d * g - b * f) - a * c * j * k * n,
                 c * g * m + d * j * k * n - c * f * l * n,
                 -l * n * (b * d * f - a * g) + m * (b * d * g + a * f) - b * c * j * k * n,
                 -h * k * n,
                 l * m * (a * d * f + b * g) + n * (a * d * g - b * f) + a * c * j * k * m,
                 c * f * l * m - d * j * k * m + c * g * n,
                 l * m * (b * d * f - a * g) + n * (b * d * g + a * f) + b * c * j * k * m,
                 h * k * m
                 );

            // matrix{{ach, dh, -bch, -j}, {k(bg-adf)-acjl, cfk-djl, k(bdf+ag)+bcjl, -hl},
            // { -ln(bg-adf)+m(adg+bf)-acjkn, -cgm-cfln-djkn, -ln(bdf+ag)+m(af-bdg)+bcjkn, -hkn},
            // { lm(bg-adf)+n(adg+bf)+acjkm, cflm+djkm-cgn, lm(bdf+ag)+n(af-bdg)-bcjkm, hkm}}

            // matrix{{ach, -dh, bch, -j}, {k(adf+bg)-acjl, cfk+djl, k(bdf-ag)-bcjl, -hl},
            // { -ln(adf+bg)+m(adg-bf)-acjkn, cgm+djkn-cfln, -ln(bdf-ag)+m(bdg+af)-bcjkn, -hkn},
            // { lm(adf+bg)+n(adg-bf)+acjkm, cflm-djkm+cgn, lm(bdf-ag)+n(bdg+af)+bcjkm, hkm}}

            //return new Matrix4(
            //     a * p9,
            //     -d * h,
            //     b * p9,
            //     -j,
            //     k * p1 - a * c * p8,
            //     c * f * k + d * p8,
            //     k * p2 - b * c * p8,
            //     -h * l,
            //     m * p6 - (a * p3 + l * p1) * n,
            //     p10 * m - p4 * n,
            //     m * p7 + (b * p3 - l * p2) * n,
            //     -h * k * n,
            //     n * p6 + (a * p3 + l * p1) * m,
            //     -p10 * n + p4 * m,
            //      n * p7 - (b * p3 - l * p2) * m,
            //     h * k * m
            //  );

        }

        /// <summary>
        /// Convert given degree rotation in given axis to orthogonal matrix rotation.
        /// </summary>
        /// <remarks>This method is much optimized than Euler(new Euler4(axis, degree))</remarks>
        inline Matrix4 Euler(int axis, float degree)
        {
            float s = sin(degree * DEG2RAD), c = cos(degree * DEG2RAD);
            Matrix4 m = Matrix4::identity();
            switch (axis)
            {
                case 0: m.ey.y = c; m.ez.z = c; m.ey.z = -s; m.ez.y = s; return m;
                case 1: m.ex.x = c; m.ez.z = c; m.ex.z = s; m.ez.x = -s; return m;
                case 2: m.ex.x = c; m.ey.y = c; m.ex.y = -s; m.ey.x = s; return m;
                case 3: m.ex.x = c; m.ew.w = c; m.ex.w = -s; m.ew.x = s; return m;
                case 4: m.ey.y = c; m.ew.w = c; m.ey.w = -s; m.ew.y = s; return m;
                case 5: m.ez.z = c; m.ew.w = c; m.ez.w = -s; m.ew.z = s; return m;
                default: return m;
            }
        }
}
