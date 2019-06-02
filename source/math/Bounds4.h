#pragma once

#include "Math.h"
#include "Plane4.h"
#include "Vector4.h"

namespace Forth
{
	/// <summary>
	/// Representation for bounds in 4D.
	/// </summary>
	struct Bounds4
	{
		Vector4 min, max;

		Bounds4(void) {}
		Bounds4(const Vector4 &_extent) : min(-_extent), max(_extent) {}
		Bounds4(const Vector4 &_min, const Vector4 &_max) : min(_min), max(_max) {}

		void Set(const Vector4 &_min, const Vector4 &_max)
		{
			min = _min;
			max = _max;
		}

		void Allocate(const Vector4 &point)
		{
			min = Min(min, point);
			max = Max(max, point);
		}

		inline Vector4 center(void) const
		{
			return (max + min) * 0.5f;
		}

		inline Vector4 extent(void) const
		{
			return (max - min) * 0.5f;
		}

		/// <summary> Is the plane hits part of the bound? </summary>
		inline bool IsIntersecting(Plane4 plane)
		{
			float r = Dot(extent(), Abs(plane.normal));
			float s = Dot(center(), plane.normal) - plane.distance;
			return Abs(s) <= r;
		}

		/// <summary> Is this bound contains the whole cell of the other bound? </summary>
		inline bool Contains(Bounds4 other)
		{
			return min < other.min && max > other.max;
		}

		/// <summary> Is this bound contains the given point </summary>
		inline bool Contains(Vector4 point)
		{
			return min < point && max > point;
		}

		/// <summary> Interpolate two vector by T. </summary>
		inline Vector4 Interpolate(Vector4 t)
		{
			return min + extent() * t * 2;
		}

		/// <summary> Returns a point that either inside or touching the bound </summary>
		inline Vector4 Clamp(const Vector4 &point) const
		{
			Vector4 c = center(), e = extent();
			Vector4 p = point - c;

			for (int i = 0; i < 4; i++)
				p[i] = Forth::Clamp(-e[i], e[i], point[i]);

			return p + c;
		}

		/// <summary> Returns a point that either outside or touching the bound </summary>
		inline Vector4 Clip(Vector4 &t)
		{
			if (!Contains(t))
				return t;

			Vector4 c = center(), e = extent();
			t -= c;

			int a = MinPerElemIdx(e - Abs(t));

			t[a] = Forth::Clamp(e[a], -e[a], t[a]);

			return t + c;
		}

		/// <summary> Check if given ray is colliding with the bound </summary>
		inline bool Raycast(const Vector4 &p, const Vector4 &d)
		{

			Vector4 dI = Invert(d);
			Vector4 t1 = (min - p) * dI;
			Vector4 t2 = (max - p) * dI;

			Vector4 tmin = MaxPerElem(Min(t1, t2));
			Vector4 tmax = MinPerElem(Max(t1, t2));

			return tmax > tmin;
		}

		static Bounds4 infinite(void)
		{
			return Bounds4(Vector4(-INFINITY));
		}
	};

	/// <summary> Determine if and only if A partially contains B </summary>
	inline bool IsIntersecting(const Bounds4 &a, const Bounds4 &b)
	{
		return b.max > a.min && b.min < a.max;
	}

	/// <summary> Combine two bounds </summary>
	inline Bounds4 Combine(Bounds4 a, Bounds4 b)
	{
		return Bounds4(Min(a.min, b.min), Max(a.max, b.max));
	}

	/// <summary> Intersect two bounds </summary>
	inline Bounds4 Intersect(const Bounds4 &a, const Bounds4 &b)
	{
		return Bounds4(Max(a.min, b.min), Min(a.max, b.max));
	}

	/// <summary> Scales the bound's extent </summary>
	inline Bounds4 Scale(const Bounds4 &b, const Vector4 &s)
	{
		Vector4 c = b.center();
		Vector4 e = b.extent() * s;
		return Bounds4(c - e, c + e);
	}

	/// <summary> Compare which Bounds4 is the closest. Positive means L is closer </summary>
	inline float Compare(Vector4 center, const Bounds4 &l, const Bounds4 &r)
	{
		return DistanceSq(r.Clamp(center), center) - DistanceSq(l.Clamp(center), center);
	}

} // namespace Forth
