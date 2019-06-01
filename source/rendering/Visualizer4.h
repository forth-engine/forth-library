#pragma once

#include "../common/Buffer3.h"
#include "../common/Buffer4.h"

namespace Forth
{
	class Visualizer4
	{

	  public:
		Visualizer4(void) {}

		/// <summary>
		/// Marks the beginning of the visualizing function
		/// </summary>
		virtual void Initialize(Buffer3 &buffer) = 0;

		/// <summary>
		/// Per-simplex visualize method
		/// Assume Vector4[] is Vector3[] with w = 0 each
		/// </summary>
		virtual void Render(const Vector4 buffer[], int count) = 0;

		/// <summary>
		/// Marks the end of the visualizing function
		/// </summary>
		virtual void End(void){
			// 🤔?
		};
	};
} // namespace Forth