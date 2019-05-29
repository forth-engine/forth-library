#pragma once

#include "../math/Vector3.h"
#include "../common/Color.h"
#include "../extras/Utils.h"
#include "VertexProfile.h"
#include "Enums.h"
#include <vector>

namespace Forth
{
	struct Buffer3
	{
		FORTH_ARRAY(vertices, Vector3);
		FORTH_ARRAY(indices, int);
		Forth::SimplexMode simplex;

		Buffer3() { simplex = SM_Triangle; }

		~Buffer3(void)
		{
			delete[] vertices;
			delete[] indices;
		}

		void Clear(void)
		{
			vertices_count = 0;
			indices_count = 0;
		}

		void AddVert(const Vector3 &v)
		{
			EnsureCapacity(&vertices, vertices_count, &vertices_cap, vertices_count + 1);
			vertices[vertices_count++] = v;
		}

		void AddTris(const int a)
		{
			EnsureCapacity(&indices, indices_count, &indices_cap, indices_count + 1);
			indices[indices_count++] = a;
		}

		void AddTris(const int a, const int b)
		{
			EnsureCapacity(&indices, indices_count, &indices_cap, indices_count + 2);
			indices[indices_count++] = a;
			indices[indices_count++] = b;
		}

		void AddTris(const int a, const int b, const int c)
		{
			EnsureCapacity(&indices, indices_count, &indices_cap, indices_count + 3);
			indices[indices_count++] = a;
			indices[indices_count++] = b;
			indices[indices_count++] = c;
		}

	};
}