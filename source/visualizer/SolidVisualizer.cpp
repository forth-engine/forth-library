#include "SolidVisualizer.h"

namespace Forth {

	SolidVisualizer::SolidVisualizer() {}

	void SolidVisualizer::Initialize(Buffer3 & buffer)
	{
		buff = &buffer;
		buff->Clear();
		buff->simplex = SimplexMode::SM_Triangle;
	}

	void SolidVisualizer::Render(const Vector4 * buffer, int count)
	{
		Buffer3 &b = *buff;
		int o = (int)b.vertices.size();
		b.AddVert(buffer[0].ToVec3());
		b.AddVert(buffer[1].ToVec3());
		for (int i = 2; i < count; i++)
		{
			b.AddVert(buffer[i].ToVec3());
			b.AddTris(0 + o, i - 1 + o, i + o);
		}
	}

	void SolidVisualizer::End()
	{
		RefineTriangleOrder();
	}

	void SolidVisualizer::RefineTriangleOrder()
	{
		std::vector<int> &t = buff->indices;
		std::vector<Vector3> &v = buff->vertices;

		Vector3 median = GetAverage(v);

		for (size_t i = 0; i < t.size(); i += 3)
		{
			int a = t[i], b = t[i + 1], c = t[i + 2];
			Vector3 &va = v[a];
			Vector3 N = Cross(v[c] - va, v[b] - va);
			if (Dot(N, median) < Dot(N, va))
			{
				// Flip
				t[i] = c;
				t[i + 2] = a;
			}
		}
	}

	Vector3 SolidVisualizer::GetAverage(const std::vector<Vector3>& v)
	{
		Vector3 median = Vector3();
		size_t size = v.size();
		for (size_t i = size; i-- > 0;)
			median += v[i];
		return median / (float)size;
	}

}
