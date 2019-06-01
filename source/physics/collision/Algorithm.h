#include "../Common.h"

namespace Forth { namespace Physics
{
    // Common geometry algorithms
    struct Algorithm
    {

        //--------------------------------------------------------------------------------------------------
        // Get nearest point from two edges, return true if parallel
		static bool EdgesContact(Vector4& CA, Vector4& CB, const Vector4& PA, const Vector4& QA, const Vector4& PB, const Vector4& QB);

		static bool EdgePlaneContact(Vector4& CA, Vector4& CB, Plane4 HA, const Vector4& PB, const Vector4& QB);

		static Vector4 EdgeVertexContact(const Vector4& A, const Vector4& B, const Vector4& P);

        /* Convex collision algorithm - soon
       //--------------------------------------------------------------------------------------------------
       // Get nearest point along the edge


       static Vector4 EdgeVertexContact(const Vector4& A, const Vector4& B, const Vector4& P, out int I)
       {
           auto AB = B - A;
           auto D = Dot(P - A, AB) / Dot(AB, AB);

           I = (D < 0 || D > 1) ? 1 : 2;
           return A + Clamp01(D) * (AB);
       }



       //--------------------------------------------------------------------------------------------------
       // Get nearest point along the triangle

       static Vector4 TriangleVertexContact(const Vector4& A, const Vector4& B, const Vector4& C, const Vector4& P)
       {
           int t;
           return TriangleVertexContact(A, B, C, P, out t);
       }

       static Vector4 TriangleVertexContact(const Vector4& A, const Vector4& B, const Vector4& C, const Vector4& P, int& I)
       {
           auto plane = Plane4(A, B, C);

           auto Q = plane.Project(P);

           Vector4 AB = B - A, AC = A - C, QA = A - Q, QB = B - Q, QC = C - Q;

           auto ABC = Cross(AB, AC);

           auto u = Dot(Cross(QC, QB), ABC);
           auto v = Dot(Cross(QA, QC), ABC);
           auto w = Dot(Cross(QB, QA), ABC);

           // auto d = Dot(ABC, ABC);
           // auto uvw = u + v + w;
           // Assert(Abs(uvw / d - 1) < 1e-4);

           if (u >= 0 && v >= 0 && w >= 0)
           {
               // Inside the triangle
               I = 4;
               return Q;
           }
           else if (u < 0 ^ v < 0 ^ w < 0)
           {
               // Edge area
               I = 2;
               if (u < 0) return EdgeVertexContact(B, C, Q);
               if (v < 0) return EdgeVertexContact(C, A, Q);
               if (w < 0) return EdgeVertexContact(A, B, Q);
           }
           else
           {
               // Corner area
               I = 1;
               if (u >= 0) return A;
               if (v >= 0) return B;
               if (w >= 0) return C;
           }
           return Q;
       }

       static int VertexIndex(Vector4[] Vs, const Vector4& V)
       {
           for (int i = 0; i < Vs.Length; i++)
           {
               if (DistanceSq(Vs[i], V) < 1e-4)
                   return i;
           }
           Assert(false);
           return -1;
       }

       //--------------------------------------------------------------------------------------------------
       // Get nearest point inside convex shape
       static Vector4 ConvexVertexContact(Vector4[] Vs, const Vector4& Q)
       {
           auto Vb = SetPool<int>.Pop(); // Blacklist data, use Set for speedup
           auto V = ListPool<VertexHull>.Pop(); // Convex vertices data
           auto v = ConvexVertexContact(Vs, Q, Vb, V);
           SetPool<int>.Push(Vb);
           ListPool<VertexHull>.Push(V);
           return v;
       }

       static Vector4 ConvexVertexContact(Vector4[] Vs, const Vector4& Q, Set<int> Vb, ::std::vector<VertexHull> V)
       {
           Assert(Vs.Length > 2);

           // Setup the instrument
           Vector4 Vc = Vector4(), Vr = Vector4();
           Plane4 H;
           int Ti;

           // Get median (don't have to be median, but must be a point inside convex)
           for (int i = 0; i < Vs.Length; i++)
           {
               Vc += Vs[i];
           }

           Vc /= Vs.Length;
           H = Plane4(Vc - Q, Q);

           // Arcquire list of vertex data
           for (int i = 0; i < Vs.Length; i++)
           {
               V.Add(VertexHull(Vr = Vs[i], i, H.Distance(Vs[i])));

           }

           // From this point, no arithmatic calculation necessary,
           // Just some magic algorithms. Lets' get dizzy!

           // -- Points on faces

           do
           {

               int A = -1, B = -1, C = -1;
               float dA = float.MaxValue, dB = float.MaxValue, dC = float.MaxValue;

               // Get three nearest vertices
               for (int i = 0; i < Vs.Length; i++)
               {
                   if (Vb.Contains(i)) continue;

                   auto dV = V[i].distance;

                   if (dV < dA) { Chain(i, ref A, ref B, ref C); Chain(dV, ref dA, ref dB, ref dC); }
                   else if (dV < dB) { Chain(i, ref B, ref C); Chain(dV, ref dB, ref dC); }
                   else if (dV < dC) { C = i; dC = dV; }
               }

               // Get the magic
               Vr = TriangleVertexContact(V[A].vector, V[B].vector, V[C].vector, Q, out Ti);

               // R must be parallel to triangle in order to be 'valid'
               if (Ti == 4)
                   return Vr;
               else
               {
                   // One of its vertex must be invalid. (get the furthest)
                   if (dA > dB && dA > dC)
                       Vb.Add(V[A].index);
                   else if (dB > dC)
                       Vb.Add(V[B].index);
                   else
                       Vb.Add(V[C].index);
               }

           } while (Vb.size() + 2 < Vs.Length);

           // -- Points on edges

           Vb.Clear();

           do
           {

               int A = -1, B = -1;
               float dA = float.MaxValue, dB = float.MaxValue;

               // Get two nearest vertices
               for (int i = 0; i < Vs.Length; i++)
               {
                   if (Vb.Contains(i)) continue;

                   auto dV = V[i].distance;

                   if (dV < dA) { Chain(i, ref A, ref B); Chain(dV, ref dA, ref dB); }
                   else if (dV < dB) { B = i; dB = dV; }
               }

               // Get the magic
               Vr = EdgeVertexContact(V[A].vector, V[B].vector, Q, out Ti);

               // R must be parallel to edge in order to be 'valid'
               if (Ti == 2)
                   return Vr;
               else
               {
                   // One of its vertex must be invalid. (get the furthest)
                   if (dA > dB)
                       Vb.Add(V[A].index);
                   else
                       Vb.Add(V[B].index);
               }

           } while (Vb.size() + 1 < Vs.Length);

           // -- Point on corner. Just find the nearest point!

           {

               int A = -1;
               float dA = float.MaxValue;

               // Get two nearest vertices
               for (int i = 0; i < Vs.Length; i++)
               {
                   auto dV = V[i].distance;

                   if (dV < dA) { A = i; dA = dV; }
               }

               // Get the magic
               return V[A].vector;
           }
       }

       //--------------------------------------------------------------------------------------------------
       // Get nearest point between edge and convex shape
       static void ConvexEdgeContact(Vector4& CV, Vector4& CP, Vector4[] Vs, const Vector4& PA, const Vector4& PB)
       {
           Vector4 A = ConvexVertexContact(Vs, PA);
           Vector4 B = ConvexVertexContact(Vs, PB);
           EdgesContact(out CV, out CP, A, B, PA, PB);
       }

       static bool IsItValidMatches(const Vector4& Q, const Vector4& Vc, const Vector4& A, const Vector4& B, const Vector4& C)
       {
           auto h = Plane4(A, B, C);
           return h.IsSameSide(Q, Vc);
       }

       static bool IsItValidMatches(const Vector4& Q, const Vector4& Vc, const Vector4& A, const Vector4& B)
       {
           auto h = Plane4(A, B, Cross(A - B, Vc - B));
           return h.IsSameSide(Q, Vc);
       }

       //static Vector4[] MinkowskiSubtract

       // Shared array instance, hence no parallel support
       static Vector4[] cubeAABB = Vector4[8];

       static Vector4[] AABBToConvex(const Vector4& extent)
       {
           for (int i = 0; i < 8; i++)
               cubeAABB[i] = Bounds4.AABBVertices[i] * extent;

           return cubeAABB;
       }

       static Vector4[] AABBToConvex(Bounds4 aabb)
       {
           Vector4 c = aabb.Center, e = aabb.Extent;
           for (int i = 0; i < 8; i++)
               cubeAABB[i] = Bounds4.AABBVertices[i] * e + c;

           return cubeAABB;
       }

       static Vector4[] OBBToConvex(OBB obb)
       {
           return OBBToConvex(obb, true);
       }

       static Vector4[] OBBToConvex(OBB obb, bool calculatePosition)
       {
           auto tx = obb.Tx;
           if (!calculatePosition)
               tx.position = zero;
           for (int i = 0; i < 8; i++)
               cubeAABB[i] = tx * (Bounds4.AABBVertices[i] * obb.E);

           return cubeAABB.Clone() as Vector4[];
       }

       struct VertexHull
       {
           Vector4 vector;
           int index;
           float distance;

           VertexHull(const Vector4& v, int i, float d)
           {
               vector = v;
               index = i;
               distance = d;
           }
       }

       */

        //--------------------------------------------------------------------------------------------------
        // Return (W-axis) oriented box extent & rotation necessary for clipping
static void ComputeReferenceEdgesAndBasis(const Vector4& eR, const Transform4& rtx, const Vector4& n, int axis, Matrix4& basis, Vector4& e);

        //--------------------------------------------------------------------------------------------------
        // Get the most potential face candidate that Incidents N
        // That is: The face that most parallel to N
static void ComputeIncidentFace(const Transform4& itx, const Vector4& e, const Vector4& n, Vector4 result[]);

        //--------------------------------------------------------------------------------------------------
        // Axis-aligned clipping algo
static int Orthographic(float e, int axis, Vector4 input[], int length, Vector4 result[]);

        static Vector4 input[Common::MULTICONTACT_COUNT];
        static Vector4 result[Common::MULTICONTACT_COUNT];
        static Vector4 incident[8];

        //--------------------------------------------------------------------------------------------------
        // Four axis clipping via (an iterative algorithm) sutherland hodgman clipping
        // http://www.randygaul.net/2014/10/27/sutherland-hodgman-clipping/
		static void Clip(const Vector4& rPos, const Vector4& e, Matrix4 basis, Vector4 incident[], class Manifold m);
	};
 }}
