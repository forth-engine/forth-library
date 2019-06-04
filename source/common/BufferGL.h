#pragma once

#include "../common/Color.h"
#include "../extras/Utils.h"
#include "../math/Vector3.h"
#include "Buffer3.h"
#include "Enums.h"
#include <vector>

namespace Forth
{
	struct BufferGL
	{
		// Layout: 0-2 Position. 3-5 Normal
		FORTH_ARRAY(vb, float);

		struct
		{
			int counts[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			int slots = 0;
			int stripe = 0;
			int simplex = 0;
			int vertexs = 0;
		} attr;

		struct
		{
			bool normal = true;
		} generate;

		BufferGL(void) {}

		~BufferGL(void)
		{
			delete[] vb;
		}

		void Clear(void)
		{
			vb_count = 0;
		}

		void FillVertices(const Buffer3 &v, int offset)
		{
			for (int i = 0; i < attr.vertexs; ++i)
			{
				auto &f = v.vertices[v.indices[i]];
				vb[i * attr.stripe + offset + 0] = f.x;
				vb[i * attr.stripe + offset + 1] = f.y;
				vb[i * attr.stripe + offset + 2] = f.z;
			}
		}

		void FillNormals(const Buffer3 &v, int offset)
		{
			for (int i = 0; i < attr.vertexs;i += 3)
			{
				const int a = v.indices[i + 0],
						  b = v.indices[i + 1],
						  c = v.indices[i + 2];

				const Vector3 &av = v.vertices[a],
							  &bv = v.vertices[b],
							  &cv = v.vertices[c],
							  nv = Forth::Normalize(Forth::Cross(bv - cv, av - cv));

				for (int j = 0; j < 3;++j)
				{
					vb[(i + j) * attr.stripe + offset + 0] = nv.x;
					vb[(i + j) * attr.stripe + offset + 1] = nv.y;
					vb[(i + j) * attr.stripe + offset + 2] = nv.z;
				}
			}
		}

		void Copy(const Buffer3 &v)
		{
			Clear();

			attr.vertexs = v.indices_count;
			attr.simplex = v.simplex + 1;
			attr.stripe = 3;
			attr.slots = 1;
			attr.counts[0] = 3; // First: Vertice Positions

			bool genNormal = v.simplex == SM_Triangle && generate.normal;
			if (genNormal)
			{
				attr.counts[1] = 3; // Second: Normal
				attr.slots++;
				attr.stripe += 3;
			}

			EnsureCapacity(&vb, 0, &vb_cap, vb_count = attr.vertexs * attr.stripe);

			FillVertices(v, 0);
			if (genNormal)
				FillNormals(v, 3);
		}
	};
} // namespace Forth