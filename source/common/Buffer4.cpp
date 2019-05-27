
#include "Buffer4.h"

namespace Forth
{
	inline void Buffer4::EnsureIndices(const int incoming)
	{
		if (indiceCount + incoming > indiceCap)
		{
			Expand(&indices, indiceCount, indiceCap = Max(indiceCount + incoming, indiceCap << 1));
		}
	}
	inline void Buffer4::EnsureVertices(const int incoming)
	{
		if (verticeCount + incoming > verticeCap)
		{
			Expand(&vertices, verticeCount, verticeCap = Max(verticeCount + incoming, verticeCap << 1));
		}
	}

	inline void Buffer4::Clear()
	{
		verticeCount = indiceCount = offset = 0;
	}

	inline void Buffer4::Clean()
	{
		Clear();
		indices = new int[indiceCap = 4];
		vertices = new Vector4[verticeCap = 4];
	}

	inline Buffer4::Buffer4()
	{
		Clean();
	}

	inline void Buffer4::Align() { offset = verticeCount; }

	inline void Buffer4::Align(int snapshot) { offset = snapshot; }

	/// <summary>
	/// Send copy of current buffer position to be reused later.
	/// </summary>
	/// <seealso cref="Align(int)"/>

	inline int Buffer4::Snapshot() { return verticeCount; }

	inline void Buffer4::AddSimplex(int i)
	{
		EnsureIndices(1);
		indices[indiceCount++] = (i + offset);
	}

	inline void Buffer4::AddSimplex(int i, int j)
	{
		EnsureIndices(2);
		indices[indiceCount++] = (i + offset);
		indices[indiceCount++] = (j + offset);
	}

	inline void Buffer4::AddSimplex(int i, int j, int k)
	{
		EnsureIndices(3);
		indices[indiceCount++] = (i + offset);
		indices[indiceCount++] = (j + offset);
		indices[indiceCount++] = (k + offset);
	}

	inline void Buffer4::AddSimplex(int i, int j, int k, int l)
	{
		EnsureIndices(2);
		indices[indiceCount++] = (i + offset);
		indices[indiceCount++] = (j + offset);
		indices[indiceCount++] = (k + offset);
		indices[indiceCount++] = (l + offset);
	}

	inline void Buffer4::AddPoint(int v0)
	{
		switch (simplex)
		{
		case SM_Point:
			AddSimplex(v0);
			break;
		}
	}

	inline void Buffer4::AddSegment(int v0, int v1)
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

	inline void Buffer4::AddTriangle(int v0, int v1, int v2)
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

	inline void Buffer4::AddQuad(int v0, int v1, int v2, int v3)
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

	inline void Buffer4::AddTrimid(int v0, int v1, int v2, int v3)
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

	inline void Buffer4::AddPyramid(int v0, int v1, int v2, int v3, int v4)
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

	inline void Buffer4::AddPrism(int v0, int v1, int v2, int v3, int v4, int v5)
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

	inline void Buffer4::AddCube(int v0, int v1, int v2, int v3, int v4, int v5, int v6, int v7)
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

	inline int Buffer4::AddVertex(int idx)
	{
		EnsureVertices(1);
		vertices[verticeCount] = (vertices[idx + offset]);
		return verticeCount++ - offset;
	}

	inline int Buffer4::AddVertex(Vector4 vert)
	{
		EnsureVertices(1);
		vertices[verticeCount] = vert;
		return verticeCount++ - offset;
	}

	inline void Buffer4::Sequence(SequenceMode mode, int start, int count)
	{
		offset += start;
		int end = count < 0 ? verticeCount - offset : count;
		switch (mode)
		{
		case SQM_Points:
			for (int i = 0; i < end;)
				AddPoint(i++);
			break;

		case SQM_Lines:
			for (int i = 0; i < end;)
				AddSegment(i++, i++);
			break;

		case SQM_LineStrip:
			for (int i = 1; i < end;)
				AddSegment(i - 1, i++);
			break;

		case SQM_LineFan:
			for (int i = 1; i < end;)
				AddSegment(0, i++);
			break;

		case SQM_LineLoop:
			for (int i = 0; i < end;)
				AddSegment(i, (++i % end));
			break;

		case SQM_Triangles:
			for (int i = 0; i < end;)
				AddTriangle(i++, i++, i++);
			break;

		case SQM_TriangleStrip:
			for (int i = 2; i < end;)
				AddTriangle(i - 2, i - 1, i++);
			break;

		case SQM_TriangleFan:
			for (int i = 1; i < end;)
				AddTriangle(0, i - 1, i++);
			break;

		case SQM_Quads:
			for (int i = 0; i < end;)
				AddQuad(i++, i++, i++, i++);
			break;

		case SQM_QuadStrip:
			for (int i = 2; i < end; i += 2)
				AddQuad(i - 2, i - 1, i + 1, i);
			break;

		case SQM_Polygon:
			for (int i = 1; i < end;)
				AddTriangle(0, i, (++i % end));
			break;

		case SQM_Trimids:
			for (int i = 0; i < end;)
				AddTrimid(i++, i++, i++, i++);
			break;

		case SQM_TrimidStrip:
			for (int i = 3; i < end;)
				AddTrimid(i - 3, i - 2, i - 1, i++);
			break;

		case SQM_TrimidFan:
			for (int i = 1; i < end;)
				AddTrimid(0, i++, i++, i++);
			break;

		case SQM_PyramidFan:
			for (int i = 1; i < end;)
				AddPyramid(0, i++, i++, i++, i++);
			break;

		case SQM_PrismFan:
			for (int i = 2; i < end; i += 2)
				AddPrism(i++, i++, 0, i + 1, i, 1);
			break;

		case SQM_Cubes:
			for (int i = 0; i < end;)
				AddCube(i++, i++, i++, i++, i++, i++, i++, i++);
			break;

		case SQM_CubeStrip:
			for (int i = 4; i < end;)
				AddCube(i - 4, i - 3, i - 2, i - 1, i++, i++, i++, i++);
			break;
		}
		offset -= start;
	}

	inline int Buffer4::SequenceIndex(int i, int j, int k, int l)
	{
		return l + _seqW * (k + _seqZ * (j + _seqY * i));
	}

	template <typename T>
	inline void Buffer4::Expand(T **arr, int count, int newSize)
	{
		T *newArr = new T[newSize];

		memcpy(newArr, *arr, count * sizeof(T));

		delete[] * arr;
		*arr = newArr;
	}

	void Buffer4::SequenceGrid(int x, int y, int z, int w)
	{

		_seqY = y;
		_seqZ = z;
		_seqW = w; // _seqX = x;

		switch (simplex)
		{
		case SM_Point:
			for (int i = 0; i < x; i++)
				for (int j = 0; j < y; j++)
					for (int k = 0; k < z; k++)
						for (int l = 0; l < w; l++)
							AddPoint(SequenceIndex(i, j, k, l));

			break;
		case SM_Line:
			for (int i = 0; i < x; i++)
				for (int j = 0; j < y; j++)
					for (int k = 0; k < z; k++)
						for (int l = 0; l < w; l++)
						{
							int N = SequenceIndex(i, j, k, l);
							if ((i > 0))
								AddSegment(SequenceIndex(i - 1, j, k, l), N); // X edges
							if ((j > 0))
								AddSegment(SequenceIndex(i, j - 1, k, l), N); // Y edges
							if ((k > 0))
								AddSegment(SequenceIndex(i, j, k - 1, l), N); // Z edges
							if ((l > 0))
								AddSegment(SequenceIndex(i, j, k, l - 1), N); // W edges
						}
			break;
		case SM_Triangle:
			for (int i = 0; i < x; i++)
			{
				int I = SequenceIndex(i, 0, 0, 0), pI = SequenceIndex(i - 1, 0, 0, 0);
				for (int j = 0; j < y; j++)
				{
					int J = SequenceIndex(0, j, 0, 0), pJ = SequenceIndex(0, j - 1, 0, 0);
					for (int k = 0; k < z; k++)
					{
						int K = SequenceIndex(0, 0, k, 0), pK = SequenceIndex(0, 0, k - 1, 0);
						for (int l = 0; l < w; l++)
						{
							int L = SequenceIndex(0, 0, 0, l), pL = SequenceIndex(0, 0, 0, l - 1), N = I + J + K + L;

							if ((k > 0) & (j > 0))
								AddQuad(N, I + J + pK + L, I + pJ + pK + L, I + pJ + K + L); // ZY faces
							if ((i > 0) & (j > 0))
								AddQuad(N, pI + J + K + L, pI + pJ + K + L, I + pJ + K + L); // XY faces
							if ((i > 0) & (k > 0))
								AddQuad(N, I + J + pK + L, pI + J + pK + L, pI + J + K + L); // XZ faces
							if ((i > 0) & (l > 0))
								AddQuad(N, I + J + K + pL, pI + J + K + pL, pI + J + K + L); // WX faces
							if ((j > 0) & (l > 0))
								AddQuad(N, I + J + K + pL, I + pJ + K + pL, I + pJ + K + L); // WY faces
							if ((k > 0) & (l > 0))
								AddQuad(N, I + J + K + pL, I + J + pK + pL, I + J + pK + L); // WZ faces
						}
					}
				}
			}
			break;
		case SM_Tetrahedron:
			for (int i = 0; i < x; i++)
			{
				int I = SequenceIndex(i, 0, 0, 0), pI = SequenceIndex(i - 1, 0, 0, 0);
				for (int j = 0; j < y; j++)
				{
					int J = SequenceIndex(0, j, 0, 0), pJ = SequenceIndex(0, j - 1, 0, 0);
					for (int k = 0; k < z; k++)
					{
						int K = SequenceIndex(0, 0, k, 0), pK = SequenceIndex(0, 0, k - 1, 0);
						for (int l = 0; l < w; l++)
						{
							int L = SequenceIndex(0, 0, 0, l), pL = SequenceIndex(0, 0, 0, l - 1), N = I + J + K + L;

							if ((i > 0) & (j > 0) & (k > 0))
								AddCube(N, pI + J + K + L, pI + pJ + K + L, I + pJ + K + L, I + J + pK + L, pI + J + pK + L, pI + pJ + pK + L, I + pJ + pK + L); // XYZ cube
							if ((i > 0) & (j > 0) & (l > 0))
								AddCube(N, pI + J + K + L, pI + pJ + K + L, I + pJ + K + L, I + J + K + pL, pI + J + K + pL, pI + pJ + K + pL, I + pJ + K + pL); // XYW cube
							if ((i > 0) & (k > 0) & (l > 0))
								AddCube(N, pI + J + K + L, pI + J + pK + L, I + J + pK + L, I + J + K + pL, pI + J + K + pL, pI + J + pK + pL, I + J + pK + pL); // XZW cube
							if ((j > 0) & (k > 0) & (l > 0))
								AddCube(N, I + pJ + K + L, I + pJ + pK + L, I + J + pK + L, I + J + K + pL, I + pJ + K + pL, I + pJ + pK + pL, I + J + pK + pL); // YZW cube
						}
					}
				}
			}
			break;
		}
	}
} // namespace Forth