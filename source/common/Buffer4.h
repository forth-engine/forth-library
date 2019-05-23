#pragma once

#include <cstring>
#include "Enums.h"
#include "VertexProfile.h"
#include "../math/Vector4.h"

namespace Forth
{
///
/// Temporary representation of 4D mesh
///
struct Buffer4
{


	Vector4 *vertices;
	int *indices;

	int verticeCount, indiceCount;
	int verticeCap, indiceCap;
	int offset = 0;

	SimplexMode simplex;

	template <typename T>
	void Expand(T **arr, int count, int newSize)
	{
		T *newArr = new T[newSize];

		memcpy(newArr, *arr, count * sizeof(T));

		delete[] *arr;
		*arr = newArr;
	}

	void EnsureIndices(const int incoming)
	{
		if (indiceCount + incoming > indiceCap)
		{
			Expand(&indices, indiceCount, indiceCap = Max(indiceCount + incoming, indiceCap << 1));
		}
	}

	void EnsureVertices(const int incoming)
	{
		if (verticeCount + incoming > verticeCap)
		{
			Expand(&vertices, verticeCount, verticeCap = Max(verticeCount + incoming, verticeCap << 1));
		}
	}

	bool IsEmpty() { return verticeCount == 0; }

	void Clear()
	{
		verticeCount = indiceCount = offset = 0;
	}

	///
	/// Clear and Clean memory traces.
	///
	void Clean()
	{
		Clear();
		indices = new int[indiceCap = 4];
		vertices = new Vector4[verticeCap = 4];
	}

	Buffer4() {
		Clean();
	}

	/// <summary>
	/// Move buffer forward to the end.
	/// </summary>
	/// <remarks>
	/// Align are handy if used together with bulk operations (e.g. <see cref="Buffer4Extension.Sequence(Buffer4, SequenceMode)"/>)
	/// </remarks>
	void Align() { offset = verticeCount; }

	/// <summary>
	/// Move buffer toward the given snapshot.
	/// </summary>
	/// <remarks>
	/// Use <see cref="Snapshot"/> to obtain snapshot at given position.
	/// </remarks>
	/// <seealso cref="Snapshot"/>
	void Align(int snapshot) { offset = snapshot; }

	/// <summary>
	/// Send copy of current buffer position to be reused later.
	/// </summary>
	/// <seealso cref="Align(int)"/>
	int Snapshot() { return verticeCount; }

	void AddSimplex(int i)
	{
		EnsureIndices(1);
		indices[indiceCount++] = (i + offset);
	}

	void AddSimplex(int i, int j)
	{
		EnsureIndices(2);
		indices[indiceCount++] = (i + offset);
		indices[indiceCount++] = (j + offset);
	}

	void AddSimplex(int i, int j, int k)
	{
		EnsureIndices(3);
		indices[indiceCount++] = (i + offset);
		indices[indiceCount++] = (j + offset);
		indices[indiceCount++] = (k + offset);
	}

	void AddSimplex(int i, int j, int k, int l)
	{
		EnsureIndices(2);
		indices[indiceCount++] = (i + offset);
		indices[indiceCount++] = (j + offset);
		indices[indiceCount++] = (k + offset);
		indices[indiceCount++] = (l + offset);
	}

	/// <summary>
	/// Add an artbitrary point.
	/// </summary>
	void AddPoint(int v0)
	{
		switch (simplex)
		{
		case SM_Point:
			AddSimplex(v0);
			break;
		}
	}

	/// <summary>
	/// Add a segment. Order doesn't matter.
	/// </summary>
	void AddSegment(int v0, int v1)
	{
		switch (simplex)
		{
		case SM_Point:
			AddSimplex(v0, v1);
			break;
		case SM_Line:
			AddSimplex(v0, v1);
			break;
		}
	}

	/// <summary>
	/// Add a flat triangle. Order doesn't matter.
	/// </summary>
	void AddTriangle(int v0, int v1, int v2)
	{
		switch (simplex)
		{
		case SM_Point:
			AddSimplex(v0, v1, v2);
			break;
		case SM_Line:
			AddSimplex(v0, v1);
			AddSimplex(v1, v2);
			AddSimplex(v2, v0);
			break;
		case SM_Triangle:
			AddSimplex(v0, v1, v2);
			break;
		}
	}

	/// <summary>
	/// Add a flat quad. Must be either clockwise/counter-clockwise order.
	/// </summary>
	void AddQuad(int v0, int v1, int v2, int v3)
	{
		switch (simplex)
		{
		case SM_Point:
			AddSimplex(v0, v1, v2, v3);
			break;
		case SM_Line:
			AddSimplex(v0, v1);
			AddSimplex(v1, v2);
			AddSimplex(v2, v3);
			AddSimplex(v3, v0);
			break;
		case SM_Triangle:
			AddSimplex(v0, v1, v2);
			AddSimplex(v2, v3, v0);
			break;
		}
	}

	/// <summary>
	/// Add a trimid (triangle pyramid) with given vert indexs. Order doesn't matter.
	/// </summary>
	void AddTrimid(int v0, int v1, int v2, int v3)
	{
		switch (simplex)
		{
		case SM_Point: // 4 vertexes
			AddSimplex(v0, v1, v2, v3);
			break;

		case SM_Line: // 6 edges
			AddSimplex(v0, v1);
			AddSimplex(v0, v2);
			AddSimplex(v0, v3);
			AddTriangle(v1, v2, v3);

			break;
		case SM_Triangle: // 4 faces
			AddTriangle(v0, v1, v2);
			AddTriangle(v0, v2, v3);
			AddTriangle(v0, v3, v1);
			AddTriangle(v1, v2, v3);
			break;

		case SM_Tetrahedron: // 1 cell
			AddSimplex(v0, v1, v2, v3);
			break;
		}
	}

	/// <summary>
	/// Helper to add a pyramid from 5 existing verts index.
	/// A pyramid is constructed from 2 trimids, v0 is the tip, the rest is the base.
	/// </summary>
	void AddPyramid(int v0, int v1, int v2, int v3, int v4)
	{
		switch (simplex)
		{
		case SM_Point: // 5 vertexes
			AddSimplex(v0, v1, v2);
			AddSimplex(v3, v4);
			break;

		case SM_Line: // 8 edges
			AddSimplex(v0, v1);
			AddSimplex(v0, v2);
			AddSimplex(v0, v3);
			AddSimplex(v0, v4);
			AddQuad(v1, v2, v3, v4);
			break;
		case SM_Triangle: // 6 faces (3 quads)
			AddTriangle(v0, v1, v2);
			AddTriangle(v0, v2, v3);
			AddTriangle(v0, v3, v4);
			AddTriangle(v0, v4, v1);
			AddQuad(v1, v2, v3, v4);
			break;

		case SM_Tetrahedron: // 2 cell
			AddSimplex(v0, v1, v2, v3);
			AddSimplex(v0, v1, v3, v4);
			break;
		}
	}

	/// <summary>
	/// Helper to add a triangular prism from 6 existing verts index.
	/// A prism is constructed from 3 trimids, v0-v1-v2 must be parallel (and in the same order) with v3-v4-v5
	/// </summary>
	void AddPrism(int v0, int v1, int v2, int v3, int v4, int v5)
	{
		switch (simplex)
		{
		case SM_Point: // 6 vertexes
			AddSimplex(v0, v1, v2);
			AddSimplex(v3, v4, v5);
			break;

		case SM_Line: // 10 edges
			AddTriangle(v0, v1, v2);
			AddTriangle(v3, v4, v5);

			AddSimplex(v0, v3);
			AddSimplex(v1, v4);
			AddSimplex(v2, v5);
			break;
		case SM_Triangle: // 5 faces
			AddTriangle(v0, v1, v2);
			AddTriangle(v3, v4, v5);

			AddQuad(v0, v1, v4, v3);
			AddQuad(v1, v2, v5, v4);
			AddQuad(v2, v1, v3, v5);
			break;

		case SM_Tetrahedron: // 3 cells
			AddSimplex(v0, v1, v2, v3);
			AddSimplex(v3, v4, v5, v2);
			AddSimplex(v1, v4, v3, v2);
			break;
		}
	}

	/// <summary>
	/// Helper to add a cube from 8 existing verts index.
	/// A cube is constructed from 5 trimids, v0-v1-v2-v3 must be parallel (and in the same order) with v4-v5-v6-v7
	/// </summary>
	void AddCube(int v0, int v1, int v2, int v3, int v4, int v5, int v6, int v7)
	{
		switch (simplex)
		{
		case SM_Point: // 8 vertices
			AddSimplex(v0, v1, v2, v3);
			AddSimplex(v4, v5, v6, v7);
			break;

		case SM_Line: // 12 edges
			AddQuad(v0, v1, v2, v3);
			AddQuad(v4, v5, v6, v7);

			AddSimplex(v0, v4);
			AddSimplex(v1, v5);
			AddSimplex(v2, v6);
			AddSimplex(v3, v7);
			break;

		case SM_Triangle: //  12 faces (6 quads)
			// topdown
			AddQuad(v0, v1, v2, v3);
			AddQuad(v4, v5, v6, v7);
			// leftright
			AddQuad(v0, v1, v5, v4);
			AddQuad(v2, v3, v7, v6);
			// frontback
			AddQuad(v1, v2, v6, v5);
			AddQuad(v0, v3, v7, v4);
			break;

		case SM_Tetrahedron: // 5 cells
			AddSimplex(v0, v1, v2, v5);
			AddSimplex(v0, v3, v2, v7);
			AddSimplex(v0, v4, v5, v7);
			AddSimplex(v2, v5, v7, v6);
			AddSimplex(v0, v2, v5, v7);
			break;
		}
	}

	/// <summary> Duplicate vertex and return the cloned index </summary>
	/// <remarks> Useful for sequencing operations </remarks>
	int AddVertex(int idx)
	{
		EnsureVertices(1);
		vertices[verticeCount] = (vertices[idx + offset]);
		return verticeCount++ - offset;
	}

	/// <summary> Add a vertex with default profile and return the index </summary>
	int AddVertex(Vector4 vert)
	{
		EnsureVertices(1);
		vertices[verticeCount] = vert;
		return verticeCount++ - offset;
	}



	//----------------------------------------------------------------------


	  /// <summary>
        /// Automatically add vertices to indices buffer
        /// since last Align() using given sequencing preset.
        /// </summary>
        void Sequence(SequenceMode mode, int start = 0, int count = -1)
        {
            offset += start;
            int end = count < 0 ? verticeCount - offset : count;
            switch (mode)
            {
                case SQM_Points:
                    for (int i = 0; i < end;) AddPoint(i++);
                    break;

                case SQM_Lines:
                    for (int i = 0; i < end;) AddSegment(i++, i++);
                    break;

                case SQM_LineStrip:
                    for (int i = 1; i < end;) AddSegment(i - 1, i++);
                    break;

                case SQM_LineFan:
                    for (int i = 1; i < end;) AddSegment(0, i++);
                    break;

                case SQM_LineLoop:
                    for (int i = 0; i < end;) AddSegment(i, (++i % end));
                    break;

                case SQM_Triangles:
                    for (int i = 0; i < end;) AddTriangle(i++, i++, i++);
                    break;

                case SQM_TriangleStrip:
                    for (int i = 2; i < end;) AddTriangle(i - 2, i - 1, i++);
                    break;

                case SQM_TriangleFan:
                    for (int i = 1; i < end;) AddTriangle(0, i - 1, i++);
                    break;

                case SQM_Quads:
                    for (int i = 0; i < end;) AddQuad(i++, i++, i++, i++);
                    break;

                case SQM_QuadStrip:
                    for (int i = 2; i < end; i += 2) AddQuad(i - 2, i - 1, i + 1, i);
                    break;

                case SQM_Polygon:
                    for (int i = 1; i < end;) AddTriangle(0, i, (++i % end));
                    break;

                case SQM_Trimids:
                    for (int i = 0; i < end;) AddTrimid(i++, i++, i++, i++);
                    break;

                case SQM_TrimidStrip:
                    for (int i = 3; i < end;) AddTrimid(i - 3, i - 2, i - 1, i++);
                    break;

                case SQM_TrimidFan:
                    for (int i = 1; i < end;) AddTrimid(0, i++, i++, i++);
                    break;

                case SQM_PyramidFan:
                    for (int i = 1; i < end;) AddPyramid(0, i++, i++, i++, i++);
                    break;

                case SQM_PrismFan:
                    for (int i = 2; i < end; i += 2) AddPrism(i++, i++, 0, i + 1, i, 1);
                    break;

                case SQM_Cubes:
                    for (int i = 0  ; i < end;) AddCube(i++, i++, i++, i++, i++, i++, i++, i++);
                    break;

                case SQM_CubeStrip:
                    for (int i = 4; i < end;) AddCube(i - 4, i - 3, i - 2, i - 1, i++, i++, i++, i++);
                    break;

            }
            offset -= start;
        }


        int _seqW, _seqZ, _seqY;// _seqX,

        int SequenceIndex(int i, int j, int k, int l)
        {
            return l + _seqW * (k + _seqZ * (j + _seqY * i));
        }

        /// <summary>
        /// Special sequencing tool when dealing with 1D, 2D, 3D or 4D grid vertices.
        /// </summary>
        /// <remarks>
        /// Set the grid count in the parameter.
        /// The nested order is for(x) => for(y) => for(z) => for(w).
        /// the starting point is always from the last Align()
        /// </remarks>
		void SequenceGrid(int x, int y = 1, int z = 1, int w = 1);

};
} // namespace Forth