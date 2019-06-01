#pragma once

#include "../extras/Utils.h"
#include "../math/Transform4.h"
#include "Projector4.h"
#include <vector>

namespace Forth
{
	class CrossSection : public Projector4
	{

		Transform4 view, viewmodel;
		bool *sides = new bool[4];
		int sides_cap = 4;
		Vector4 _temp[4];
		VertexProfile _temp2[4];

		// Internal separate projection methods
		void InternalProject1(const Buffer4 &source, Visualizer4 *dest);

		// Internal separate projection methods
		void InternalProject2(const Buffer4 &source, Visualizer4 *dest);

		// Internal separate projection methods
		void InternalProject3(const Buffer4 &source, Visualizer4 *dest);

	  public:
		CrossSection(void);

		~CrossSection(void)
		{
			delete[] sides;
		}

		/// <summary>
		/// Called by the viewer to initialize the projction
		/// </summary>
		void Setup(const Transform4 &viewer) override
		{
			view = Inverse(viewer);
		}

		/// <summary>
		/// Dynamic projection
		/// </summary>
		void Project(const Buffer4 &source, const Transform4 &transform, Visualizer4 *dest) override;

		/// <summary>
		/// Arbitrary (4D to 3D) point projection
		/// </summary>
		Vector3 Project(const Vector4 &v) const override
		{
			auto q = (viewmodel * v);
			return q.ToVec3();
		}

		/// <summary>
		/// Arbitrary (4D to 3D) point projection with cull verification
		/// </summary>
		Vector3 Project(const Vector4 &v, bool *culled) const override
		{
			return (viewmodel * v).ToVec3();
		}

		/// <summary>
		/// For static objects, see if given object AABB can be culled out completely
		/// </summary>
		bool IsCullable(const SphereBounds4 &bound) const override
		{
			// IsIntersecting(slicer, bound); <- Unoptimized
			return Abs(bound.center.w + view.position.w) > bound.radius;
		}

		/// <summary>
		/// Adapt to simplex requirement for this projection.
		/// </summary>
		SimplexMode SimplexModeForVisualizing(SimplexMode mode) const override
		{
			return (SimplexMode)(mode - 1);
		};
	};

	/// <summary>
	/// Internally do Plane4.Intersect(), then Lerp()
	/// </summary>
	inline Vector4 CrossInterpolate(const Vector4 &x, const Vector4 &y)
	{
		return x + (y - x) * (x.w / (x.w - y.w));
	}

	/// <summary>
	/// Internally do Plane4.Intersect(), then Lerp(), and make the phase out
	/// </summary>
	inline Vector4 CrossInterpolate(const Vector4 &x, const Vector4 &y, float *phase)
	{
		return x + (y - x) * (*phase = (x.w / (x.w - y.w)));
	}

	inline bool IsEqual(const bool a, const bool b, const bool c)
	{
		return (a == b) && (c == b);
	}

	inline bool IsEqual(const bool a, const bool b, const bool c, const bool d)
	{
		return (a == b) && (c == d) && (b == c);
	}

	const int _leftEdges[] = {
		0,
		0,
		1,
		2,
		1,
		0,
	};

	const int _rightEdges[] = {
		1,
		2,
		2,
		3,
		3,
		3,
	};

} // namespace Forth