#pragma once

namespace Forth
{
	struct Color
	{
		float r, g, b, a;

		Color(void) { }

		Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) { }
	};

	/// <summary>
	/// Interpolate between two colors (without clamping)
	/// </summary>
	inline Color LerpUnclamped(const Color& a, const Color& b, float t)
	{
		return Color(
			a.r + (b.r - a.r) * t,
			a.g + (b.g - a.g) * t,
			a.b + (b.b - a.b) * t,
			a.a + (b.a - a.a) * t
		);
	}
}