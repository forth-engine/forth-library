#include "Matrix4.h"

namespace Forth
{
	/// <summary>
	/// Convert degree euler to orthogonal matrix rotation individually.
	/// </summary>
	/// <remarks>
	/// This creates a rotation matrix that rotates a point by Y, Z, X, T, U, then V. In that order.
	/// </remarks>
	Matrix4 Euler(float x, float y, float z, float t, float u, float v)
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
	Matrix4 Euler(int axis, float degree)
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