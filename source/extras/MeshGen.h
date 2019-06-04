
#pragma once

#include "../common/Buffer4.h"
#include "../math/Transform4.h"

namespace Forth
{
	struct MeshGen
	{

		// Golden ratio and Square roots
		static const float _GR;
		static const float _SQRT3;
		static const float _SQRT5;
		static const float _SQRT6;
		static const float _SQRT10;

		static void MakeHyperplane(Buffer4 &input, float scale = 1);

		static const Vector4 _pentatopeVertices[5];

		static void MakePentatope(Buffer4 &input, float scale = 1);

		/// <summary>
		/// Make tesseract.
		/// </summary>
		static void MakeTesseract(Buffer4 &input, float scale = 1);

		static void MakeHexdecahedroid(Buffer4 &input, float scale = 1);

		// Dual pyramid spec: a & b is the pole, the rest is the meridian
		static void AddDualPyramid(Buffer4 &input, int a, int b, int c, int d, int e, int f);

		static void MakeIcosatetrahedroid(Buffer4 &input, float scale = 1);

		// Building block for primitive dodecahedron
		// The order follows: 5 top, 5 middle up, 5 middle low, 5 bottom. CCW from bird view
		static void AddDedocahedron(Buffer4 &input, int a, int b, int c, int d, int e, int f, int g, int h,
									int i, int j, int k, int l, int m, int n, int o, int p, int q, int r, int s, int t);
		/// <summary>
		/// Give sign to any nonzero vector elements
		/// </summary>
		static Vector4 GiveSign(Vector4 v, float x, float y);

		/// <summary>
		/// Give sign to any nonzero vector elements
		/// </summary>
		static Vector4 GiveSign(Vector4 v, float x, float y, float z);

		/// <summary>
		/// Give sign to any nonzero vector elements
		/// </summary>
		static Vector4 GiveSign(Vector4 &v, float x, float y, float z, float w);

		static const Vector4 _cell120Vertices[6];

		static const float _cell120Vertices2[3][2];

		static const Vector4 _cell120Vertices3[24];

		static const Vector4 _cell120Vertices4[12];

		static const Vector4 _hexacosidedroidVertices[12];

		static void MakeHecatonicosahedroid(Buffer4 &input, float scale = 1);

		static void MakeHexacosidedroid(Buffer4 &input, float scale = 1);

		struct HypersphereState
		{
			int subdivision;
			float radius;
			Vector4 extent;
			bool useedge;
			int subedge;
			int subdiv;
			float subinv;
		};

		static Vector4 GetCoordinate(float lon, float lat, float azm)
		{

			float clat = cos(lat), slat = sin(lat), clon = cos(lon),
				  slon = sin(lon), cazm = cos(azm), sazm = sin(azm);

			// Spherical coordinate upgraded in 4D according to wiki
			return Vector4(sazm * slat * slon, sazm * clat, sazm * slat * clon, cazm);
		}

		static void LoopMethod(Buffer4 &input, HypersphereState &state, float x, float y, float z)
		{
			Vector4 C = GetCoordinate(x, y, z);
			input.AddVertex(C * state.radius + state.extent * Sign(C));
		}

		static void LoopAzimuth(Buffer4 &input, HypersphereState &state, float x, float y)
		{
			for (int z = 0; z <= state.subdiv; z++) // Loop until pi
			{
				if (state.useedge && z % state.subedge == 0) // Every 90 deg
				{
					LoopMethod(input, state, x, y, z * state.subinv - EPSILON);
					LoopMethod(input, state, x, y, z * state.subinv + EPSILON);
				}
				else
					LoopMethod(input, state, x, y, z * state.subinv);
			}
		}

		static void LoopLatitude(Buffer4 &input, HypersphereState &state, float x)
		{
			for (int y = 0; y <= state.subdiv; y++) // Loop until pi
			{
				if (state.useedge && y % state.subedge == 0) // Every 90 deg
				{
					LoopAzimuth(input, state, x, y * state.subinv - EPSILON);
					LoopAzimuth(input, state, x, y * state.subinv + EPSILON);
				}
				else
					LoopAzimuth(input, state, x, y * state.subinv);
			}
		}

		static void MakeHypersphere(Buffer4 &input, int subdivision, float radius = 1, const Vector4 &extent = Vector4())
		{
			HypersphereState state;
			state.radius = radius;
			state.subdivision = subdivision;
			state.extent = extent;
			state.subdiv = subdivision * 2;			  // Real sphere subdivision (it's always even)
			state.subinv = PI / state.subdiv;		  // Angle advancement ratio
			state.subedge = subdivision;			  // Threshhold to signal extent cases
			state.useedge = LengthSq(extent) > 1e-4f; // Do we need to add more verts for extents?

			for (int x = 0; x <= state.subdiv * 2; x++) // Loop until 2*pi
			{
				if (state.useedge && x % state.subedge == 0) // Every 90 deg
				{
					LoopLatitude(input, state, x * state.subinv - EPSILON);
					LoopLatitude(input, state, x * state.subinv + EPSILON);
				}
				else
					LoopLatitude(input, state, x * state.subinv);
			}

			if (state.useedge)
				input.SequenceGrid(state.subdiv * 2 + 6, state.subdiv + 4, state.subdiv + 4);
			else
				input.SequenceGrid(state.subdiv * 2 + 1, state.subdiv + 1, state.subdiv + 1);
		}

		static void Transform(Buffer4 &input, const Transform4& transform, bool realign = true)
		{
			for(int i = input.verticeCount; i-- > input.offset;)
			{
				input.vertices[i] = transform * input.vertices[i];
			}

			if (realign)
			{
				input.Align();
			}
		}
	};
} // namespace Forth