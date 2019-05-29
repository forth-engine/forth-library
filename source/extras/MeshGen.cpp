#include "MeshGen.h"
#include "../math/Vector4.h"
#include <cmath>

namespace Forth
{
	const float MeshGen::_GR = 1.6180339887498948482045868343656f;
	const float MeshGen::_SQRT3 = 1.7320508075688772935274463415059f;
	const float MeshGen::_SQRT5 = 2.2360679774997896964091736687313f;
	const float MeshGen::_SQRT6 = 2.4494897427831780981972840747059f;
	const float MeshGen::_SQRT10 = 3.1622776601683793319988935444327f;

	const Vector4 MeshGen::_pentatopeVertices[5] = {
		Vector4(1 / _SQRT6, -1 / _SQRT10, 1 / _SQRT3, -1),
		Vector4(1 / _SQRT6, -1 / _SQRT10, 1 / _SQRT3, 1),
		Vector4(1 / _SQRT6, -1 / _SQRT10, -2 / _SQRT3, 0),
		Vector4(-std::sqrt(3.f / 2.f), -1 / _SQRT10, 0, 0),
		Vector4(0, 2 * std::sqrt(2.f / 5.f), 0, 0),
	};

	const Vector4 MeshGen::_cell120Vertices[6] = {
		Vector4(2, 2, 0, 0),
		Vector4(2, 0, 2, 0),
		Vector4(2, 0, 0, 2),
		Vector4(0, 2, 2, 0),
		Vector4(0, 2, 0, 2),
		Vector4(0, 0, 2, 2),
	};

	const float MeshGen::_cell120Vertices2[3][2] = {
		{_SQRT5, 1},
		{1 / (_GR * _GR), _GR},
		{_GR * _GR, 1 / _GR},
	};

	const Vector4 MeshGen::_cell120Vertices3[24] = {
		Vector4(_GR * _GR, 1, 1 / (_GR * _GR), 0),
		Vector4(_GR *_GR, 1 / (_GR * _GR), 0, 1),
		Vector4(_GR *_GR, 0, 1, 1 / (_GR * _GR)),
		Vector4(1, _GR *_GR, 0, 1 / (_GR * _GR)),
		Vector4(1, 1 / (_GR * _GR), _GR *_GR, 0),
		Vector4(1, 0, 1 / (_GR * _GR), _GR *_GR),
		Vector4(1 / (_GR * _GR), _GR *_GR, 1, 0),
		Vector4(1 / (_GR * _GR), 1, 0, _GR *_GR),
		Vector4(1 / (_GR * _GR), 0, _GR *_GR, 1),
		Vector4(0, _GR *_GR, 1 / (_GR * _GR), 1),
		Vector4(0, 1, _GR *_GR, 1 / (_GR * _GR)),
		Vector4(0, 1 / (_GR * _GR), 1, _GR *_GR),
		Vector4(_SQRT5, _GR, 1 / _GR, 0),
		Vector4(_SQRT5, 1 / _GR, 0, _GR),
		Vector4(_SQRT5, 0, _GR, 1 / _GR),
		Vector4(_GR, _SQRT5, 0, 1 / _GR),
		Vector4(_GR, 1 / _GR, _SQRT5, 0),
		Vector4(_GR, 0, 1 / _GR, _SQRT5),
		Vector4(1 / _GR, _SQRT5, _GR, 0),
		Vector4(1 / _GR, _GR, 0, _SQRT5),
		Vector4(1 / _GR, 0, _SQRT5, _GR),
		Vector4(0, _SQRT5, 1 / _GR, _GR),
		Vector4(0, _GR, _SQRT5, 1 / _GR),
		Vector4(0, 1 / _GR, _GR, _SQRT5),
	};

	const Vector4 MeshGen::_cell120Vertices4[12] = {
		Vector4(2, _GR, 1, 1 / _GR),
		Vector4(2, 1, 1 / _GR, _GR),
		Vector4(2, 1 / _GR, _GR, 1),
		Vector4(_GR, 2, 1 / _GR, 1),
		Vector4(_GR, 1, 2, 1 / _GR),
		Vector4(_GR, 1 / _GR, 1, 2),
		Vector4(1, 2, _GR, 1 / _GR),
		Vector4(1, _GR, 1 / _GR, 2),
		Vector4(1, 1 / _GR, 2, _GR),
		Vector4(1 / _GR, 2, 1, _GR),
		Vector4(1 / _GR, _GR, 2, 1),
		Vector4(1 / _GR, 1, _GR, 2),
	};

	const Vector4 MeshGen::_hexacosidedroidVertices[12] = {
		Vector4(_GR, 1, 1 / _GR, 0),
		Vector4(_GR, 1 / _GR, 0, 1),
		Vector4(_GR, 0, 1, 1 / _GR),
		Vector4(1, _GR, 0, 1 / _GR),
		Vector4(1, 1 / _GR, _GR, 0),
		Vector4(1, 0, 1 / _GR, _GR),
		Vector4(1 / _GR, _GR, 1, 0),
		Vector4(1 / _GR, 1, 0, _GR),
		Vector4(1 / _GR, 0, _GR, 1),
		Vector4(0, _GR, 1 / _GR, 1),
		Vector4(0, 1, _GR, 1 / _GR),
		Vector4(0, 1 / _GR, 1, _GR),
	};

	void MeshGen::MakeHyperplane(Buffer4 &input, float scale)
	{
		// Permutation of (-+1, 0, -+1, -+1)
		for (float w = -1; w <= 1; w += 2)
			for (float x = -1; x <= 1; x += 2)
				for (float z = -1; z <= 1; z += 2)
				{
					Vector4 v = Vector4(x, 0, z, w);
					input.AddVertex(v * scale);
				}

		// This is 3D grid, so can be done with .SequenceGrid
		input.SequenceGrid(2, 2, 2);
	}

	void MeshGen::MakePentatope(Buffer4 &input, float scale)
	{
		// Vertices according to the wiki...
		for (int a = 0; a < 5; a++)
			input.AddVertex(_pentatopeVertices[a] * scale);

		// Notice the pattern...
		input.AddTrimid(1, 2, 3, 4);
		input.AddTrimid(0, 2, 3, 4);
		input.AddTrimid(0, 1, 3, 4);
		input.AddTrimid(0, 1, 2, 4);
		input.AddTrimid(0, 1, 2, 3);
	}

	/// <summary>
	/// Make tesseract.
	/// </summary>

	void MeshGen::MakeTesseract(Buffer4 &input, float scale)
	{
		// Permutation of (-+1, -+1, -+1, -+1)
		// Idk why the order have to be like this?
		for (float w = -1; w <= 1; w += 2)
			for (float x = -1; x <= 1; x += 2)
				for (float y = -1; y <= 1; y += 2)
					for (float z = -1; z <= 1; z += 2)
					{
						Vector4 v = Vector4(x, y, z, w);
						input.AddVertex(v * scale);
					}

		input.SequenceGrid(2, 2, 2, 2);
	}

	void MeshGen::MakeHexdecahedroid(Buffer4 &input, float scale)
	{
		for (int a = 0; a < 4; a += 1)
			for (float b = -1; b <= 1; b += 2)
			{
				Vector4 v = Vector4(a, b);
				input.AddVertex(v * scale);
			}

		input.AddTrimid(0, 2, 4, 6);
		input.AddTrimid(1, 3, 5, 7);

		input.AddTrimid(1, 2, 4, 6);
		input.AddTrimid(0, 3, 4, 6);
		input.AddTrimid(0, 2, 5, 6);
		input.AddTrimid(0, 2, 4, 7);

		input.AddTrimid(0, 3, 5, 7);
		input.AddTrimid(1, 2, 5, 7);
		input.AddTrimid(1, 3, 4, 7);
		input.AddTrimid(1, 3, 5, 6);

		input.AddTrimid(1, 3, 4, 6);
		input.AddTrimid(1, 2, 5, 6);
		input.AddTrimid(1, 2, 4, 7);
		input.AddTrimid(0, 3, 5, 6);
		input.AddTrimid(0, 3, 4, 7);
		input.AddTrimid(0, 2, 5, 7);
	}

	// Dual pyramid spec: a & b is the pole, the rest is the meridian

	void MeshGen::AddDualPyramid(Buffer4 &input, int a, int b, int c, int d, int e, int f)
	{
		switch (input.simplex)
		{
		case SM_Point:
			input.AddPoints(a, b, c, d, e, f);
			break;
		case SM_Line:
			input.AddQuad(c, d, e, f);
			input.AddBySequence(SQM_LineFan, {a, c, d, e, f});
			input.AddBySequence(SQM_LineFan, {b, c, d, e, f});
			break;
		case SM_Triangle:
			input.AddPolygon({{a, c, d, e, f}});
			input.AddPolygon({{b, c, d, e, f}});
			break;
		case SM_Tetrahedron:
			input.AddPyramid(a, d, c, f, e);
			input.AddPyramid(b, c, d, e, f);
			break;
		}
	}

	void MeshGen::MakeIcosatetrahedroid(Buffer4 &input, float scale)
	{
		for (float x = -1; x <= 1; x += 2)
			for (float y = -1; y <= 1; y += 2)
				for (float z = -1; z <= 1; z += 2)
					for (float w = -1; w <= 1; w += 2)
					{
						input.AddVertex(Vector4(x, y, z, w) * 0.5f * scale);
					}

		for (int a = 0; a < 4; a += 1)
			for (float b = -1; b <= 1; b += 2)
			{
				input.AddVertex(Vector4(a, b) * scale);
			}

		// North/south
		AddDualPyramid(input, 16, 22, 0, 2, 6, 4);
		AddDualPyramid(input, 17, 22, 8, 10, 14, 12);
		AddDualPyramid(input, 18, 22, 0, 2, 10, 8);
		AddDualPyramid(input, 19, 22, 4, 6, 14, 12);
		AddDualPyramid(input, 20, 22, 0, 4, 12, 8);
		AddDualPyramid(input, 21, 22, 2, 6, 14, 10);

		AddDualPyramid(input, 16, 23, 1, 3, 7, 5);
		AddDualPyramid(input, 17, 23, 9, 11, 15, 13);
		AddDualPyramid(input, 18, 23, 1, 3, 11, 9);
		AddDualPyramid(input, 19, 23, 5, 7, 15, 13);
		AddDualPyramid(input, 20, 23, 1, 5, 13, 9);
		AddDualPyramid(input, 21, 23, 3, 7, 15, 11);

		//Equator
		AddDualPyramid(input, 16, 20, 0, 4, 5, 1);
		AddDualPyramid(input, 16, 21, 2, 6, 7, 3);
		AddDualPyramid(input, 17, 20, 8, 12, 13, 9);
		AddDualPyramid(input, 17, 21, 10, 14, 15, 11);

		AddDualPyramid(input, 16, 19, 4, 5, 7, 6);
		AddDualPyramid(input, 17, 19, 12, 13, 15, 14);
		AddDualPyramid(input, 17, 18, 8, 9, 11, 10);
		AddDualPyramid(input, 16, 18, 0, 1, 3, 2);

		AddDualPyramid(input, 18, 20, 0, 1, 9, 8);
		AddDualPyramid(input, 18, 21, 2, 3, 11, 10);
		AddDualPyramid(input, 19, 20, 4, 5, 13, 12);
		AddDualPyramid(input, 19, 21, 6, 7, 15, 14);
	}

	// Building block for primitive dodecahedron
	// The order follows: 5 top, 5 middle up, 5 middle low, 5 bottom. CCW from bird view

	void MeshGen::AddDedocahedron(Buffer4 &input, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p, int q, int r, int s, int t)
	{

		switch (input.simplex)
		{
		case SM_Point:

			input.AddPoints(
				a, b, c, d, e, f, g, h, i, j,
				k, l, m, n, o, p, q, r, s, t);
			break;

		case SM_Line:

			input.AddSegment({a, f, b, g, c, h, d, i, e, j,
							  p, k, q, l, r, m, s, n, t, o});

			input.AddPolygon({a, b, c, d, e});
			input.AddPolygon({p, q, r, s, t});
			input.AddPolygon({f, k, g, l, h, m, i, n, j, o});
			break;

		case SM_Triangle:

			input.AddPolygon({a, b, c, d, e});
			input.AddPolygon({t, s, r, q, p});

			input.AddPolygon({f, k, g, b, a});
			input.AddPolygon({g, l, h, c, b});
			input.AddPolygon({h, m, i, d, c});
			input.AddPolygon({i, n, j, e, d});
			input.AddPolygon({j, o, f, a, e});

			input.AddPolygon({p, q, k, f, o});
			input.AddPolygon({q, r, l, g, k});
			input.AddPolygon({r, s, m, h, l});
			input.AddPolygon({s, t, n, i, m});
			input.AddPolygon({t, p, o, j, n});

			break;

		case SM_Tetrahedron:

			// These have been so carefully ordered that there are no volume 'gap' inside.
			// Don't touch. It took 24 hours with drinking coffee writing these blocks.

			input.AddTrimid(a, b, c, g);
			input.AddTrimid(a, d, e, j);
			input.AddTrimid(c, m, h, l);
			input.AddTrimid(d, m, i, n);

			input.AddTrimid(f, o, j, t);
			input.AddTrimid(f, k, g, p);
			input.AddTrimid(r, q, p, l);
			input.AddTrimid(r, s, t, n);

			input.AddTrimid(a, c, d, m);
			input.AddTrimid(r, p, t, f);

			input.AddPrism(a, f, j, m, r, n);
			input.AddPrism(a, f, g, m, r, l);

			input.AddPyramid(d, a, j, n, m);
			input.AddPyramid(c, a, g, l, m);
			input.AddPyramid(t, f, j, n, r);
			input.AddPyramid(p, f, g, l, n);

			input.AddTrimid(t, r, f, n);
			input.AddTrimid(p, r, f, l);

			input.AddTrimid(a, c, m, l);
			input.AddTrimid(a, d, m, n);

			break;
		default:
			break;
		}
	}

	/// <summary>
	/// Give sign to any nonzero vector elements
	/// </summary>

	Vector4 MeshGen::GiveSign(Vector4 v, float x, float y)
	{
		int loop = 0;
		for (int i = 0; i < 4; i++)
		{
			if (v[i] != 0)
				v[i] *= loop++ == 0 ? x : y;
		}
		return v;
	}

	/// <summary>
	/// Give sign to any nonzero vector elements
	/// </summary>

	Vector4 MeshGen::GiveSign(Vector4 v, float x, float y, float z)
	{
		int loop = 0;
		for (int i = 0; i < 4; i++)
		{
			if (v[i] != 0)
				v[i] *= ++loop == 1 ? x : (loop == 2 ? y : z);
		}
		return v;
	}

	/// <summary>
	/// Give sign to any nonzero vector elements
	/// </summary>

	Vector4 MeshGen::GiveSign(Vector4 &v, float x, float y, float z, float w)
	{
		return v * Vector4(x, y, z, w);
	}

	void MeshGen::MakeHecatonicosahedroid(Buffer4 &input, float scale)
	{
		// 6 * 4 => 24
		for (int i = 0; i < 6; i += 1)
			for (float x = -1; x <= 1; x += 2)
				for (float y = -1; y <= 1; y += 2)
				{
					Vector4 v = _cell120Vertices[i];
					v = GiveSign(v, x, y);
					input.AddVertex(v);
				}
		// 3 * 4 * 16 => 192 + 24 => 216
		for (int i = 0; i < 3; i++)
			for (int j = i == 1 ? 3 : 0; i == 1 ? j >= 0 : j < 4; j = i == 1 ? j - 1 : j + 1)
				for (float x = -1; x <= 1; x += 2)
					for (float y = -1; y <= 1; y += 2)
						for (float z = -1; z <= 1; z += 2)
							for (float w = -1; w <= 1; w += 2)
							{
								Vector4 v = Vector4(1) * _cell120Vertices2[i][1];
								v[j] = _cell120Vertices2[i][0];
								v = GiveSign(v, x, y, z, w);
								input.AddVertex(v);
							}
		// 24 * 8 => 192 + 216 = 408
		for (int i = 0; i < 24; i++)
			for (float x = -1; x <= 1; x += 2)
				for (float y = -1; y <= 1; y += 2)
					for (float z = -1; z <= 1; z += 2)
					{
						Vector4 v = _cell120Vertices3[i];
						v = GiveSign(v, x, y, z);
						input.AddVertex(v);
					}
		// 12 * 16 => 192 + 408 = 600
		for (int i = 0; i < 12; i++)
			for (float x = -1; x <= 1; x += 2)
				for (float y = -1; y <= 1; y += 2)
					for (float z = -1; z <= 1; z += 2)
						for (float w = -1; w <= 1; w += 2)
						{
							Vector4 v = _cell120Vertices4[i];
							v = GiveSign(v, x, y, z, w);
							input.AddVertex(v);
						}

		// Normalize
		float _R = scale / (_GR * _GR);
		for (int i = 0; i < input.verticeCount; i++)
		{
			input.vertices[i] *= _R;
		}

		// These list starts from one, hence shift back -1
		input.offset--;
		// Magic cell-table: http://web.archive.org/web/20091024133911/http://homepages.cwi.nl/~dik/english/mathematics/poly/db/5,3,3/c/s-1.html
		// clang-format off
		AddDedocahedron(input, 1, 313, 217, 218, 314, 337, 409, 153, 155, 411, 457, 25, 225, 27, 459, 105, 425, 321, 427, 107);
		AddDedocahedron(input, 1, 313, 217, 218, 314, 338, 410, 154, 156, 412, 458, 26, 226, 28, 460, 106, 426, 322, 428, 108);
		AddDedocahedron(input, 1, 313, 409, 457, 337, 338, 410, 89, 41, 241, 458, 90, 505, 169, 242, 42, 506, 361, 265, 170);
		AddDedocahedron(input, 1, 314, 411, 459, 337, 338, 412, 91, 43, 241, 460, 92, 507, 171, 242, 44, 508, 362, 266, 172);
		AddDedocahedron(input, 2, 315, 219, 220, 316, 339, 413, 157, 159, 415, 461, 29, 227, 31, 463, 109, 429, 323, 431, 111);
		AddDedocahedron(input, 2, 315, 219, 220, 316, 340, 414, 158, 160, 416, 462, 30, 228, 32, 464, 110, 430, 324, 432, 112);
		AddDedocahedron(input, 2, 315, 413, 461, 339, 340, 414, 93, 45, 243, 462, 94, 509, 173, 244, 46, 510, 363, 267, 174);
		AddDedocahedron(input, 2, 316, 415, 463, 339, 340, 416, 95, 47, 243, 464, 96, 511, 175, 244, 48, 512, 364, 268, 176);
		AddDedocahedron(input, 3, 317, 221, 222, 318, 341, 417, 161, 163, 419, 465, 33, 229, 35, 467, 113, 433, 325, 435, 115);
		AddDedocahedron(input, 3, 317, 221, 222, 318, 342, 418, 162, 164, 420, 466, 34, 230, 36, 468, 114, 434, 326, 436, 116);
		AddDedocahedron(input, 3, 317, 417, 465, 341, 342, 418, 97, 49, 245, 466, 98, 513, 177, 246, 50, 514, 365, 269, 178);
		AddDedocahedron(input, 3, 318, 419, 467, 341, 342, 420, 99, 51, 245, 468, 100, 515, 179, 246, 52, 516, 366, 270, 180);
		AddDedocahedron(input, 4, 319, 223, 224, 320, 343, 421, 165, 167, 423, 469, 37, 231, 39, 471, 117, 437, 327, 439, 119);
		AddDedocahedron(input, 4, 319, 223, 224, 320, 344, 422, 166, 168, 424, 470, 38, 232, 40, 472, 118, 438, 328, 440, 120);
		AddDedocahedron(input, 4, 319, 421, 469, 343, 344, 422, 101, 53, 247, 470, 102, 517, 181, 248, 54, 518, 367, 271, 182);
		AddDedocahedron(input, 4, 320, 423, 471, 343, 344, 424, 103, 55, 247, 472, 104, 519, 183, 248, 56, 520, 368, 272, 184);
		AddDedocahedron(input, 5, 329, 233, 234, 330, 345, 441, 153, 154, 442, 473, 25, 217, 26, 474, 89, 409, 313, 410, 90);
		AddDedocahedron(input, 5, 329, 233, 234, 330, 347, 445, 157, 158, 446, 477, 29, 219, 30, 478, 93, 413, 315, 414, 94);
		AddDedocahedron(input, 5, 329, 441, 473, 345, 347, 445, 121, 57, 249, 477, 125, 537, 185, 251, 61, 541, 377, 281, 189);
		AddDedocahedron(input, 5, 330, 442, 474, 345, 347, 446, 122, 58, 249, 478, 126, 538, 186, 251, 62, 542, 378, 282, 190);
		AddDedocahedron(input, 6, 331, 235, 236, 332, 346, 443, 155, 156, 444, 475, 27, 218, 28, 476, 91, 411, 314, 412, 92);
		AddDedocahedron(input, 6, 331, 235, 236, 332, 348, 447, 159, 160, 448, 479, 31, 220, 32, 480, 95, 415, 316, 416, 96);
		AddDedocahedron(input, 6, 331, 443, 475, 346, 348, 447, 123, 59, 250, 479, 127, 539, 187, 252, 63, 543, 379, 283, 191);
		AddDedocahedron(input, 6, 332, 444, 476, 346, 348, 448, 124, 60, 250, 480, 128, 540, 188, 252, 64, 544, 380, 284, 192);
		AddDedocahedron(input, 7, 333, 237, 238, 334, 349, 449, 161, 162, 450, 481, 33, 221, 34, 482, 97, 417, 317, 418, 98);
		AddDedocahedron(input, 7, 333, 237, 238, 334, 351, 453, 165, 166, 454, 485, 37, 223, 38, 486, 101, 421, 319, 422, 102);
		AddDedocahedron(input, 7, 333, 449, 481, 349, 351, 453, 129, 65, 253, 485, 133, 545, 193, 255, 69, 549, 381, 285, 197);
		AddDedocahedron(input, 7, 334, 450, 482, 349, 351, 454, 130, 66, 253, 486, 134, 546, 194, 255, 70, 550, 382, 286, 198);
		AddDedocahedron(input, 8, 335, 239, 240, 336, 350, 451, 163, 164, 452, 483, 35, 222, 36, 484, 99, 419, 318, 420, 100);
		AddDedocahedron(input, 8, 335, 239, 240, 336, 352, 455, 167, 168, 456, 487, 39, 224, 40, 488, 103, 423, 320, 424, 104);
		AddDedocahedron(input, 8, 335, 451, 483, 350, 352, 455, 131, 67, 254, 487, 135, 547, 195, 256, 71, 551, 383, 287, 199);
		AddDedocahedron(input, 8, 336, 452, 484, 350, 352, 456, 132, 68, 254, 488, 136, 548, 196, 256, 72, 552, 384, 288, 200);
		AddDedocahedron(input, 9, 321, 225, 227, 323, 353, 425, 153, 157, 429, 489, 25, 233, 29, 493, 121, 441, 329, 445, 125);
		AddDedocahedron(input, 9, 321, 225, 227, 323, 355, 427, 155, 159, 431, 491, 27, 235, 31, 495, 123, 443, 331, 447, 127);
		AddDedocahedron(input, 9, 321, 425, 489, 353, 355, 427, 105, 73, 257, 491, 107, 521, 201, 259, 75, 523, 369, 273, 203);
		AddDedocahedron(input, 9, 323, 429, 493, 353, 355, 431, 109, 77, 257, 495, 111, 525, 205, 259, 79, 527, 371, 275, 207);
		AddDedocahedron(input, 10, 322, 226, 228, 324, 354, 426, 154, 158, 430, 490, 26, 234, 30, 494, 122, 442, 330, 446, 126);
		AddDedocahedron(input, 10, 322, 226, 228, 324, 356, 428, 156, 160, 432, 492, 28, 236, 32, 496, 124, 444, 332, 448, 128);
		AddDedocahedron(input, 10, 322, 426, 490, 354, 356, 428, 106, 74, 258, 492, 108, 522, 202, 260, 76, 524, 370, 274, 204);
		AddDedocahedron(input, 10, 324, 430, 494, 354, 356, 432, 110, 78, 258, 496, 112, 526, 206, 260, 80, 528, 372, 276, 208);
		AddDedocahedron(input, 11, 325, 229, 231, 327, 357, 433, 161, 165, 437, 497, 33, 237, 37, 501, 129, 449, 333, 453, 133);
		AddDedocahedron(input, 11, 325, 229, 231, 327, 359, 435, 163, 167, 439, 499, 35, 239, 39, 503, 131, 451, 335, 455, 135);
		AddDedocahedron(input, 11, 325, 433, 497, 357, 359, 435, 113, 81, 261, 499, 115, 529, 209, 263, 83, 531, 373, 277, 211);
		AddDedocahedron(input, 11, 327, 437, 501, 357, 359, 439, 117, 85, 261, 503, 119, 533, 213, 263, 87, 535, 375, 279, 215);
		AddDedocahedron(input, 12, 326, 230, 232, 328, 358, 434, 162, 166, 438, 498, 34, 238, 38, 502, 130, 450, 334, 454, 134);
		AddDedocahedron(input, 12, 326, 230, 232, 328, 360, 436, 164, 168, 440, 500, 36, 240, 40, 504, 132, 452, 336, 456, 136);
		AddDedocahedron(input, 12, 326, 434, 498, 358, 360, 436, 114, 82, 262, 500, 116, 530, 210, 264, 84, 532, 374, 278, 212);
		AddDedocahedron(input, 12, 328, 438, 502, 358, 360, 440, 118, 86, 262, 504, 120, 534, 214, 264, 88, 536, 376, 280, 216);
		AddDedocahedron(input, 13, 361, 265, 269, 365, 393, 505, 169, 177, 513, 569, 41, 289, 49, 577, 137, 553, 385, 561, 145);
		AddDedocahedron(input, 13, 361, 265, 269, 365, 394, 506, 170, 178, 514, 570, 42, 290, 50, 578, 138, 554, 386, 562, 146);
		AddDedocahedron(input, 13, 361, 505, 569, 393, 394, 506, 89, 57, 297, 570, 90, 473, 185, 298, 58, 474, 345, 249, 186);
		AddDedocahedron(input, 13, 365, 513, 577, 393, 394, 514, 97, 65, 297, 578, 98, 481, 193, 298, 66, 482, 349, 253, 194);
		AddDedocahedron(input, 14, 362, 266, 270, 366, 395, 507, 171, 179, 515, 571, 43, 291, 51, 579, 139, 555, 387, 563, 147);
		AddDedocahedron(input, 14, 362, 266, 270, 366, 396, 508, 172, 180, 516, 572, 44, 292, 52, 580, 140, 556, 388, 564, 148);
		AddDedocahedron(input, 14, 362, 507, 571, 395, 396, 508, 91, 59, 299, 572, 92, 475, 187, 300, 60, 476, 346, 250, 188);
		AddDedocahedron(input, 14, 366, 515, 579, 395, 396, 516, 99, 67, 299, 580, 100, 483, 195, 300, 68, 484, 350, 254, 196);
		AddDedocahedron(input, 15, 363, 267, 271, 367, 397, 509, 173, 181, 517, 573, 45, 293, 53, 581, 141, 557, 389, 565, 149);
		AddDedocahedron(input, 15, 363, 267, 271, 367, 398, 510, 174, 182, 518, 574, 46, 294, 54, 582, 142, 558, 390, 566, 150);
		AddDedocahedron(input, 15, 363, 509, 573, 397, 398, 510, 93, 61, 301, 574, 94, 477, 189, 302, 62, 478, 347, 251, 190);
		AddDedocahedron(input, 15, 367, 517, 581, 397, 398, 518, 101, 69, 301, 582, 102, 485, 197, 302, 70, 486, 351, 255, 198);
		AddDedocahedron(input, 16, 364, 268, 272, 368, 399, 511, 175, 183, 519, 575, 47, 295, 55, 583, 143, 559, 391, 567, 151);
		AddDedocahedron(input, 16, 364, 268, 272, 368, 400, 512, 176, 184, 520, 576, 48, 296, 56, 584, 144, 560, 392, 568, 152);
		AddDedocahedron(input, 16, 364, 511, 575, 399, 400, 512, 95, 63, 303, 576, 96, 479, 191, 304, 64, 480, 348, 252, 192);
		AddDedocahedron(input, 16, 368, 519, 583, 399, 400, 520, 103, 71, 303, 584, 104, 487, 199, 304, 72, 488, 352, 256, 200);
		AddDedocahedron(input, 17, 369, 273, 277, 373, 385, 521, 201, 209, 529, 553, 73, 305, 81, 561, 137, 585, 401, 593, 145);
		AddDedocahedron(input, 17, 369, 273, 277, 373, 387, 523, 203, 211, 531, 555, 75, 307, 83, 563, 139, 587, 403, 595, 147);
		AddDedocahedron(input, 17, 369, 521, 553, 385, 387, 523, 105, 41, 289, 555, 107, 457, 169, 291, 43, 459, 337, 241, 171);
		AddDedocahedron(input, 17, 373, 529, 561, 385, 387, 531, 113, 49, 289, 563, 115, 465, 177, 291, 51, 467, 341, 245, 179);
		AddDedocahedron(input, 18, 370, 274, 278, 374, 386, 522, 202, 210, 530, 554, 74, 306, 82, 562, 138, 586, 402, 594, 146);
		AddDedocahedron(input, 18, 370, 274, 278, 374, 388, 524, 204, 212, 532, 556, 76, 308, 84, 564, 140, 588, 404, 596, 148);
		AddDedocahedron(input, 18, 370, 522, 554, 386, 388, 524, 106, 42, 290, 556, 108, 458, 170, 292, 44, 460, 338, 242, 172);
		AddDedocahedron(input, 18, 374, 530, 562, 386, 388, 532, 114, 50, 290, 564, 116, 466, 178, 292, 52, 468, 342, 246, 180);
		AddDedocahedron(input, 19, 371, 275, 279, 375, 389, 525, 205, 213, 533, 557, 77, 309, 85, 565, 141, 589, 405, 597, 149);
		AddDedocahedron(input, 19, 371, 275, 279, 375, 391, 527, 207, 215, 535, 559, 79, 311, 87, 567, 143, 591, 407, 599, 151);
		AddDedocahedron(input, 19, 371, 525, 557, 389, 391, 527, 109, 45, 293, 559, 111, 461, 173, 295, 47, 463, 339, 243, 175);
		AddDedocahedron(input, 19, 375, 533, 565, 389, 391, 535, 117, 53, 293, 567, 119, 469, 181, 295, 55, 471, 343, 247, 183);
		AddDedocahedron(input, 20, 372, 276, 280, 376, 390, 526, 206, 214, 534, 558, 78, 310, 86, 566, 142, 590, 406, 598, 150);
		AddDedocahedron(input, 20, 372, 276, 280, 376, 392, 528, 208, 216, 536, 560, 80, 312, 88, 568, 144, 592, 408, 600, 152);
		AddDedocahedron(input, 20, 372, 526, 558, 390, 392, 528, 110, 46, 294, 560, 112, 462, 174, 296, 48, 464, 340, 244, 176);
		AddDedocahedron(input, 20, 376, 534, 566, 390, 392, 536, 118, 54, 294, 568, 120, 470, 182, 296, 56, 472, 344, 248, 184);
		AddDedocahedron(input, 21, 377, 281, 285, 381, 401, 537, 185, 193, 545, 585, 57, 297, 65, 593, 137, 569, 393, 577, 145);
		AddDedocahedron(input, 21, 377, 281, 285, 381, 405, 541, 189, 197, 549, 589, 61, 301, 69, 597, 141, 573, 397, 581, 149);
		AddDedocahedron(input, 21, 377, 537, 585, 401, 405, 541, 121, 73, 305, 589, 125, 489, 201, 309, 77, 493, 353, 257, 205);
		AddDedocahedron(input, 21, 381, 545, 593, 401, 405, 549, 129, 81, 305, 597, 133, 497, 209, 309, 85, 501, 357, 261, 213);
		AddDedocahedron(input, 22, 378, 282, 286, 382, 402, 538, 186, 194, 546, 586, 58, 298, 66, 594, 138, 570, 394, 578, 146);
		AddDedocahedron(input, 22, 378, 282, 286, 382, 406, 542, 190, 198, 550, 590, 62, 302, 70, 598, 142, 574, 398, 582, 150);
		AddDedocahedron(input, 22, 378, 538, 586, 402, 406, 542, 122, 74, 306, 590, 126, 490, 202, 310, 78, 494, 354, 258, 206);
		AddDedocahedron(input, 22, 382, 546, 594, 402, 406, 550, 130, 82, 306, 598, 134, 498, 210, 310, 86, 502, 358, 262, 214);
		AddDedocahedron(input, 23, 379, 283, 287, 383, 403, 539, 187, 195, 547, 587, 59, 299, 67, 595, 139, 571, 395, 579, 147);
		AddDedocahedron(input, 23, 379, 283, 287, 383, 407, 543, 191, 199, 551, 591, 63, 303, 71, 599, 143, 575, 399, 583, 151);
		AddDedocahedron(input, 23, 379, 539, 587, 403, 407, 543, 123, 75, 307, 591, 127, 491, 203, 311, 79, 495, 355, 259, 207);
		AddDedocahedron(input, 23, 383, 547, 595, 403, 407, 551, 131, 83, 307, 599, 135, 499, 211, 311, 87, 503, 359, 263, 215);
		AddDedocahedron(input, 24, 380, 284, 288, 384, 404, 540, 188, 196, 548, 588, 60, 300, 68, 596, 140, 572, 396, 580, 148);
		AddDedocahedron(input, 24, 380, 284, 288, 384, 408, 544, 192, 200, 552, 592, 64, 304, 72, 600, 144, 576, 400, 584, 152);
		AddDedocahedron(input, 24, 380, 540, 588, 404, 408, 544, 124, 76, 308, 592, 128, 492, 204, 312, 80, 496, 356, 260, 208);
		AddDedocahedron(input, 24, 384, 548, 596, 404, 408, 552, 132, 84, 308, 600, 136, 500, 212, 312, 88, 504, 360, 264, 216);
		AddDedocahedron(input, 25, 409, 89, 473, 441, 425, 457, 505, 57, 121, 105, 41, 569, 537, 489, 521, 553, 137, 585, 73);
		AddDedocahedron(input, 26, 410, 90, 474, 442, 426, 458, 506, 58, 122, 106, 42, 570, 538, 490, 522, 554, 138, 586, 74);
		AddDedocahedron(input, 27, 411, 91, 475, 443, 427, 459, 507, 59, 123, 107, 43, 571, 539, 491, 523, 555, 139, 587, 75);
		AddDedocahedron(input, 28, 412, 92, 476, 444, 428, 460, 508, 60, 124, 108, 44, 572, 540, 492, 524, 556, 140, 588, 76);
		AddDedocahedron(input, 29, 413, 93, 477, 445, 429, 461, 509, 61, 125, 109, 45, 573, 541, 493, 525, 557, 141, 589, 77);
		AddDedocahedron(input, 30, 414, 94, 478, 446, 430, 462, 510, 62, 126, 110, 46, 574, 542, 494, 526, 558, 142, 590, 78);
		AddDedocahedron(input, 31, 415, 95, 479, 447, 431, 463, 511, 63, 127, 111, 47, 575, 543, 495, 527, 559, 143, 591, 79);
		AddDedocahedron(input, 32, 416, 96, 480, 448, 432, 464, 512, 64, 128, 112, 48, 576, 544, 496, 528, 560, 144, 592, 80);
		AddDedocahedron(input, 33, 417, 97, 481, 449, 433, 465, 513, 65, 129, 113, 49, 577, 545, 497, 529, 561, 145, 593, 81);
		AddDedocahedron(input, 34, 418, 98, 482, 450, 434, 466, 514, 66, 130, 114, 50, 578, 546, 498, 530, 562, 146, 594, 82);
		AddDedocahedron(input, 35, 419, 99, 483, 451, 435, 467, 515, 67, 131, 115, 51, 579, 547, 499, 531, 563, 147, 595, 83);
		AddDedocahedron(input, 36, 420, 100, 484, 452, 436, 468, 516, 68, 132, 116, 52, 580, 548, 500, 532, 564, 148, 596, 84);
		AddDedocahedron(input, 37, 421, 101, 485, 453, 437, 469, 517, 69, 133, 117, 53, 581, 549, 501, 533, 565, 149, 597, 85);
		AddDedocahedron(input, 38, 422, 102, 486, 454, 438, 470, 518, 70, 134, 118, 54, 582, 550, 502, 534, 566, 150, 598, 86);
		AddDedocahedron(input, 39, 423, 103, 487, 455, 439, 471, 519, 71, 135, 119, 55, 583, 551, 503, 535, 567, 151, 599, 87);
		AddDedocahedron(input, 40, 424, 104, 488, 456, 440, 472, 520, 72, 136, 120, 56, 584, 552, 504, 536, 568, 152, 600, 88);
		AddDedocahedron(input, 153, 217, 154, 234, 233, 225, 218, 226, 158, 157, 155, 156, 228, 219, 227, 235, 236, 160, 220, 159);
		AddDedocahedron(input, 161, 221, 162, 238, 237, 229, 222, 230, 166, 165, 163, 164, 232, 223, 231, 239, 240, 168, 224, 167);
		AddDedocahedron(input, 169, 241, 171, 291, 289, 265, 242, 266, 179, 177, 170, 172, 270, 245, 269, 290, 292, 180, 246, 178);
		AddDedocahedron(input, 173, 243, 175, 295, 293, 267, 244, 268, 183, 181, 174, 176, 272, 247, 271, 294, 296, 184, 248, 182);
		AddDedocahedron(input, 185, 249, 186, 298, 297, 281, 251, 282, 194, 193, 189, 190, 286, 253, 285, 301, 302, 198, 255, 197);
		AddDedocahedron(input, 187, 250, 188, 300, 299, 283, 252, 284, 196, 195, 191, 192, 288, 254, 287, 303, 304, 200, 256, 199);
		AddDedocahedron(input, 201, 257, 205, 309, 305, 273, 259, 275, 213, 209, 203, 207, 279, 261, 277, 307, 311, 215, 263, 211);
		AddDedocahedron(input, 202, 258, 206, 310, 306, 274, 260, 276, 214, 210, 204, 208, 280, 262, 278, 308, 312, 216, 264, 212);
		// clang-format on
	}

	void MeshGen::MakeHexacosidedroid(Buffer4 &input, float scale)
	{
		// 4 * 3 * 2 * 2 * 2 = 12 * 6 = 96
		for (int i = 0; i < 12; i += 1)
			for (float x = -1; x <= 1; x += 2)
				for (float y = -1; y <= 1; y += 2)
					for (float z = -1; z <= 1; z += 2)
					{
						Vector4 v = _hexacosidedroidVertices[i];
						v = GiveSign(v, x, y, z);
						input.AddVertex(v);
					}

		// 104 + 8 = 112
		for (int a = 0; a < 4; a += 1)
			for (float b = -1; b <= 1; b += 2)
			{
				Vector4 v = Vector4(a, b * 2);
				input.AddVertex(v);
			}
		// 96 + 8 = 104
		for (float x = -1; x <= 1; x += 2)
			for (float y = -1; y <= 1; y += 2)
				for (float z = -1; z <= 1; z += 2)
					for (float w = -1; w <= 1; w += 2)
					{
						Vector4 v = Vector4(x, y, z, w);
						input.AddVertex(v);
					}

		// Normalize
		for (int i = 0; i < input.verticeCount; i++)
		{
			input.vertices[i] = input.vertices[i] * (scale / 2.f);
		}

		// Magic cell-table: http://web.archive.org/web/20091024133911/http://homepages.cwi.nl/~dik/english/mathematics/poly/db/3,3,5/c/s-1.html
		// clang-format off
		input.offset--;
		input.AddTrimid(1, 2, 9, 25);
		input.AddTrimid(1, 2, 9, 97);
		input.AddTrimid(1, 2, 10, 26);
		input.AddTrimid(1, 2, 10, 97);
		input.AddTrimid(1, 2, 25, 26);
		input.AddTrimid(1, 9, 17, 97);
		input.AddTrimid(1, 9, 17, 105);
		input.AddTrimid(1, 9, 25, 105);
		input.AddTrimid(1, 10, 18, 97);
		input.AddTrimid(1, 10, 18, 106);
		input.AddTrimid(1, 10, 26, 106);
		input.AddTrimid(1, 17, 18, 33);
		input.AddTrimid(1, 17, 18, 97);
		input.AddTrimid(1, 17, 33, 105);
		input.AddTrimid(1, 18, 33, 106);
		input.AddTrimid(1, 25, 26, 49);
		input.AddTrimid(1, 25, 49, 105);
		input.AddTrimid(1, 26, 49, 106);
		input.AddTrimid(1, 33, 49, 105);
		input.AddTrimid(1, 33, 49, 106);
		input.AddTrimid(2, 9, 19, 97);
		input.AddTrimid(2, 9, 19, 107);
		input.AddTrimid(2, 9, 25, 107);
		input.AddTrimid(2, 10, 20, 97);
		input.AddTrimid(2, 10, 20, 108);
		input.AddTrimid(2, 10, 26, 108);
		input.AddTrimid(2, 19, 20, 34);
		input.AddTrimid(2, 19, 20, 97);
		input.AddTrimid(2, 19, 34, 107);
		input.AddTrimid(2, 20, 34, 108);
		input.AddTrimid(2, 25, 26, 50);
		input.AddTrimid(2, 25, 50, 107);
		input.AddTrimid(2, 26, 50, 108);
		input.AddTrimid(2, 34, 50, 107);
		input.AddTrimid(2, 34, 50, 108);
		input.AddTrimid(3, 4, 11, 27);
		input.AddTrimid(3, 4, 11, 97);
		input.AddTrimid(3, 4, 12, 28);
		input.AddTrimid(3, 4, 12, 97);
		input.AddTrimid(3, 4, 27, 28);
		input.AddTrimid(3, 11, 17, 97);
		input.AddTrimid(3, 11, 17, 109);
		input.AddTrimid(3, 11, 27, 109);
		input.AddTrimid(3, 12, 18, 97);
		input.AddTrimid(3, 12, 18, 110);
		input.AddTrimid(3, 12, 28, 110);
		input.AddTrimid(3, 17, 18, 35);
		input.AddTrimid(3, 17, 18, 97);
		input.AddTrimid(3, 17, 35, 109);
		input.AddTrimid(3, 18, 35, 110);
		input.AddTrimid(3, 27, 28, 51);
		input.AddTrimid(3, 27, 51, 109);
		input.AddTrimid(3, 28, 51, 110);
		input.AddTrimid(3, 35, 51, 109);
		input.AddTrimid(3, 35, 51, 110);
		input.AddTrimid(4, 11, 19, 97);
		input.AddTrimid(4, 11, 19, 111);
		input.AddTrimid(4, 11, 27, 111);
		input.AddTrimid(4, 12, 20, 97);
		input.AddTrimid(4, 12, 20, 112);
		input.AddTrimid(4, 12, 28, 112);
		input.AddTrimid(4, 19, 20, 36);
		input.AddTrimid(4, 19, 20, 97);
		input.AddTrimid(4, 19, 36, 111);
		input.AddTrimid(4, 20, 36, 112);
		input.AddTrimid(4, 27, 28, 52);
		input.AddTrimid(4, 27, 52, 111);
		input.AddTrimid(4, 28, 52, 112);
		input.AddTrimid(4, 36, 52, 111);
		input.AddTrimid(4, 36, 52, 112);
		input.AddTrimid(5, 6, 13, 29);
		input.AddTrimid(5, 6, 13, 98);
		input.AddTrimid(5, 6, 14, 30);
		input.AddTrimid(5, 6, 14, 98);
		input.AddTrimid(5, 6, 29, 30);
		input.AddTrimid(5, 13, 21, 98);
		input.AddTrimid(5, 13, 21, 113);
		input.AddTrimid(5, 13, 29, 113);
		input.AddTrimid(5, 14, 22, 98);
		input.AddTrimid(5, 14, 22, 114);
		input.AddTrimid(5, 14, 30, 114);
		input.AddTrimid(5, 21, 22, 37);
		input.AddTrimid(5, 21, 22, 98);
		input.AddTrimid(5, 21, 37, 113);
		input.AddTrimid(5, 22, 37, 114);
		input.AddTrimid(5, 29, 30, 53);
		input.AddTrimid(5, 29, 53, 113);
		input.AddTrimid(5, 30, 53, 114);
		input.AddTrimid(5, 37, 53, 113);
		input.AddTrimid(5, 37, 53, 114);
		input.AddTrimid(6, 13, 23, 98);
		input.AddTrimid(6, 13, 23, 115);
		input.AddTrimid(6, 13, 29, 115);
		input.AddTrimid(6, 14, 24, 98);
		input.AddTrimid(6, 14, 24, 116);
		input.AddTrimid(6, 14, 30, 116);
		input.AddTrimid(6, 23, 24, 38);
		input.AddTrimid(6, 23, 24, 98);
		input.AddTrimid(6, 23, 38, 115);
		input.AddTrimid(6, 24, 38, 116);
		input.AddTrimid(6, 29, 30, 54);
		input.AddTrimid(6, 29, 54, 115);
		input.AddTrimid(6, 30, 54, 116);
		input.AddTrimid(6, 38, 54, 115);
		input.AddTrimid(6, 38, 54, 116);
		input.AddTrimid(7, 8, 15, 31);
		input.AddTrimid(7, 8, 15, 98);
		input.AddTrimid(7, 8, 16, 32);
		input.AddTrimid(7, 8, 16, 98);
		input.AddTrimid(7, 8, 31, 32);
		input.AddTrimid(7, 15, 21, 98);
		input.AddTrimid(7, 15, 21, 117);
		input.AddTrimid(7, 15, 31, 117);
		input.AddTrimid(7, 16, 22, 98);
		input.AddTrimid(7, 16, 22, 118);
		input.AddTrimid(7, 16, 32, 118);
		input.AddTrimid(7, 21, 22, 39);
		input.AddTrimid(7, 21, 22, 98);
		input.AddTrimid(7, 21, 39, 117);
		input.AddTrimid(7, 22, 39, 118);
		input.AddTrimid(7, 31, 32, 55);
		input.AddTrimid(7, 31, 55, 117);
		input.AddTrimid(7, 32, 55, 118);
		input.AddTrimid(7, 39, 55, 117);
		input.AddTrimid(7, 39, 55, 118);
		input.AddTrimid(8, 15, 23, 98);
		input.AddTrimid(8, 15, 23, 119);
		input.AddTrimid(8, 15, 31, 119);
		input.AddTrimid(8, 16, 24, 98);
		input.AddTrimid(8, 16, 24, 120);
		input.AddTrimid(8, 16, 32, 120);
		input.AddTrimid(8, 23, 24, 40);
		input.AddTrimid(8, 23, 24, 98);
		input.AddTrimid(8, 23, 40, 119);
		input.AddTrimid(8, 24, 40, 120);
		input.AddTrimid(8, 31, 32, 56);
		input.AddTrimid(8, 31, 56, 119);
		input.AddTrimid(8, 32, 56, 120);
		input.AddTrimid(8, 40, 56, 119);
		input.AddTrimid(8, 40, 56, 120);
		input.AddTrimid(9, 11, 17, 41);
		input.AddTrimid(9, 11, 17, 97);
		input.AddTrimid(9, 11, 19, 43);
		input.AddTrimid(9, 11, 19, 97);
		input.AddTrimid(9, 11, 41, 43);
		input.AddTrimid(9, 17, 41, 105);
		input.AddTrimid(9, 19, 43, 107);
		input.AddTrimid(9, 25, 57, 105);
		input.AddTrimid(9, 25, 57, 107);
		input.AddTrimid(9, 41, 43, 57);
		input.AddTrimid(9, 41, 57, 105);
		input.AddTrimid(9, 43, 57, 107);
		input.AddTrimid(10, 12, 18, 42);
		input.AddTrimid(10, 12, 18, 97);
		input.AddTrimid(10, 12, 20, 44);
		input.AddTrimid(10, 12, 20, 97);
		input.AddTrimid(10, 12, 42, 44);
		input.AddTrimid(10, 18, 42, 106);
		input.AddTrimid(10, 20, 44, 108);
		input.AddTrimid(10, 26, 58, 106);
		input.AddTrimid(10, 26, 58, 108);
		input.AddTrimid(10, 42, 44, 58);
		input.AddTrimid(10, 42, 58, 106);
		input.AddTrimid(10, 44, 58, 108);
		input.AddTrimid(11, 17, 41, 109);
		input.AddTrimid(11, 19, 43, 111);
		input.AddTrimid(11, 27, 59, 109);
		input.AddTrimid(11, 27, 59, 111);
		input.AddTrimid(11, 41, 43, 59);
		input.AddTrimid(11, 41, 59, 109);
		input.AddTrimid(11, 43, 59, 111);
		input.AddTrimid(12, 18, 42, 110);
		input.AddTrimid(12, 20, 44, 112);
		input.AddTrimid(12, 28, 60, 110);
		input.AddTrimid(12, 28, 60, 112);
		input.AddTrimid(12, 42, 44, 60);
		input.AddTrimid(12, 42, 60, 110);
		input.AddTrimid(12, 44, 60, 112);
		input.AddTrimid(13, 15, 21, 45);
		input.AddTrimid(13, 15, 21, 98);
		input.AddTrimid(13, 15, 23, 47);
		input.AddTrimid(13, 15, 23, 98);
		input.AddTrimid(13, 15, 45, 47);
		input.AddTrimid(13, 21, 45, 113);
		input.AddTrimid(13, 23, 47, 115);
		input.AddTrimid(13, 29, 61, 113);
		input.AddTrimid(13, 29, 61, 115);
		input.AddTrimid(13, 45, 47, 61);
		input.AddTrimid(13, 45, 61, 113);
		input.AddTrimid(13, 47, 61, 115);
		input.AddTrimid(14, 16, 22, 46);
		input.AddTrimid(14, 16, 22, 98);
		input.AddTrimid(14, 16, 24, 48);
		input.AddTrimid(14, 16, 24, 98);
		input.AddTrimid(14, 16, 46, 48);
		input.AddTrimid(14, 22, 46, 114);
		input.AddTrimid(14, 24, 48, 116);
		input.AddTrimid(14, 30, 62, 114);
		input.AddTrimid(14, 30, 62, 116);
		input.AddTrimid(14, 46, 48, 62);
		input.AddTrimid(14, 46, 62, 114);
		input.AddTrimid(14, 48, 62, 116);
		input.AddTrimid(15, 21, 45, 117);
		input.AddTrimid(15, 23, 47, 119);
		input.AddTrimid(15, 31, 63, 117);
		input.AddTrimid(15, 31, 63, 119);
		input.AddTrimid(15, 45, 47, 63);
		input.AddTrimid(15, 45, 63, 117);
		input.AddTrimid(15, 47, 63, 119);
		input.AddTrimid(16, 22, 46, 118);
		input.AddTrimid(16, 24, 48, 120);
		input.AddTrimid(16, 32, 64, 118);
		input.AddTrimid(16, 32, 64, 120);
		input.AddTrimid(16, 46, 48, 64);
		input.AddTrimid(16, 46, 64, 118);
		input.AddTrimid(16, 48, 64, 120);
		input.AddTrimid(17, 18, 33, 35);
		input.AddTrimid(17, 33, 35, 65);
		input.AddTrimid(17, 33, 65, 105);
		input.AddTrimid(17, 35, 65, 109);
		input.AddTrimid(17, 41, 65, 105);
		input.AddTrimid(17, 41, 65, 109);
		input.AddTrimid(18, 33, 35, 66);
		input.AddTrimid(18, 33, 66, 106);
		input.AddTrimid(18, 35, 66, 110);
		input.AddTrimid(18, 42, 66, 106);
		input.AddTrimid(18, 42, 66, 110);
		input.AddTrimid(19, 20, 34, 36);
		input.AddTrimid(19, 34, 36, 67);
		input.AddTrimid(19, 34, 67, 107);
		input.AddTrimid(19, 36, 67, 111);
		input.AddTrimid(19, 43, 67, 107);
		input.AddTrimid(19, 43, 67, 111);
		input.AddTrimid(20, 34, 36, 68);
		input.AddTrimid(20, 34, 68, 108);
		input.AddTrimid(20, 36, 68, 112);
		input.AddTrimid(20, 44, 68, 108);
		input.AddTrimid(20, 44, 68, 112);
		input.AddTrimid(21, 22, 37, 39);
		input.AddTrimid(21, 37, 39, 69);
		input.AddTrimid(21, 37, 69, 113);
		input.AddTrimid(21, 39, 69, 117);
		input.AddTrimid(21, 45, 69, 113);
		input.AddTrimid(21, 45, 69, 117);
		input.AddTrimid(22, 37, 39, 70);
		input.AddTrimid(22, 37, 70, 114);
		input.AddTrimid(22, 39, 70, 118);
		input.AddTrimid(22, 46, 70, 114);
		input.AddTrimid(22, 46, 70, 118);
		input.AddTrimid(23, 24, 38, 40);
		input.AddTrimid(23, 38, 40, 71);
		input.AddTrimid(23, 38, 71, 115);
		input.AddTrimid(23, 40, 71, 119);
		input.AddTrimid(23, 47, 71, 115);
		input.AddTrimid(23, 47, 71, 119);
		input.AddTrimid(24, 38, 40, 72);
		input.AddTrimid(24, 38, 72, 116);
		input.AddTrimid(24, 40, 72, 120);
		input.AddTrimid(24, 48, 72, 116);
		input.AddTrimid(24, 48, 72, 120);
		input.AddTrimid(25, 26, 49, 99);
		input.AddTrimid(25, 26, 50, 99);
		input.AddTrimid(25, 49, 73, 99);
		input.AddTrimid(25, 49, 73, 105);
		input.AddTrimid(25, 50, 75, 99);
		input.AddTrimid(25, 50, 75, 107);
		input.AddTrimid(25, 57, 73, 75);
		input.AddTrimid(25, 57, 73, 105);
		input.AddTrimid(25, 57, 75, 107);
		input.AddTrimid(25, 73, 75, 99);
		input.AddTrimid(26, 49, 74, 99);
		input.AddTrimid(26, 49, 74, 106);
		input.AddTrimid(26, 50, 76, 99);
		input.AddTrimid(26, 50, 76, 108);
		input.AddTrimid(26, 58, 74, 76);
		input.AddTrimid(26, 58, 74, 106);
		input.AddTrimid(26, 58, 76, 108);
		input.AddTrimid(26, 74, 76, 99);
		input.AddTrimid(27, 28, 51, 100);
		input.AddTrimid(27, 28, 52, 100);
		input.AddTrimid(27, 51, 77, 100);
		input.AddTrimid(27, 51, 77, 109);
		input.AddTrimid(27, 52, 79, 100);
		input.AddTrimid(27, 52, 79, 111);
		input.AddTrimid(27, 59, 77, 79);
		input.AddTrimid(27, 59, 77, 109);
		input.AddTrimid(27, 59, 79, 111);
		input.AddTrimid(27, 77, 79, 100);
		input.AddTrimid(28, 51, 78, 100);
		input.AddTrimid(28, 51, 78, 110);
		input.AddTrimid(28, 52, 80, 100);
		input.AddTrimid(28, 52, 80, 112);
		input.AddTrimid(28, 60, 78, 80);
		input.AddTrimid(28, 60, 78, 110);
		input.AddTrimid(28, 60, 80, 112);
		input.AddTrimid(28, 78, 80, 100);
		input.AddTrimid(29, 30, 53, 99);
		input.AddTrimid(29, 30, 54, 99);
		input.AddTrimid(29, 53, 73, 99);
		input.AddTrimid(29, 53, 73, 113);
		input.AddTrimid(29, 54, 75, 99);
		input.AddTrimid(29, 54, 75, 115);
		input.AddTrimid(29, 61, 73, 75);
		input.AddTrimid(29, 61, 73, 113);
		input.AddTrimid(29, 61, 75, 115);
		input.AddTrimid(29, 73, 75, 99);
		input.AddTrimid(30, 53, 74, 99);
		input.AddTrimid(30, 53, 74, 114);
		input.AddTrimid(30, 54, 76, 99);
		input.AddTrimid(30, 54, 76, 116);
		input.AddTrimid(30, 62, 74, 76);
		input.AddTrimid(30, 62, 74, 114);
		input.AddTrimid(30, 62, 76, 116);
		input.AddTrimid(30, 74, 76, 99);
		input.AddTrimid(31, 32, 55, 100);
		input.AddTrimid(31, 32, 56, 100);
		input.AddTrimid(31, 55, 77, 100);
		input.AddTrimid(31, 55, 77, 117);
		input.AddTrimid(31, 56, 79, 100);
		input.AddTrimid(31, 56, 79, 119);
		input.AddTrimid(31, 63, 77, 79);
		input.AddTrimid(31, 63, 77, 117);
		input.AddTrimid(31, 63, 79, 119);
		input.AddTrimid(31, 77, 79, 100);
		input.AddTrimid(32, 55, 78, 100);
		input.AddTrimid(32, 55, 78, 118);
		input.AddTrimid(32, 56, 80, 100);
		input.AddTrimid(32, 56, 80, 120);
		input.AddTrimid(32, 64, 78, 80);
		input.AddTrimid(32, 64, 78, 118);
		input.AddTrimid(32, 64, 80, 120);
		input.AddTrimid(32, 78, 80, 100);
		input.AddTrimid(33, 35, 65, 101);
		input.AddTrimid(33, 35, 66, 101);
		input.AddTrimid(33, 49, 81, 82);
		input.AddTrimid(33, 49, 81, 105);
		input.AddTrimid(33, 49, 82, 106);
		input.AddTrimid(33, 65, 81, 101);
		input.AddTrimid(33, 65, 81, 105);
		input.AddTrimid(33, 66, 82, 101);
		input.AddTrimid(33, 66, 82, 106);
		input.AddTrimid(33, 81, 82, 101);
		input.AddTrimid(34, 36, 67, 102);
		input.AddTrimid(34, 36, 68, 102);
		input.AddTrimid(34, 50, 83, 84);
		input.AddTrimid(34, 50, 83, 107);
		input.AddTrimid(34, 50, 84, 108);
		input.AddTrimid(34, 67, 83, 102);
		input.AddTrimid(34, 67, 83, 107);
		input.AddTrimid(34, 68, 84, 102);
		input.AddTrimid(34, 68, 84, 108);
		input.AddTrimid(34, 83, 84, 102);
		input.AddTrimid(35, 51, 85, 86);
		input.AddTrimid(35, 51, 85, 109);
		input.AddTrimid(35, 51, 86, 110);
		input.AddTrimid(35, 65, 85, 101);
		input.AddTrimid(35, 65, 85, 109);
		input.AddTrimid(35, 66, 86, 101);
		input.AddTrimid(35, 66, 86, 110);
		input.AddTrimid(35, 85, 86, 101);
		input.AddTrimid(36, 52, 87, 88);
		input.AddTrimid(36, 52, 87, 111);
		input.AddTrimid(36, 52, 88, 112);
		input.AddTrimid(36, 67, 87, 102);
		input.AddTrimid(36, 67, 87, 111);
		input.AddTrimid(36, 68, 88, 102);
		input.AddTrimid(36, 68, 88, 112);
		input.AddTrimid(36, 87, 88, 102);
		input.AddTrimid(37, 39, 69, 101);
		input.AddTrimid(37, 39, 70, 101);
		input.AddTrimid(37, 53, 81, 82);
		input.AddTrimid(37, 53, 81, 113);
		input.AddTrimid(37, 53, 82, 114);
		input.AddTrimid(37, 69, 81, 101);
		input.AddTrimid(37, 69, 81, 113);
		input.AddTrimid(37, 70, 82, 101);
		input.AddTrimid(37, 70, 82, 114);
		input.AddTrimid(37, 81, 82, 101);
		input.AddTrimid(38, 40, 71, 102);
		input.AddTrimid(38, 40, 72, 102);
		input.AddTrimid(38, 54, 83, 84);
		input.AddTrimid(38, 54, 83, 115);
		input.AddTrimid(38, 54, 84, 116);
		input.AddTrimid(38, 71, 83, 102);
		input.AddTrimid(38, 71, 83, 115);
		input.AddTrimid(38, 72, 84, 102);
		input.AddTrimid(38, 72, 84, 116);
		input.AddTrimid(38, 83, 84, 102);
		input.AddTrimid(39, 55, 85, 86);
		input.AddTrimid(39, 55, 85, 117);
		input.AddTrimid(39, 55, 86, 118);
		input.AddTrimid(39, 69, 85, 101);
		input.AddTrimid(39, 69, 85, 117);
		input.AddTrimid(39, 70, 86, 101);
		input.AddTrimid(39, 70, 86, 118);
		input.AddTrimid(39, 85, 86, 101);
		input.AddTrimid(40, 56, 87, 88);
		input.AddTrimid(40, 56, 87, 119);
		input.AddTrimid(40, 56, 88, 120);
		input.AddTrimid(40, 71, 87, 102);
		input.AddTrimid(40, 71, 87, 119);
		input.AddTrimid(40, 72, 88, 102);
		input.AddTrimid(40, 72, 88, 120);
		input.AddTrimid(40, 87, 88, 102);
		input.AddTrimid(41, 43, 57, 103);
		input.AddTrimid(41, 43, 59, 103);
		input.AddTrimid(41, 57, 89, 103);
		input.AddTrimid(41, 57, 89, 105);
		input.AddTrimid(41, 59, 93, 103);
		input.AddTrimid(41, 59, 93, 109);
		input.AddTrimid(41, 65, 89, 93);
		input.AddTrimid(41, 65, 89, 105);
		input.AddTrimid(41, 65, 93, 109);
		input.AddTrimid(41, 89, 93, 103);
		input.AddTrimid(42, 44, 58, 104);
		input.AddTrimid(42, 44, 60, 104);
		input.AddTrimid(42, 58, 90, 104);
		input.AddTrimid(42, 58, 90, 106);
		input.AddTrimid(42, 60, 94, 104);
		input.AddTrimid(42, 60, 94, 110);
		input.AddTrimid(42, 66, 90, 94);
		input.AddTrimid(42, 66, 90, 106);
		input.AddTrimid(42, 66, 94, 110);
		input.AddTrimid(42, 90, 94, 104);
		input.AddTrimid(43, 57, 91, 103);
		input.AddTrimid(43, 57, 91, 107);
		input.AddTrimid(43, 59, 95, 103);
		input.AddTrimid(43, 59, 95, 111);
		input.AddTrimid(43, 67, 91, 95);
		input.AddTrimid(43, 67, 91, 107);
		input.AddTrimid(43, 67, 95, 111);
		input.AddTrimid(43, 91, 95, 103);
		input.AddTrimid(44, 58, 92, 104);
		input.AddTrimid(44, 58, 92, 108);
		input.AddTrimid(44, 60, 96, 104);
		input.AddTrimid(44, 60, 96, 112);
		input.AddTrimid(44, 68, 92, 96);
		input.AddTrimid(44, 68, 92, 108);
		input.AddTrimid(44, 68, 96, 112);
		input.AddTrimid(44, 92, 96, 104);
		input.AddTrimid(45, 47, 61, 103);
		input.AddTrimid(45, 47, 63, 103);
		input.AddTrimid(45, 61, 89, 103);
		input.AddTrimid(45, 61, 89, 113);
		input.AddTrimid(45, 63, 93, 103);
		input.AddTrimid(45, 63, 93, 117);
		input.AddTrimid(45, 69, 89, 93);
		input.AddTrimid(45, 69, 89, 113);
		input.AddTrimid(45, 69, 93, 117);
		input.AddTrimid(45, 89, 93, 103);
		input.AddTrimid(46, 48, 62, 104);
		input.AddTrimid(46, 48, 64, 104);
		input.AddTrimid(46, 62, 90, 104);
		input.AddTrimid(46, 62, 90, 114);
		input.AddTrimid(46, 64, 94, 104);
		input.AddTrimid(46, 64, 94, 118);
		input.AddTrimid(46, 70, 90, 94);
		input.AddTrimid(46, 70, 90, 114);
		input.AddTrimid(46, 70, 94, 118);
		input.AddTrimid(46, 90, 94, 104);
		input.AddTrimid(47, 61, 91, 103);
		input.AddTrimid(47, 61, 91, 115);
		input.AddTrimid(47, 63, 95, 103);
		input.AddTrimid(47, 63, 95, 119);
		input.AddTrimid(47, 71, 91, 95);
		input.AddTrimid(47, 71, 91, 115);
		input.AddTrimid(47, 71, 95, 119);
		input.AddTrimid(47, 91, 95, 103);
		input.AddTrimid(48, 62, 92, 104);
		input.AddTrimid(48, 62, 92, 116);
		input.AddTrimid(48, 64, 96, 104);
		input.AddTrimid(48, 64, 96, 120);
		input.AddTrimid(48, 72, 92, 96);
		input.AddTrimid(48, 72, 92, 116);
		input.AddTrimid(48, 72, 96, 120);
		input.AddTrimid(48, 92, 96, 104);
		input.AddTrimid(49, 53, 73, 81);
		input.AddTrimid(49, 53, 73, 99);
		input.AddTrimid(49, 53, 74, 82);
		input.AddTrimid(49, 53, 74, 99);
		input.AddTrimid(49, 53, 81, 82);
		input.AddTrimid(49, 73, 81, 105);
		input.AddTrimid(49, 74, 82, 106);
		input.AddTrimid(50, 54, 75, 83);
		input.AddTrimid(50, 54, 75, 99);
		input.AddTrimid(50, 54, 76, 84);
		input.AddTrimid(50, 54, 76, 99);
		input.AddTrimid(50, 54, 83, 84);
		input.AddTrimid(50, 75, 83, 107);
		input.AddTrimid(50, 76, 84, 108);
		input.AddTrimid(51, 55, 77, 85);
		input.AddTrimid(51, 55, 77, 100);
		input.AddTrimid(51, 55, 78, 86);
		input.AddTrimid(51, 55, 78, 100);
		input.AddTrimid(51, 55, 85, 86);
		input.AddTrimid(51, 77, 85, 109);
		input.AddTrimid(51, 78, 86, 110);
		input.AddTrimid(52, 56, 79, 87);
		input.AddTrimid(52, 56, 79, 100);
		input.AddTrimid(52, 56, 80, 88);
		input.AddTrimid(52, 56, 80, 100);
		input.AddTrimid(52, 56, 87, 88);
		input.AddTrimid(52, 79, 87, 111);
		input.AddTrimid(52, 80, 88, 112);
		input.AddTrimid(53, 73, 81, 113);
		input.AddTrimid(53, 74, 82, 114);
		input.AddTrimid(54, 75, 83, 115);
		input.AddTrimid(54, 76, 84, 116);
		input.AddTrimid(55, 77, 85, 117);
		input.AddTrimid(55, 78, 86, 118);
		input.AddTrimid(56, 79, 87, 119);
		input.AddTrimid(56, 80, 88, 120);
		input.AddTrimid(57, 61, 73, 75);
		input.AddTrimid(57, 61, 73, 89);
		input.AddTrimid(57, 61, 75, 91);
		input.AddTrimid(57, 61, 89, 103);
		input.AddTrimid(57, 61, 91, 103);
		input.AddTrimid(57, 73, 89, 105);
		input.AddTrimid(57, 75, 91, 107);
		input.AddTrimid(58, 62, 74, 76);
		input.AddTrimid(58, 62, 74, 90);
		input.AddTrimid(58, 62, 76, 92);
		input.AddTrimid(58, 62, 90, 104);
		input.AddTrimid(58, 62, 92, 104);
		input.AddTrimid(58, 74, 90, 106);
		input.AddTrimid(58, 76, 92, 108);
		input.AddTrimid(59, 63, 77, 79);
		input.AddTrimid(59, 63, 77, 93);
		input.AddTrimid(59, 63, 79, 95);
		input.AddTrimid(59, 63, 93, 103);
		input.AddTrimid(59, 63, 95, 103);
		input.AddTrimid(59, 77, 93, 109);
		input.AddTrimid(59, 79, 95, 111);
		input.AddTrimid(60, 64, 78, 80);
		input.AddTrimid(60, 64, 78, 94);
		input.AddTrimid(60, 64, 80, 96);
		input.AddTrimid(60, 64, 94, 104);
		input.AddTrimid(60, 64, 96, 104);
		input.AddTrimid(60, 78, 94, 110);
		input.AddTrimid(60, 80, 96, 112);
		input.AddTrimid(61, 73, 89, 113);
		input.AddTrimid(61, 75, 91, 115);
		input.AddTrimid(62, 74, 90, 114);
		input.AddTrimid(62, 76, 92, 116);
		input.AddTrimid(63, 77, 93, 117);
		input.AddTrimid(63, 79, 95, 119);
		input.AddTrimid(64, 78, 94, 118);
		input.AddTrimid(64, 80, 96, 120);
		input.AddTrimid(65, 69, 81, 89);
		input.AddTrimid(65, 69, 81, 101);
		input.AddTrimid(65, 69, 85, 93);
		input.AddTrimid(65, 69, 85, 101);
		input.AddTrimid(65, 69, 89, 93);
		input.AddTrimid(65, 81, 89, 105);
		input.AddTrimid(65, 85, 93, 109);
		input.AddTrimid(66, 70, 82, 90);
		input.AddTrimid(66, 70, 82, 101);
		input.AddTrimid(66, 70, 86, 94);
		input.AddTrimid(66, 70, 86, 101);
		input.AddTrimid(66, 70, 90, 94);
		input.AddTrimid(66, 82, 90, 106);
		input.AddTrimid(66, 86, 94, 110);
		input.AddTrimid(67, 71, 83, 91);
		input.AddTrimid(67, 71, 83, 102);
		input.AddTrimid(67, 71, 87, 95);
		input.AddTrimid(67, 71, 87, 102);
		input.AddTrimid(67, 71, 91, 95);
		input.AddTrimid(67, 83, 91, 107);
		input.AddTrimid(67, 87, 95, 111);
		input.AddTrimid(68, 72, 84, 92);
		input.AddTrimid(68, 72, 84, 102);
		input.AddTrimid(68, 72, 88, 96);
		input.AddTrimid(68, 72, 88, 102);
		input.AddTrimid(68, 72, 92, 96);
		input.AddTrimid(68, 84, 92, 108);
		input.AddTrimid(68, 88, 96, 112);
		input.AddTrimid(69, 81, 89, 113);
		input.AddTrimid(69, 85, 93, 117);
		input.AddTrimid(70, 82, 90, 114);
		input.AddTrimid(70, 86, 94, 118);
		input.AddTrimid(71, 83, 91, 115);
		input.AddTrimid(71, 87, 95, 119);
		input.AddTrimid(72, 84, 92, 116);
		input.AddTrimid(72, 88, 96, 120);
		input.AddTrimid(73, 81, 89, 105);
		input.AddTrimid(73, 81, 89, 113);
		input.AddTrimid(74, 82, 90, 106);
		input.AddTrimid(74, 82, 90, 114);
		input.AddTrimid(75, 83, 91, 107);
		input.AddTrimid(75, 83, 91, 115);
		input.AddTrimid(76, 84, 92, 108);
		input.AddTrimid(76, 84, 92, 116);
		input.AddTrimid(77, 85, 93, 109);
		input.AddTrimid(77, 85, 93, 117);
		input.AddTrimid(78, 86, 94, 110);
		input.AddTrimid(78, 86, 94, 118);
		input.AddTrimid(79, 87, 95, 111);
		input.AddTrimid(79, 87, 95, 119);
		input.AddTrimid(80, 88, 96, 112);
		input.AddTrimid(80, 88, 96, 120);
		input.offset = 0;
		// clang-format on
	}

} // namespace Forth