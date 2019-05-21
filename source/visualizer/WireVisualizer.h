#pragma once

#include "../rendering/Visualizer4.h"

namespace Forth
{
	class WireVisualizer : public Visualizer4
	{
	public:
		WireVisualizer() { }

		Buffer3 *buff;

		/// <summary>
		/// Marks the beginning of the visualizing function
		/// </summary>
		void Initialize(Buffer3 &buffer) override
		{
			buff = &buffer;
			buff->Clear();
			buff->simplex = SimplexMode::SM_Line;
		}

		/// <summary>
		/// Per-simplex visualize method
		/// Assume Vector4[] is Vector3[] with w = 0 each
		/// </summary>
		// Because it's a segment, count is expected to be 2
		void Render(const Vector4 *buffer, int count) override
		{
			for (int i = 0; i < count; i++)
			{
				buff->AddTris((int)buff->vertices.size());
				buff->AddVert(buffer[i].ToVec3());
			}
		}
	};

}