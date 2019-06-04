#include "Frustum4.h"

namespace Forth
{
	void Frustum4::Setup()
	{
		// viewer.position -= viewer.rotation * Vector4(3, shiftClip);
		ratio = 1 / focalLength;
		nearPlane = {nearClip, Vector4(3, 1)};
		farPlane = {farClip, Vector4(3, -1)};

		if (useFrustumCulling)
		{
			auto normalizer = 1 / Sqrt(ratio * ratio + 1); // To keep the length = 1
			xMinPlane = {0.f, (Vector4(3, -ratio) - Vector4(0, 1)) * normalizer};
			xMaxPlane = {0.f, (Vector4(3, -ratio) + Vector4(0, 1)) * normalizer};
			yMinPlane = {0.f, (Vector4(3, -ratio) - Vector4(1, 1)) * normalizer};
			yMaxPlane = {0.f, (Vector4(3, -ratio) + Vector4(1, 1)) * normalizer};
			zMinPlane = {0.f, (Vector4(3, -ratio) - Vector4(2, 1)) * normalizer};
			zMaxPlane = {0.f, (Vector4(3, -ratio) + Vector4(2, 1)) * normalizer};
		}
	}

	Vector4 Frustum4::Clip(const Vector4 &vertex, const Vector4 &other) const
	{
		// Get intersection between vertex (which lies outside) and other (lies inside)

		float rP = ratio * vertex.w, rN = -rP, interpol = 0;

		if (vertex.w < nearClip)
			interpol = Max(interpol, nearPlane.Intersect(vertex, other));
		else if (vertex.w > farClip)
			interpol = Max(interpol, farPlane.Intersect(vertex, other));

		if (useFrustumCulling)
		{
			if (vertex.x < rN)
				interpol = Max(interpol, xMinPlane.Intersect(vertex, other));
			else if (vertex.x > rP)
				interpol = Max(interpol, xMaxPlane.Intersect(vertex, other));

			if (vertex.y < rN)
				interpol = Max(interpol, yMinPlane.Intersect(vertex, other));
			else if (vertex.y > rP)
				interpol = Max(interpol, yMaxPlane.Intersect(vertex, other));

			if (vertex.z < rN)
				interpol = Max(interpol, zMinPlane.Intersect(vertex, other));
			else if (vertex.z > rP)
				interpol = Max(interpol, farPlane.Intersect(vertex, other));
		}

		return LerpUnclamped(vertex, other, interpol);
	}

	int Frustum4::OrthographicXYZ(int axis, Vector4 input[], int length, Vector4 result[])
	{
		int count = 0;

		for (int i = 0; i < length; ++i)
		{
			Vector4 a = input[i], b = input[(i + 1) % length];

			float da = Abs(a[axis]) - ratio * a.w, db = Abs(b[axis]) - ratio * b.w;

			if (da < 0 && db < 0)
				result[count++] = b;
			else if ((da < 0) ^ (db < 0))
			{
				// Intersection point between A and B
				auto cv = a + (b - a) * (da / (da - db));

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

	int Frustum4::OrthographicW(Vector4 input[], int length, Vector4 result[])
	{
		int count = 0;

		for (int i = 0; i < length; ++i)
		{
			Vector4 a = input[i], b = input[(i + 1) % length];

			float da = -(a.w) + nearClip, db = -(b.w) + nearClip;

			if (da < 0 && db < 0)
				result[count++] = b;
			else if ((da < 0) ^ (db < 0))
			{
				// Intersection point between A and B
				auto cv = a + (b - a) * (da / (da - db));

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
			Vector4 a = result[i], b = result[(i + 1) % count];

			float da = (a.w) - farClip, db = (b.w) - farClip;

			if (da < 0 && db < 0)
				input[count2++] = b;
			else if ((da < 0) ^ (db < 0))
			{
				// Intersection point between A and B
				auto cv = a + (b - a) * (da / (da - db));

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

	Vector4 Frustum4::Clip(const Vector4 &vertex, const Vector4 &other, float &interpol)
	{
		// Get intersection between vertex (which lies outside) and other (lies inside)

		auto flag = GetSide(vertex);
		interpol = 0;

		if (useFrustumCulling)
		{
			if ((flag & 0x1) > 0)
				interpol = Max(interpol, xMinPlane.Intersect(vertex, other));
			else if ((flag & 0x2) > 0)
				interpol = Max(interpol, xMaxPlane.Intersect(vertex, other));

			if ((flag & 0x4) > 0)
				interpol = Max(interpol, yMinPlane.Intersect(vertex, other));
			else if ((flag & 0x8) > 0)
				interpol = Max(interpol, yMaxPlane.Intersect(vertex, other));

			if ((flag & 0x10) > 0)
				interpol = Max(interpol, zMinPlane.Intersect(vertex, other));
			else if ((flag & 0x20) > 0)
				interpol = Max(interpol, zMaxPlane.Intersect(vertex, other));
		}

		if ((flag & 0x40) > 0)
			interpol = Max(interpol, nearPlane.Intersect(vertex, other));
		else if ((flag & 0x80) > 0)
			interpol = Max(interpol, farPlane.Intersect(vertex, other));

		return LerpUnclamped(vertex, other, interpol);
	}

	int Frustum4::GetSide(const Vector4 &v)
	{
		int flag = 0;
		float rP = ratio * v.w, rN = -rP;

		if (v.w < nearClip)
			flag |= 0x40;
		else if (v.w > farClip)
			flag |= 0x80;

		if (useFrustumCulling)
		{
			if (v.x < rN)
				flag |= 0x1;
			else if (v.x > rP)
				flag |= 0x2;

			if (v.y < rN)
				flag |= 0x4;
			else if (v.y > rP)
				flag |= 0x8;

			if (v.z < rN)
				flag |= 0x10;
			else if (v.z > rP)
				flag |= 0x20;
		}

		return flag;
	}

	bool Frustum4::GetSideTest(const Vector4 &v) const
	{
		if (v.w < nearClip || v.w > farClip)
			return false;

		if (useFrustumCulling)
		{
			auto ratio = this->ratio * v.w;
			if (Abs(v.x) > ratio)
				return false;
			if (Abs(v.y) > ratio)
				return false;
			if (Abs(v.z) > ratio)
				return false;
		}

		return true; // Definitely inside
	}

	void Frustum4::InternalProject1(const Buffer4 &source, Visualizer4 *dest)
	{
		int *t4 = source.indices;
		auto t4c = source.indiceCount;
		auto s = sides;
		auto t = _temp;
		// Loop over all point
		for (int i = 0; i < t4c; i += 1)
		{
			// Get the case
			switch (GetCase(s[t4[i + 0]]))
			{
			case FRC_Inside:
				// Add 'em all & Push
				t[0] = ProjectPoint(vmverts[t4[i]]);
				dest->Render(t, 1);
				break;
			}
		}
	}

	void Frustum4::InternalProject2(const Buffer4 &source, Visualizer4 *dest)
	{
		int *t4 = source.indices;
		auto t4c = source.indiceCount;
		auto s = sides;
		auto t = _temp;
		// Loop over all edge
		for (int i = 0; i < t4c; i += 2)
		{
			// Get the case
			int a, b;
			switch (GetCase(s[a = t4[i + 0]], s[b = t4[i + 1]]))
			{
			case FRC_Inside:
				// Add 'em all & Push
				t[0] = ProjectPoint(vmverts[a]);
				t[1] = ProjectPoint(vmverts[b]);
				dest->Render(t, 2);
				break;
			case FRC_Intersect:
				if (s[a])
				{
					t[0] = ProjectPoint(vmverts[a]);
					t[1] = ProjectPoint(Clip(vmverts[b], vmverts[a]));
				}
				else
				{
					t[0] = ProjectPoint(Clip(vmverts[a], vmverts[b]));
					t[1] = ProjectPoint(vmverts[b]);
				}
				dest->Render(t, 2);
				break;
			}
		}
	}

	void Frustum4::InternalProject3(const Buffer4 &source, Visualizer4 *dest)
	{
		int *t4 = source.indices;
		auto t4c = source.indiceCount;
		auto s = sides;
		auto t = _temp;

		// Loop over all triangle
		for (int i = 0; i < t4c; i += 3)
		{
			// Get the case
			int a, b, c;
			switch (GetCase(s[a = t4[i + 0]], s[b = t4[i + 1]], s[c = t4[i + 2]]))
			{
			case FRC_Inside:
				// Add 'em all & Push
				t[0] = ProjectPoint(vmverts[a]);
				t[1] = ProjectPoint(vmverts[b]);
				t[2] = ProjectPoint(vmverts[c]);
				dest->Render(t, 3);
				break;
			case FRC_Intersect:
				t[0] = (vmverts[a]);
				t[1] = (vmverts[b]);
				t[2] = (vmverts[c]);

				// Special algorithm
				int count = OrthographicW(_temp, 3, _temp2);
				if (useFrustumCulling)
				{
					count = OrthographicXYZ(0, _temp, count, _temp2);
					count = OrthographicXYZ(1, _temp2, count, _temp);
					count = OrthographicXYZ(2, _temp, count, _temp2);
				}

				for (int j = 0; j < count; j++)
					_temp2[j] = ProjectPoint(_temp2[j]);
				dest->Render(_temp2, count);
				break;
			}
		}
	}

	bool Frustum4::IsSphereInFrustum(const SphereBounds4 &bound) const
	{
		auto radius = bound.radius;
		if (useFrustumCulling)
		{
			auto xyz = view * (bound.center);
			auto ratio = this->ratio * xyz.w;

			if (xyz.w + radius < nearClip || xyz.w - radius > farClip)
				return false;
			if (xyz.x + radius < -ratio || xyz.x - radius > ratio)
				return false;
			if (xyz.y + radius < -ratio || xyz.y - radius > ratio)
				return false;
			if (xyz.z + radius < -ratio || xyz.z - radius > ratio)
				return false;
			return true;
		}
		else
		{
			float n = Dot(view * Vector4(3, 1.f), bound.center + view.position);
			return (n + radius >= nearClip && n - radius < farClip);
		}
	}

	void Frustum4::Project(const Buffer4 &source, const Transform4 &transform, Visualizer4 *dest)
	{
		viewmodel = view * transform;
		auto v4 = source.vertices;
		auto v4c = source.verticeCount;

		EnsureCapacity(&sides, 0, &sides_cap, v4c);
		EnsureCapacity(&vmverts, 0, &vmverts_cap, v4c);

		// Predetect vertex sides on plane
		for (int i = 0; i < v4c; i++)
			sides[i] = GetSideTest(vmverts[i] = viewmodel * v4[i]);

		switch (source.simplex)
		{
		case SM_Point:
			InternalProject1(source, dest);
			break;
		case SM_Line:
			InternalProject2(source, dest);
			break;
		case SM_Triangle:
			InternalProject3(source, dest);
			break;
		}
	}
} // namespace Forth