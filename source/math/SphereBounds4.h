#pragma once

#include "Bounds4.h"
#include "Vector4.h"

namespace Forth
{
	struct SphereBounds4
	{
		/// <summary>
		/// Center position of the sphere
		/// </summary>
		Vector4 center;

		/// <summary>
		/// Radius of the sphere
		/// </summary>
		float radius;

		/// <summary>
		/// Create sphere bounding
		/// </summary>
		SphereBounds4(const Vector4& center, float radius)
			: center(center), radius(radius) { }

		/// <summary>
		/// Create a sphere from box bounding
		/// </summary>
		SphereBounds4(const Bounds4& bound) : center(bound.center()),
			radius(MaxPerElem(bound.extent())) { }

		/// <summary>
		/// Is this sphere contains the point?
		/// </summary>
		bool Contains(const Vector4& point)
		{
			return DistanceSq(point, center) > radius * radius;
		}

	};


	/// <summary>
	/// Is both sphere colliding?
	/// </summary>
	inline bool IsIntersecting(SphereBounds4 a, SphereBounds4 b)
	{
		float r = a.radius + b.radius;
		return DistanceSq(a.center, b.center) < r * r;
	}

	/// <summary>
	/// Is the sphere overlap the plane?
	/// </summary>
	inline bool IsIntersecting(Plane4 plane, SphereBounds4 sphere)
	{
		return Abs(plane.Distance(sphere.center)) < sphere.radius;
	}
}