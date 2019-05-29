#pragma once

#include "../rendering/Visualizer4.h"

namespace Forth
{
	class SolidVisualizer : public Visualizer4
	{
	  public:
		SolidVisualizer(void);

		Buffer3 *buff;

		/// <summary>
		/// Marks the beginning of the visualizing function
		/// </summary>
		void Initialize(Buffer3 &buffer) override;

		/// <summary>
		/// Per-simplex visualize method
		/// Assume Vector4[] is Vector3[] with w = 0 each
		/// </summary>
		void Render(const Vector4 *buffer, int count) override;

		/// <summary>
		/// Marks the end of the visualizing function
		/// </summary>
		void End(void) override;

		/// <summary>
		/// Make normal consistent so we don't get trouble with shadows.
		/// </summary>
		void RefineTriangleOrder(void);

		Vector3 GetAverage(const std::vector<Vector3> &v);
	};

} // namespace Forth