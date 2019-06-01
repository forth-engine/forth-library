#pragma once

#include "../math/Math.h"
#include "../math/Matrix4.h"
#include "../math/Vector4.h"
#include "Color.h"

namespace Forth
{
	struct VertexProfile
	{
		Color color;
		Vector4 uv, uv2, uv3;

		VertexProfile(void) {}
		VertexProfile(Forth::Color c) : color(c) {}
		VertexProfile(Forth::Color c, Vector4 uv) : color(c), uv(uv) {}
		VertexProfile(Forth::Color c, Vector4 uv, Vector4 uv2) : color(c), uv(uv), uv2(uv2) {}
		VertexProfile(Forth::Color c, Vector4 uv, Vector4 uv2, Vector4 uv3) : color(c), uv(uv), uv2(uv2), uv3(uv3) {}

		VertexProfile(Vector4 uv) : uv(uv) {}
		VertexProfile(Vector4 uv, Vector4 uv2) : uv(uv), uv2(uv2) {}
		VertexProfile(Vector4 uv, Vector4 uv2, Vector4 uv3) : uv(uv), uv2(uv2), uv3(uv3) {}

		static VertexProfile initial(void)
		{
			return VertexProfile(Forth::Color(1, 1, 1, 1));
		}
	};

	/// <summary>
	/// Interpolate between two profile (without clamping)
	/// </summary>
	inline VertexProfile LerpUnclamped(const VertexProfile &a, const VertexProfile &b, float t)
	{
		return VertexProfile(
			LerpUnclamped(a.color, b.color, t),
			LerpUnclamped(a.uv, b.uv, t),
			LerpUnclamped(a.uv2, b.uv2, t),
			LerpUnclamped(a.uv3, b.uv3, t));
	}

	/// <summary>
	/// Interpolate between two profile
	/// </summary>
	inline VertexProfile Lerp(const VertexProfile &a, const VertexProfile &b, float t)
	{
		return LerpUnclamped(a, b, Clamp01(t));
	}

	/// <summary>
	/// Transform UV Profile
	/// </summary>
	inline VertexProfile Transform(Matrix4 t, VertexProfile profile)
	{
		return VertexProfile(profile.color,
							 t * profile.uv,
							 t * profile.uv2,
							 t * profile.uv3);
	}
} // namespace Forth
