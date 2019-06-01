#pragma once

#include "Vector4.h"

namespace Forth
{
	/// <summary>
	/// Mathematical abstraction of a plane that split spaces in 4D.
	/// </summary>
	struct Plane4
	{
		/// <summary>
		/// Distance of the plane from center.
		/// </summary>
		float distance;

		/// <summary>
		/// Normal of the plane.
		/// </summary>
		Vector4 normal;

		/// <summary>
		/// Create empty plane.
		/// </summary>
		Plane4(void) {}

		/// <summary>
		/// Create new plane with given normal at given point.
		/// </summary>
		Plane4(const Vector4 &_norm, const Vector4 &_point) : normal(_norm), distance(Dot(_norm, _point)) {}

		/// <summary>
		/// Create new plane with given normal and distance.
		/// </summary>
		Plane4(const Vector4 &_norm, float _dist) : normal(_norm), distance(_dist) {}

		/// <summary>
		/// Create a new plane from known vertices.
		/// </summary>
		Plane4(Vector4 a, Vector4 b, Vector4 c, Vector4 d)
			: normal(Normalize(Cross(b - a, c - a, d - a))),
			  distance(Dot(normal, a)) {}

		void Set(const Vector4 &_norm, const Vector4 &_point)
		{
			normal = _norm;
			distance = Dot(normal, _point);
		}

		/// <summary>
		/// Get origin of a plane.
		/// </summary>
		const inline Vector4 origin(void) const
		{
			return normal * distance;
		}

		/// <summary>
		/// Get distance between a point and the nearest point on a plane.
		/// </summary>
		/// <remarks>
		/// This method can return a negative value, which mean the point is behind the plane.
		/// </remarks>
		inline float Distance(const Vector4 &point) const
		{
			return Dot(normal, point) - distance;
		}

		/// <summary>
		/// Is the point is above or behind the plane?
		/// </summary>
		inline bool GetSide(const Vector4 &point) const
		{
			return Dot(normal, point) - distance >= -EPSILON;
		}

		/// <summary>
		/// Given an edge represented as two points, return an interpolation where this edge intersects.
		/// </summary>
		/// <remarks>
		/// The resulting interpolation is unclamped, can go beyond 0..1
		/// </remarks>
		inline float Intersect(const Vector4 &a, const Vector4 &b) const
		{
			// Think of an inverse lerp function
			float x = Dot(normal, a);
			float y = Dot(normal, b);
			return y == x ? 0 : (distance - x) / (y - x);
		}

		/// <summary>
		/// Project the point to the nearest point on the plane.
		/// </summary>
		inline Vector4 Project(const Vector4 &point) const
		{
			return point - normal * Distance(point);
		}

		/// <summary>
		/// Is these two points on the same side of the plane?
		/// </summary>
		inline bool SameSide(const Vector4 &a, const Vector4 &b) const
		{
			return Dot(normal, a) * Dot(normal, b) > 0;
		}
	};

} // namespace Forth
