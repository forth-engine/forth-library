
#include "Buffer4.h"

namespace Forth
{
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
}