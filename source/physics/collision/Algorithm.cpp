#include "Algorithm.h"
#include "../dynamics/Contact.h"

namespace Forth
{
	namespace Physics
	{
		Vector4 Algorithm::input[Common::MULTICONTACT_COUNT];
		Vector4 Algorithm::result[Common::MULTICONTACT_COUNT];
		Vector4 Algorithm::incident[8];

		//--------------------------------------------------------------------------------------------------
		// Get nearest point from two edges, return true if parallel

		bool Algorithm::EdgesContact(Vector4 &CA, Vector4 &CB, const Vector4 &PA, const Vector4 &QA, const Vector4 &PB, const Vector4 &QB)
		{
			Vector4 DA = QA - PA;
			Vector4 DB = QB - PB;
			Vector4 r = PA - PB;
			float a = Dot(DA, DA);
			float e = Dot(DB, DB);
			float f = Dot(DB, r);
			float c = Dot(DA, r);

			float b = Dot(DA, DB);
			float denom = a * e - b * b;

			float TA, TB;
			if (denom < 1e-4)
			{
				// Parallel, get another idea to do it.
				auto M = (PA + PB + QA + QB) / 4;
				TA = Clamp01(Dot(M - PA, DA) / (a + 1e-5f));
				TB = Clamp01(Dot(M - PB, DB) / (e + 1e-5f));
			}
			else
			{
				TA = Clamp01((b * f - c * e) / denom);
				TB = Clamp01((b * TA + f) / e);
			}

			CA = PA + DA * TA;
			CB = PB + DB * TB;

			return denom < 1e-4;
		}

		bool Algorithm::EdgePlaneContact(Vector4 &CA, Vector4 &CB, Plane4 HA, const Vector4 &PB, const Vector4 &QB)
		{
			Vector4 u = QB - PB;
			Vector4 w = PB - HA.origin();
			float D = Dot(HA.normal, u);
			float N = -Dot(HA.normal, w);

			if (D * D < 1e-4f)
			{
				// Parallel, get another idea to do it.
				CB = CA = HA.origin();
				return true;
			}
			else
			{
				CB = PB + u * Clamp01(N / D);
				CA = HA.Project(CB);
				return false;
			}
		}

		Vector4 Algorithm::EdgeVertexContact(const Vector4 &A, const Vector4 &B, const Vector4 &P)
		{
			auto AB = B - A;
			auto D = Dot(P - A, AB) / Dot(AB, AB);
			return A + AB * Clamp01(D);
		}

		//--------------------------------------------------------------------------------------------------
		// Return (W-axis) oriented box extent & rotation necessary for clipping

		void Algorithm::ComputeReferenceEdgesAndBasis(const Vector4 &eR, const Transform4 &rtx, const Vector4 &n, int axis, Matrix4 &basis, Vector4 &e)
		{
			Vector4 nr = n / rtx.rotation;

			axis = axis % 4;

			//{
			//    e = Vector4();
			//    e[0] = eR[(axis + 1) % 4];
			//    e[1] = eR[(axis + 2) % 4];
			//    e[2] = eR[(axis + 3) % 4];
			//    e[3] = eR[axis];
			//}
			//{
			//    basis = Matrix4();
			//    basis.Column0 = rtx.rotation.Column((axis + 1) % 4);
			//    basis.Column1 = rtx.rotation.Column((axis + 2) % 4);
			//    basis.Column2 = rtx.rotation.Column((axis + 3) % 4);
			//    basis.Column3 = rtx.rotation.Column(axis);
			//    if (Sign(n[axis]) < 0)
			//        basis = Matrix4.zero - basis;
			//}
			auto rotate = Transpose(LookAt(/*Vector4(axis, Sign(n[axis]))*/ nr));
			basis = rotate * rtx.rotation;
			e = Abs(rotate * eR);
		}

		//--------------------------------------------------------------------------------------------------
		// Axis-aligned clipping algo

		int Algorithm::Orthographic(float e, int axis, Vector4 input[], int length, Vector4 result[])
		{
			int count = 0;

			for (int i = 0; i < length; ++i)
			{
				Vector4 a = input[i], b = input[(i + 1) % length];

				float xa = a[axis], xb = b[axis], da = Abs(xa) - e, db = Abs(xb) - e;

				if (da < 0 && db < 0)
					result[count++] = b;
				else if ((da < 0) ^ (db < 0))
				{
					// Intersection point between A and B
					auto cv = a + (b - a) * (da / (da - db));

					if (da < 0)
						result[count++] = cv;
					else
					{
						result[count++] = cv;
						result[count++] = b;
					}
				}
				//else if (xa * xb < 0)
				//{
				//    result[count++] = a + (b - a) * ((e - xa) / (da - db));
				//    result[count++] = a + (b - a) * ((-e - xa) / (da - db));
				//}
			}

			return count;
		}

		//--------------------------------------------------------------------------------------------------
		// Get the most potential face candidate that Incidents N
		// That is: The face that most parallel to N

		void Algorithm::ComputeIncidentFace(const Transform4 &itx, const Vector4 &e, const Vector4 &n, Vector4 result[])
		{
			auto ni = (n / itx.rotation);

			int i = MaxPerElemIdx(Abs(ni)), k = 0;

			for (int x = -1; x <= 1; x += 2)
				for (int y = -1; y <= 1; y += 2)
					for (int z = -1; z <= 1; z += 2)
					{
						auto r = e;
						r[i] *= -Sign(ni[i]);
						r[(i + 1) % 4] *= x;
						r[(i + 2) % 4] *= y;
						r[(i + 3) % 4] *= z;
						result[k++] = itx * r;
					}
		}

		//--------------------------------------------------------------------------------------------------
		// Axis-aligned clipping algo

		//--------------------------------------------------------------------------------------------------
		// Four axis clipping via (an iterative algorithm) sutherland hodgman clipping
		// http://www.randygaul.net/2014/10/27/sutherland-hodgman-clipping/

		void Algorithm::Clip(const Vector4 &rPos, const Vector4 &e, Matrix4 basis, Vector4 incident[], Manifold m)
		{
			int count = 8;
			float d;

			for (int i = 0; i < 8; ++i)
				input[i] = (incident[i] - rPos) / basis;

			// Clip extent X
			if ((count = Orthographic(e.x, 0, input, count, result)) == 0)
				return;

			// Clip extent Y
			if ((count = Orthographic(e.y, 1, result, count, input)) == 0)
				return;

			// Clip extent Z
			if ((count = Orthographic(e.z, 2, input, count, result)) == 0)
				return;

			for (int i = 0; i < count; ++i)
			{
				// Clip extent W
				if ((d = result[i].w - e.w) < 0.f)
				{
					m.MakeContact(basis * result[i] + rPos, d);
				}
			}
		}
	} // namespace Physics
} // namespace Forth