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

/// <summary> Sequincing preset to define the order of indices </summary>
/// <remarks> It's recommended to choose based on the simplex type </remarks>
/// <seealso cref="Buffer4Extension.Sequence(Buffer4, SequenceMode)"/>
enum SequenceMode
{
	/// <summary> Individual Points: 0 - 1 - 2 - 3 ... n </summary>
	SQM_Points = 0,
	/// <summary> Individual Segments: 0,1 - 2,3 - 4,5 ... n-1,n </summary>
	SQM_Lines,
	/// <summary> Continous Segments: 0,1 - 1,2 - 2,3 ... n-1,n </summary>
	SQM_LineStrip,
	/// <summary> Branched Segments: 0,1 - 0,2 - 0,3 ... 0,n </summary>
	SQM_LineFan,
	/// <summary> Continous Cycled Segments: 0,1 - 1,2 - 2,3 ... n,0 </summary>
	SQM_LineLoop,
	/// <summary> Individual Triangles: 0,1,2 - 3,4,5 ... n-2,n-1,n </summary>
	SQM_Triangles,
	/// <summary> Continous Triangles: 0,1,2 - 1,2,3 ... n-2,n-1,n </summary>
	SQM_TriangleStrip,
	/// <summary> Branched Triangles: 0,1,2 - 0,2,3 ... 0,n-1,n </summary>
	SQM_TriangleFan,
	/// <summary> Individual Quads: 0,1,2,3 - 4,5,6,7 ... n-3,n-2,n-1,n </summary>
	SQM_Quads,
	/// <summary> Continous Quads: 0,1,3,2 - 2,3,5,4 ... n-3,n-2,n,n-1 </summary>
	SQM_QuadStrip,
	/// <summary> Branched Cycled Triangles: 0,1,2 - 0,2,3 ... 0,n,1 </summary>
	SQM_Polygon,
	/// <summary> Individual Trimids: 0,1,2,3 - 4,5,6,7 ... n-3,n-2,n-1,n </summary>
	SQM_Trimids,
	/// <summary> Continous Trimids: 0,1,2,3 - 1,2,3,4 ... n-3,n-2,n-1,n </summary>
	SQM_TrimidStrip,
	/// <summary> Branched Trimids: 0,1,2,3 - 0,4,5,6 ... 0,n-2,n-1,n </summary>
	SQM_TrimidFan,
	/// <summary> Branched Pyramids: 0,1,2,3,4 - 0,5,6,7,8 ... 0,n-3,n-2,n-1,n </summary>
	SQM_PyramidFan,
	/// <summary> Branched Prism: 2,3,0,5,4,1 - 6,7,0,9,8,1 ... n-3,n-2,0,n,n-1,1 </summary>
	SQM_PrismFan,
	/// <summary> Individual Cubes: 0,1,2,3,4 - 0,5,6,7,8 ... 0,n-3,n-2,n-1,n </summary>
	SQM_Cubes,
	/// <summary> Continous Cube: 0,1,2,3,4 - 0,5,6,7,8 ... 0,n-3,n-2,n-1,n </summary>
	SQM_CubeStrip,
};

/// <summary>
/// Is UV calculated?
/// </summary>
inline bool
HasUV(VertexProfiles v)
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
} // namespace Forth
