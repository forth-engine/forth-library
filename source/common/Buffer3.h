#pragma once

#include "../math/Vector3.h"
#include "../common/Color.h"
#include "VertexProfile.h"
#include "Enums.h"
#include <vector>

namespace Forth
{
	struct Buffer3
	{
		std::vector<Vector3> vertices;
		std::vector<int> indices;
		std::vector<VertexProfile> profiles;

		Buffer3() { }

		~Buffer3()
		{
			Clear();
		}

		void Clear()
		{
			vertices.clear();
			indices.clear();
			profiles.clear();
		}

		void AddVert(const Vector3 &v)
		{
			vertices.push_back(v);
		}


		void AddTris(const int a)
		{
			indices.push_back(a);
		}

		void AddTris(const int a, const int b)
		{
			indices.push_back(a);
			indices.push_back(b);
		}

		void AddTris(const int a, const int b, const int c)
		{
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
		}

		void AddProfile(const VertexProfile &p)
		{
			profiles.push_back(p);
		}

		Forth::SimplexMode simplex;
	};
}