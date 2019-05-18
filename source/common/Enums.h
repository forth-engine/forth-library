#pragma once

namespace Forth
{
	/// <summary> Additional channels to be included when generating the mesh output </summary>
	enum VertexProfiles
	{
		/// <summary> Only generate vertices and indices </summary>
		VP_None = 0,
		/// <summary> Generate first UV </summary>
		VP_UV = 1,
		/// <summary> Generate first, second UV </summary>
		VP_UV2 = 2 | 1,
		/// <summary> Generate first, second, and third UV </summary>
		VP_UV3 = 4 | 2 | 1,
		/// <summary> Generate Color </summary>
		VP_Color = 8,
		/// <summary> Generate Color and first UV </summary>
		VP_ColorAndUV = 8 | 1,
		/// <summary> Generate Color and first, second UV </summary>
		VP_ColorAndUV2 = 8 | 1 | 2,
		/// <summary> Generate Color and first, secord, third UV </summary>
		VP_ColorAndUV3 = 8 | 1 | 2 | 4,
	};

	/// <summary> Simplex contain points. Simplex count must be divisible by 1 </summary>
	enum SimplexMode
	{
		/// <summary> Simplex contain points. Simplex count must be divisible by 1 </summary>
		SM_Point = 0,
		/// <summary> Simplex contain line segments. Simplex count must be divisible by 2 </summary>
		SM_Line = 1,
		/// <summary> Simplex contain flat triangles. Simplex count must be divisible by 3 </summary>
		SM_Triangle = 2,
		/// <summary> Simplex contain volume tetrahedrons. Simplex count must be divisible by 4 </summary>
		SM_Tetrahedron = 3,
	};

	/// <summary> What kind of visualization need to be shown? </summary>
	enum VisualizeMode
	{
		/// <summary> Visualize using mesh points </summary>
		VM_Particle = 0,
		/// <summary> Visualize using mesh wires (wireframe) </summary>
		VM_Wire = 1,
		/// <summary> Visualize using mesh triangles </summary>
		VM_Solid = 2,
		/// <summary> Use custom visualizer attached in the game object </summary>
		VM_Custom = 3,
	};

	/// <summary> Space relatives for transformations </summary>
	enum Space4
	{
		/// <summary> Transform is applied from the local coordinate of an object </summary>
		SP_Self,
		/// <summary> Transform is applied from the four-dimensional world coordinate of an object </summary>
		SP_World,
		/// <summary> Transform is applied from the projected three-dimensional world coordinate of an object </summary>
		SP_View,
	};

	/// <summary>
	/// Is UV calculated?
	/// </summary>
	inline bool HasUV(VertexProfiles v)
	{
		return ((int)v & 1) > 0;
	}

	/// <summary>
	/// Is UV2 calculated?
	/// </summary>
	inline bool HasUV2(VertexProfiles v)
	{
		return ((int)v & 2) > 0;
	}

	/// <summary>
	/// Is UV3 calculated?
	/// </summary>
	inline bool HasUV3(VertexProfiles v)
	{
		return ((int)v & 4) > 0;
	}

	/// <summary>
	/// Is Color calculated?
	/// </summary>
	inline bool HasColor(VertexProfiles v)
	{
		return ((int)v & 8) > 0;
	}
}
