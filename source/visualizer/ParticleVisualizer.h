#pragma once

#include "../rendering/Visualizer4.h"

namespace Forth
{
	class ParticleVisualizer : public Visualizer4
	{

	public:

		ParticleVisualizer(void) { }

		Buffer3 *buff;

		/// <summary>
		/// Marks the beginning of the visualizing function
		/// </summary>
		void Initialize(Buffer3 &buffer) override
		{
			buff = &buffer;
			buff->Clear();
			buff->simplex = SimplexMode::SM_Point;
		}

		/// <summary>
		/// Per-simplex visualize method
		/// Assume Vector4[] is Vector3[] with w = 0 each
		/// </summary>
		// Because it's a segment, count is expected to be 2
		void Render(const Vector4 *buffer, int count) override
		{
			buff->AddTris((int)buff->vertices_count);
			buff->AddVert(buffer[0].ToVec3());
		}

	};

}