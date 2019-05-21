#pragma once

#include "../rendering/Visualizer4.h"

namespace Forth
{
	class SolidVisualizer : public Visualizer4
	{
	public:
		SolidVisualizer() { }

		Buffer3 *buff;

		/// <summary>
		/// Marks the beginning of the visualizing function
		/// </summary>
		void Initialize(Buffer3 &buffer) override
		{
			buff = &buffer;
			buff->Clear();
			buff->simplex = SimplexMode::SM_Triangle;
		}

		/// <summary>
		/// Per-simplex visualize method
		/// Assume Vector4[] is Vector3[] with w = 0 each
		/// </summary>
		void Render(const Vector4 *buffer, int count) override
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

		/// <summary>
		/// Marks the end of the visualizing function
		/// </summary>
		void End() override
		{
			RefineTriangleOrder();
		};

		/// <summary>
		/// Make normal consistent so we don't get trouble with shadows.
		/// </summary>
		void RefineTriangleOrder()
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

		Vector3 GetAverage(const std::vector<Vector3> &v)
		{
			Vector3 median;
			size_t size = v.size();
			for (size_t i = size; i-- > 0;)
				median += v[i];
			return median / (float)size;
		}

	};

}