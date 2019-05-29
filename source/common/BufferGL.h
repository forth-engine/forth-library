#pragma once

#include "../common/Color.h"
#include "../math/Vector3.h"
#include "Buffer3.h"
#include "Enums.h"
#include <vector>

namespace Forth
{
	struct BufferGL
	{
		// Layout: 0-2 Position. 3-5 Normal
		std::vector<float> vb;

		BufferGL(void) {}

		~BufferGL(void)
		{
			Clear();
		}

		void Clear(void)
		{
			vb.clear();
		}

		void Copy(const Buffer3 &v)
		{
			Clear();
			size_t len = v.indices.size();

			for (size_t i = 0; i < len;)
			{
				const int a = v.indices[i++],
						  b = v.indices[i++],
						  c = v.indices[i++];

				const Vector3 av = v.vertices[a],
							  bv = v.vertices[b],
							  cv = v.vertices[c],
							  nv = Forth::Normalize(Forth::Cross(bv - cv, av - cv));

				// clang-format off
				float arr[] = {
					av.x, av.y, av.z, nv.x, nv.y, nv.z,
					bv.x, bv.y, bv.z, nv.x, nv.y, nv.z,
					cv.x, cv.y, cv.z, nv.x, nv.y, nv.z,
				};
				// clang-format on

				vb.insert(vb.end(), arr, arr + 18);
			}
		}
	};
} // namespace Forth