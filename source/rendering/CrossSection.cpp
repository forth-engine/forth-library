
#include "CrossSection.h"

namespace Forth
{

	CrossSection::CrossSection(void) : Projector4()
	{

		Setup(Forth::Transform4::identity());
	}

	void CrossSection::InternalProject1(const Buffer4 &source, Visualizer4 *dest)
	{
		int *t4 = source.indices;
		Vector4 *v4 = source.vertices;
		// Loop over all point
		for (int i = 0; i < source.indiceCount; i += 2)
		{
			// Do initial check if this edge is really intersect
			if (sides[t4[i]] == sides[t4[i + 1]])
				continue;

			// Intersect
			int a = t4[0 + i], b = t4[1 + i];
			_temp[0] = CrossInterpolate(viewmodel * v4[a], viewmodel * v4[b]);

			// Push to destination
			dest->Render(_temp, 1);
		}
	}

	void CrossSection::InternalProject2(const Buffer4 &source, Visualizer4 *dest)
	{
		int *t4 = source.indices;
		Vector4 *v4 = source.vertices;
		// Loop over all point
		for (int i = 0; i < source.indiceCount; i += 3)
		{
			// Do initial check if this edge is really intersect
			if (IsEqual(sides[t4[i]], sides[t4[i + 1]], sides[t4[i + 2]]))
				continue;

			// Intersect
			int iter = 0, a, b;
			for (int j = 0; j < 3; j++)
			{
				if (sides[a = t4[_leftEdges[j] + i]] ^ sides[b = t4[_rightEdges[j] + i]])
				{
					_temp[iter++] = CrossInterpolate(viewmodel * v4[a], viewmodel * v4[b]);
				}
			}

			// Push to destination
			dest->Render(_temp, iter);
		}
	}

	void CrossSection::InternalProject3(const Buffer4 &source, Visualizer4 *dest)
	{
		int *t4 = source.indices;
		Vector4 *v4 = source.vertices;

		// Loop over all point
		for (int i = 0; i < source.indiceCount; i += 4)
		{
			// Do initial check if this edge is really intersect
			if (IsEqual(sides[t4[i]], sides[t4[i + 1]], sides[t4[i + 2]], sides[t4[i + 3]]))
				continue;

			// Intersect
			int iter = 0, a, b;
			for (int j = 0; j < 6; j++)
			{
				if (sides[a = t4[_leftEdges[j] + i]] ^ sides[b = t4[_rightEdges[j] + i]])
				{
					_temp[iter++] = CrossInterpolate(viewmodel * v4[a], viewmodel * v4[b]);
				}
			}

			// Push to destination
			dest->Render(_temp, iter);
		}
	}
	void CrossSection::Project(const Buffer4 &source, const Transform4 &transform, Visualizer4 *dest)
	{
		viewmodel = view * transform;

		EnsureCapacity(&sides, 0, &sides_cap, source.verticeCount);
		{
			// faster than "sides[i] = viewmodel * source.vertices[i] > 0"
			Vector4 &vmw = viewmodel.rotation.ew;
			float vmwp = viewmodel.position.w;
			for (int i = 0; i < source.verticeCount; ++i)
				sides[i] = (Dot(vmw, source.vertices[i]) < vmwp);
		}

		{
			switch (source.simplex)
			{
			case SimplexMode::SM_Line:
				InternalProject1(source, dest);
				break;
			case SimplexMode::SM_Triangle:
				InternalProject2(source, dest);
				break;
			case SimplexMode::SM_Tetrahedron:
				InternalProject3(source, dest);
				break;
			}
		}
	}
} // namespace Forth
