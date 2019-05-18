#pragma once

#include "Frustum.h"
#include "../math/Math.h"
#include "../common/Enums.h"

namespace Forth
{
	void Frustum::InternalProject1(const Buffer4 & source, Visualizer4 * dest)
	{
		int *t4 = source.indices;
		Vector4 *v4 = source.vertices;

		// Loop over all point
		for (int i = 0; i < source.indiceCount; i += 1)
		{
			// Get the case
			switch (GetCase(sides[t4[i + 0]]))
			{
			case FrustumCase::FC_Inside:
				// Add 'em all & Push
				_temp[0] = ProjectPoint(viewmodel * v4[t4[i]]);
				dest->Render(_temp, 1);
				break;
			}
		}
	}

	void Frustum::InternalProject2(const Buffer4 & source, Visualizer4 * dest)
	{
		int *t4 = source.indices;
		Vector4 *v4 = source.vertices;
		// Loop over all edge
		for (int i = 0; i < source.indiceCount; i += 2)
		{
			// Get the case
			int a, b;
			switch (GetCase(sides[a = t4[i + 0]], sides[b = t4[i + 1]]))
			{
			case FrustumCase::FC_Inside:
				// Add 'em all & Push
				_temp[0] = ProjectPoint(viewmodel * v4[a]);
				_temp[1] = ProjectPoint(viewmodel * v4[b]);
				dest->Render(_temp, 2);
				break;
			case FrustumCase::FC_Intersect:
				if (sides[a])
				{
					_temp[0] = ProjectPoint(viewmodel * v4[a]);
					_temp[1] = ProjectPoint(Clip(viewmodel * v4[b], viewmodel * v4[a]));
				}
				else
				{
					_temp[0] = ProjectPoint(Clip(viewmodel * v4[a], viewmodel * v4[b]));
					_temp[1] = ProjectPoint(viewmodel * v4[b]);
				}
				dest->Render(_temp, 2);
				break;
			}
		}
	}

	void Frustum::InternalProject3(const Buffer4 & source, Visualizer4 * dest)
	{
		int *t4 = source.indices;
		Vector4 *v4 = source.vertices;

		// Loop over all triangle
		for (int i = 0; i < source.indiceCount; i += 3)
		{
			// Get the case
			int a, b, c;
			switch (GetCase(sides[a = t4[i + 0]], sides[b = t4[i + 1]], sides[c = t4[i + 2]]))
			{
			case FrustumCase::FC_Inside:
				// Add 'em all & Push
				_temp[0] = ProjectPoint(viewmodel * v4[a]);
				_temp[1] = ProjectPoint(viewmodel * v4[b]);
				_temp[2] = ProjectPoint(viewmodel * v4[c]);
				dest->Render(_temp, 3);
				break;
			case FrustumCase::FC_Intersect:
				_temp[0] = (viewmodel * v4[a]);
				_temp[1] = (viewmodel * v4[b]);
				_temp[2] = (viewmodel * v4[c]);

				// Special algorithm
				int count;
				count = OrthographicW(_temp, 3, _temp2);
				if (useFrustumCulling)
				{
					count = OrthographicXYZ(0, _temp, count, _temp2);
					count = OrthographicXYZ(1, _temp2, count, _temp);
					count = OrthographicXYZ(2, _temp, count, _temp2);
				}

				for (int j = 0; j < count; j++) _temp2[j] = ProjectPoint(_temp2[j]);
				dest->Render(_temp2, count);
				break;
			}
		}
	}

	int Frustum::OrthographicXYZ(int axis, Vector4 input[], int length, Vector4 result[])
	{
		int count = 0;

		for (int i = 0; i < length; ++i)
		{
			const Vector4 &a = input[i], &b = input[(i + 1) % length];

			float da = Abs(a[axis]) - ratio * a.w, db = Abs(b[axis]) - ratio * b.w;

			if (da < 0 && db < 0)
				result[count++] = b;
			else if ((da < 0) ^ (db < 0))
			{
				// Intersection point between A and B
				const Vector4 &cv = a + (b - a) * (da / (da - db));

				if (da < 0)
					result[count++] = cv;
				else
				{
					result[count++] = cv;
					result[count++] = b;
				}
			}
		}
		return count;
	}

	int Frustum::OrthographicW(Vector4 input[], int length, Vector4 result[])
	{
		int count = 0;

		for (int i = 0; i < length; ++i)
		{
			const Vector4 &a = input[i], &b = input[(i + 1) % length];

			float da = -(a.w) + nearClip, db = -(b.w) + nearClip;

			if (da < 0 && db < 0)
				result[count++] = b;
			else if ((da < 0) ^ (db < 0))
			{
				// Intersection point between A and B
				const Vector4 &cv = a + (b - a) * (da / (da - db));

				if (da < 0)
					result[count++] = cv;
				else
				{
					result[count++] = cv;
					result[count++] = b;
				}
			}
		}

		int count2 = 0;

		for (int i = 0; i < count; ++i)
		{
			const Vector4 &a = result[i], &b = result[(i + 1) % count];

			float da = (a.w) - farClip, db = (b.w) - farClip;

			if (da < 0 && db < 0)
				input[count2++] = b;
			else if ((da < 0) ^ (db < 0))
			{
				// Intersection point between A and B
				const Vector4 &cv = a + (b - a) * (da / (da - db));

				if (da < 0)
					input[count2++] = cv;
				else
				{
					input[count2++] = cv;
					input[count2++] = b;
				}
			}
		}

		return count2;
	}

	void Frustum::Setup(const Transform4 &viewer)
	{
		view = viewer;
		view.position = view.position - viewer.rotation * Vector4(3, shiftClip);
		view = Inverse(view);
		ratio = 1.f / focalLength;
		nearPlane = Plane4(Vector4(3, 1), nearClip);
		farPlane = Plane4(Vector4(3, -1), farClip);

		if (useFrustumCulling)
		{
			float normalizer = InvSqrt(ratio * ratio + 1); // To keep the length = 1
			xMinPlane = Plane4((Vector4(3, -ratio) - Vector4(0, 1)) * normalizer, 0);
			xMaxPlane = Plane4((Vector4(3, -ratio) + Vector4(0, 1)) * normalizer, 0);
			yMinPlane = Plane4((Vector4(3, -ratio) - Vector4(1, 1)) * normalizer, 0);
			yMaxPlane = Plane4((Vector4(3, -ratio) + Vector4(1, 1)) * normalizer, 0);
			zMinPlane = Plane4((Vector4(3, -ratio) - Vector4(2, 1)) * normalizer, 0);
			zMaxPlane = Plane4((Vector4(3, -ratio) + Vector4(2, 1)) * normalizer, 0);
		}
	}

	void Frustum::Project(const Buffer4 &source, const Transform4 &transform, Visualizer4 *dest)
	{
		viewmodel = view * transform;
		int *t4 = source.indices;
		Vector4 *v4 = source.vertices;

		// Predetect vertex sides on plane
		sides.clear();
		for (int i = 0; i < source.verticeCount; i++)
			sides.push_back(GetSideTest(viewmodel * v4[i]));

		/*if (source.m_ProfilesCount == source.m_IndicesCount)
			switch (source.simplex)
			{
			case SimplexMode.Point: InternalProjectProfile1(source, dest); break;
			case SimplexMode.Line: InternalProjectProfile2(source, dest); break;
			case SimplexMode.Triangle: InternalProjectProfile3(source, dest); break;
			}
		else*/
			switch (source.simplex)
			{
			case SimplexMode::SM_Point: InternalProject1(source, dest); break;
			case SimplexMode::SM_Line: InternalProject2(source, dest); break;
			case SimplexMode::SM_Triangle: InternalProject3(source, dest); break;
			}
	}
}
