#pragma once

#include "Projector4.h"
#include "../math/Transform4.h"
#include "../math/Plane4.h"
#include "../math/Math.h"
#include <vector>

namespace Forth
{
	class Frustum : public Projector4
	{

		Transform4 view, viewmodel;
		std::vector<bool> sides;
		Vector4 _temp[9];
		Vector4 _temp2[9];
		VertexProfile _temp3[9];

		float ratio;
		Plane4 nearPlane, farPlane;
		Plane4 xMaxPlane, xMinPlane;
		Plane4 yMaxPlane, yMinPlane;
		Plane4 zMaxPlane, zMinPlane;

		// Internal separate projection methods
		void InternalProject1(const Buffer4& source, Visualizer4 *dest);

		// Internal separate projection methods
		void InternalProject2(const Buffer4& source, Visualizer4 *dest);

		// Internal separate projection methods
		void InternalProject3(const Buffer4& source, Visualizer4 *dest);

		Vector4 ProjectPoint(const Vector4& v) const
		{
			// if (perspectiveness < 1e-4f) return v;
			return v * Lerp(1, ratio / v.w, perspectiveness);
		}

		// The optimized Frustum <-> segment Intersection.
		Vector4 Clip(const Vector4& vertex, const Vector4& other) const
		{
			// Get Intersection between vertex (which lies Outside) and other (lies Inside)

			float rP = ratio * vertex.w;
			float rN = -rP;
			float interpol = 0;

			if (vertex.w < nearClip) interpol = Max(interpol, nearPlane.Intersect(vertex, other));
			else if (vertex.w > farClip) interpol = Max(interpol, farPlane.Intersect(vertex, other));

			if (useFrustumCulling)
			{
				if (vertex.x < rN) interpol = Max(interpol, xMinPlane.Intersect(vertex, other));
				else if (vertex.x > rP) interpol = Max(interpol, xMaxPlane.Intersect(vertex, other));

				if (vertex.y < rN) interpol = Max(interpol, yMinPlane.Intersect(vertex, other));
				else if (vertex.y > rP) interpol = Max(interpol, yMaxPlane.Intersect(vertex, other));

				if (vertex.z < rN) interpol = Max(interpol, zMinPlane.Intersect(vertex, other));
				else if (vertex.z > rP) interpol = Max(interpol, farPlane.Intersect(vertex, other));
			}

			return LerpUnclamped(vertex, other, interpol);
		}


		bool GetSideTest(const Vector4& v) const
		{
			if (v.w < nearClip || v.w > farClip)
				return false;

			if (useFrustumCulling)
			{
				float ratio = this->ratio * v.w;
				return Abs(v.x) > ratio && Abs(v.y) > ratio
					&& Abs(v.z) > ratio;
			}

			return true; // Definitely inside
		}

		int OrthographicXYZ(int axis, Vector4 input[], int length, Vector4 result[]);


		int OrthographicW(Vector4 input[], int length, Vector4 result[]);


	public:


		struct frustumparams
		{
			float perspectiveness = 1;
			float focalLength = 1;
			float nearClip = 0.1f;
			float farClip = 1000.f;
			float shiftClip = 0.f;
			bool useFrustumCulling = true;

			frustumparams() { }
		};


		/// <summary>
		/// Transition between orthographic (0) and perspective (1)
		/// </summary>
		float perspectiveness = 1;
		/// <summary>
		/// Focal length of the camera
		/// </summary>
		float focalLength = 1;
		/// <summary>
		/// Minimum projection distance limit
		/// </summary>
		float nearClip = 0.1f;
		/// <summary>
		/// Maximum projection distance limit
		/// </summary>
		float farClip = 1000.f;
		/// <summary>
		/// Backward shift of the camera
		/// </summary>
		/// <remarks>
		/// It's equivalent to move camera in negative W direction
		/// </remarks>
		float shiftClip = 0.f;
		/// <summary>
		/// Use additional frustum culling?
		/// </summary>
		/// <remarks>
		/// </remarks>
		bool useFrustumCulling = true;

		Frustum() { }

		~Frustum()
		{
			sides.clear();
		}

		void Set(float perps, float focal, float near, float far, float shift, bool clip)
		{
			perspectiveness = perps;
			focalLength = focal;
			nearClip = near;
			farClip = far;
			shiftClip = shift;
			useFrustumCulling = clip;
		}

		void Set(const frustumparams &params)
		{
			perspectiveness = params.perspectiveness;
			focalLength = params.focalLength;
			nearClip = params.nearClip;
			farClip = params.farClip;
			shiftClip = params.shiftClip;
			useFrustumCulling = params.useFrustumCulling;
		}

		/// <summary>
		/// Called by the viewer to initialize the projction
		/// </summary>
		void Setup(const Transform4& viewer) override;

		/// <summary>
		/// Dynamic projection
		/// </summary>
		void Project(const Buffer4& source, const Transform4& transform, Visualizer4 *dest) override;


		/// <summary>
		/// Arbitrary (4D to 3D) point projection
		/// </summary>
		Vector3 Project(const Vector4& v) const override
		{
			return ProjectPoint(view * v).ToVec3();
		}

		/// <summary>
		/// Arbitrary (4D to 3D) point projection with cull verification
		/// </summary>
		Vector3 Project(const Vector4& v, bool *culled) const override
		{
			return ProjectPoint(view * v).ToVec3();
		}

		/// <summary>
		/// For static objects, see if given object AABB can be culled out completely
		/// </summary>
		bool IsCullable(const SphereBounds4& bound) const override
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

	typedef enum { FC_Inside, FC_Intersect, FC_Outside } FrustumCase;

	inline FrustumCase GetCase(bool a)
	{
		return a ? FC_Inside : FC_Outside;
	}

	inline FrustumCase GetCase(bool a, bool b)
	{
		return (a == b) ? (a ? FC_Inside : FC_Outside) : FC_Intersect;
	}

	inline FrustumCase GetCase(bool a, bool b, bool c)
	{
		return (a == b) & (b == c) ? (a ? FC_Inside : FC_Outside) : FC_Intersect;
	}

}