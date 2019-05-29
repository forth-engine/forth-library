#pragma once

#include "../common/Color.h"
#include "../math/Vector3.h"
#include "../extras/Utils.h"
#include "Buffer3.h"
#include "Enums.h"
#include <vector>

namespace Forth
{
	struct BufferGL
	{
		// Layout: 0-2 Position. 3-5 Normal
		FORTH_ARRAY(vb, float);

		BufferGL(void) {}

		~BufferGL(void)
		{
			delete[] vb;
		}

		void Clear(void)
		{
			vb_count = 0;
		}

		void Copy(const Buffer3 &v)
		{
			Clear();

			int len = v.indices_count;

			const int BUFFER_PER_VERTEX = 6;
			const int BUFFER_PER_TRIS = BUFFER_PER_VERTEX * 3;

			EnsureCapacity(&vb, 0, &vb_cap, len * BUFFER_PER_VERTEX);

			for (int i = 0; i < len;)
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

				for(int j = 0; j < BUFFER_PER_TRIS;)
				{
					vb[vb_count++] = arr[j++];
				}

			}
		}
	};
} // namespace Forth