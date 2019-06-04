#pragma once

#include "../extras/Utils.h"
#include "../math/Plane4.h"
#include "../math/Transform4.h"
#include "Projector4.h"
#include <vector>

namespace Forth
{
	class Frustum4 : public Projector4
	{
		Plane4 nearPlane, farPlane;
		Plane4 xMaxPlane, xMinPlane;
		Plane4 yMaxPlane, yMinPlane;
		Plane4 zMaxPlane, zMinPlane;

		bool *sides = new bool[4];
		int sides_cap = 4;
		Vector4 *vmverts = new Vector4[4];
		int vmverts_cap = 4;
		Vector4 _temp[9];
		Vector4 _temp2[9];

	  public:
		enum FrustumCase
		{
			FRC_Inside,
			FRC_Intersect,
			FRC_Outside
		};


		float ratio;

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

		/// <summary>
		/// Access the calculated field of view angle in degree
		/// </summary>
		float GetFieldOfView() { return atan2(1.f, focalLength) * RAD2DEG; }

		void SetFieldOfView(float value) { focalLength = 1 / tan(Clamp(value, 0.01f, 179.9f) * RAD2DEG); }

		///

		void Setup();

		///

		Vector4 ProjectPoint(const Vector4 &v) const
		{
			return v * (1.f + (ratio / v.w - 1.f) * perspectiveness);
		}

		// The optimized frustum <-> segment intersection.
		Vector4 Clip(const Vector4 &vertex, const Vector4 &other) const;

		int OrthographicXYZ(int axis, Vector4 input[], int length, Vector4 result[]);

		int OrthographicW(Vector4 input[], int length, Vector4 result[]);

		// The optimized frustum <-> segment intersection.
		Vector4 Clip(const Vector4 &vertex, const Vector4 &other, float &interpol);

		// 0x1=x- 0x2=x+ 0x4=y- 0x8=y+ 0x10=z- 0x20=z+
		// 0x40=w- 0x80=w+ 0 = inside. Can be Multiflagged
		int GetSide(const Vector4 &v);

		bool GetSideTest(const Vector4 &v) const;

		// Internal separate projection methods

		void InternalProject1(const Buffer4 &source, Visualizer4 *dest);

		void InternalProject2(const Buffer4 &source, Visualizer4 *dest);

		void InternalProject3(const Buffer4 &source, Visualizer4 *dest);

		static FrustumCase GetCase(bool a)
		{
			return a ? FRC_Inside : FRC_Outside;
		}

		static FrustumCase GetCase(bool a, bool b)
		{
			return (a == b) ? (a ? FRC_Inside : FRC_Outside) : FRC_Intersect;
		}

		static FrustumCase GetCase(bool a, bool b, bool c)
		{
			return (a == b) & (b == c) ? (a ? FRC_Inside : FRC_Outside) : FRC_Intersect;
		}

		///

		bool IsCullable(const SphereBounds4 &bound) const override
		{
			return !IsSphereInFrustum(bound);
		}

		// Keep culling fast by assuming the bound is a sphere
		bool IsSphereInFrustum(const SphereBounds4 &bound) const;

		///

		Vector3 Project(const Vector4 &v) const override
		{
			return ProjectPoint(view * v).ToVec3();
		}

		///

		Vector3 Project(const Vector4 &v, bool *culled) const override
		{
			*culled = GetSideTest(view * v);
			return ProjectPoint(view * v).ToVec3();
		}

		/// <summary>
		/// This method always return the same simplex mode.
		/// </summary>
		SimplexMode SimplexModeForVisualizing(SimplexMode mode) const override
		{
			return mode;
		}

		void Project(const Buffer4 &source, const Transform4 &transform, Visualizer4 *dest) override;

		~Frustum4()
		{
			delete[] sides;
			delete[] vmverts;
		}
	};
} // namespace Forth