#pragma once


#include "Enums.h"
#include "VertexProfile.h"
#include "../math/Vector4.h"

namespace Forth
{
	/// <summary>
	/// Temporary representation of 4D mesh
	/// </summary>
	struct Buffer4
	{
		Buffer4() { }

		Vector4 *vertices;
		int *indices;
		VertexProfile *profiles;

		int verticeCount, indiceCount, profileCount;
		int verticeCap, indiceCap, profileCap;

		SimplexMode simplex;

		void EnsureIndices(int incoming)
        {

            if (indiceCount + incoming > indiceCap)
			{
				int newSize = Max(indiceCount + incoming, indiceCap << 1);
				int* newArr = new int[newSize];

				memcpy( newArr, indices, indiceCount * sizeof(int) );

				indiceCap = newSize;
				delete [] indices;
				indices = newArr;
			}
        }
	};
}