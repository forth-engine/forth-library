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
	void Expand(T **arr, int count, int newSize);

	void EnsureIndices(const int incoming);

	void EnsureVertices(const int incoming);

	bool IsEmpty() { return verticeCount == 0; }

	void Clear();

	///
	/// Clear and Clean memory traces.
	///
	void Clean();

	Buffer4();

	/// <summary>
	/// Move buffer forward to the end.
	/// </summary>
	/// <remarks>
	/// Align are handy if used together with bulk operations (e.g. <see cref="Buffer4Extension.Sequence(Buffer4, SequenceMode)"/>)
	/// </remarks>
	void Align();

	/// <summary>
	/// Move buffer toward the given snapshot.
	/// </summary>
	/// <remarks>
	/// Use <see cref="Snapshot"/> to obtain snapshot at given position.
	/// </remarks>
	/// <seealso cref="Snapshot"/>
	void Align(int snapshot);

	/// <summary>
	/// Send copy of current buffer position to be reused later.
	/// </summary>
	/// <seealso cref="Align(int)"/>
	int Snapshot();

	void AddSimplex(int i);

	void AddSimplex(int i, int j);

	void AddSimplex(int i, int j, int k);

	void AddSimplex(int i, int j, int k, int l);

	/// <summary>
	/// Add an artbitrary point.
	/// </summary>
	void AddPoint(int v0);

	/// <summary>
	/// Add a segment. Order doesn't matter.
	/// </summary>
	void AddSegment(int v0, int v1);

	/// <summary>
	/// Add a flat triangle. Order doesn't matter.
	/// </summary>
	void AddTriangle(int v0, int v1, int v2);

	/// <summary>
	/// Add a flat quad. Must be either clockwise/counter-clockwise order.
	/// </summary>
	void AddQuad(int v0, int v1, int v2, int v3);

	/// <summary>
	/// Add a trimid (triangle pyramid) with given vert indexs. Order doesn't matter.
	/// </summary>
	void AddTrimid(int v0, int v1, int v2, int v3);

	/// <summary>
	/// Helper to add a pyramid from 5 existing verts index.
	/// A pyramid is constructed from 2 trimids, v0 is the tip, the rest is the base.
	/// </summary>
	void AddPyramid(int v0, int v1, int v2, int v3, int v4);

	/// <summary>
	/// Helper to add a triangular prism from 6 existing verts index.
	/// A prism is constructed from 3 trimids, v0-v1-v2 must be parallel (and in the same order) with v3-v4-v5
	/// </summary>
	void AddPrism(int v0, int v1, int v2, int v3, int v4, int v5);

	/// <summary>
	/// Helper to add a cube from 8 existing verts index.
	/// A cube is constructed from 5 trimids, v0-v1-v2-v3 must be parallel (and in the same order) with v4-v5-v6-v7
	/// </summary>
	void AddCube(int v0, int v1, int v2, int v3, int v4, int v5, int v6, int v7);

	/// <summary> Duplicate vertex and return the cloned index </summary>
	/// <remarks> Useful for sequencing operations </remarks>
	int AddVertex(int idx);

	/// <summary> Add a vertex with default profile and return the index </summary>
	int AddVertex(Vector4 vert);

	//----------------------------------------------------------------------

	/// <summary>
	/// Automatically add vertices to indices buffer
	/// since last Align() using given sequencing preset.
	/// </summary>
	void Sequence(SequenceMode mode, int start = 0, int count = -1);

	int _seqW, _seqZ, _seqY; // _seqX,

	int SequenceIndex(int i, int j, int k, int l);

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