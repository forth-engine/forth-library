#include "Collide.h"
#include "../dynamics/Contact.h"
#include "Algorithm.h"
#include "../dynamics/Body.h"

namespace Forth
{
	namespace Physics
	{
		//--------------------------------------------------------------------------------------------------

		void Collide::BoxToBox(Manifold &m, const Box &a, const Box &b)
		{
			Transform4 atx = a.GetWorldTransform();
			Transform4 btx = b.GetWorldTransform();
			Vector4 eA = a.extent, eB = b.extent, e;

			// B's frame input A's space
			Matrix4 E = btx.rotation / atx.rotation, C = Abs(E);
			Vector4 T = btx.position / atx;

			// Query states
			float s, S = FLT_MIN;
			Vector4 N = Vector4();
			int axis = -1;
			Matrix4 basis;

			// SAT from A
			for (int i = 0; i < 4; i++)
			{
				if ((s = Abs(T[i]) - (eA[i] + Dot(C[i], eB))) > S)
				{
					if ((S = s) > 0)
						return;
					N = atx.rotation.Column(i);
					axis = i;
				}
			}

			// SAT from B
			for (int i = 0; i < 4; i++)
			{
				if ((s = Abs(Dot(T, E.Column(i))) - (eB[i] + Dot(C.Column(i), eA))) > S)
				{
					if ((S = s) > 0)
						return;
					N = btx.rotation.Column(i);
					axis = i + 4;
				}
			}

			if (Dot(N, btx.position - atx.position) < 0)
				N *= -1;

			// Don't know why have to unflag this?
			if (axis >= 4)
			{
				std::swap(atx, btx);
				std::swap(eA, eB);
				N = -N;
			}

			// Compute reference and incident edge information necessary for clipping

			Algorithm::ComputeIncidentFace(btx, eB, N, Algorithm::incident);

			Algorithm::ComputeReferenceEdgesAndBasis(eA, atx, N, axis, basis, e);

			Algorithm::Clip(atx.position, e, basis, Algorithm::incident, m);

			m.normal = axis >= 4 ? -N : N;

			// if (m.contacts == 0)
			// 	Debug.LogWarning("SOME DEATH AT " + Time.frameCount);
		}

		//--------------------------------------------------------------------------------------------------

		void Collide::BoxToSphere(Manifold &m, const Box &a, const Sphere &b) //CHECK
		{
			Transform4 atx = a.GetWorldTransform();
			Transform4 btx = b.GetWorldTransform();

			Bounds4 eAABB = Bounds4(a.extent);

			float r2 = b.radius * b.radius, d2;

			// Vector4 from center A to center B relative to A's space
			Vector4 t = btx.position / atx;

			Vector4 n = eAABB.Clamp(t), d = t - n;

			if ((d2 = LengthSq(d)) > r2)
				return;

			m.MakeContact(atx.rotation * d, atx * n, r2 - d2);
		}

		//--------------------------------------------------------------------------------------------------

		void Collide::SphereToSphere(Manifold &m, const Sphere &a, const Sphere &b) //CHECK
		{
			// For simplicity we can ignore rotation and calculate directly in world space.

			Vector4 ap = a.body->Tx * a.local.position;
			Vector4 bp = b.body->Tx * b.local.position;

			Vector4 d = bp - ap;

			float r = a.radius + b.radius, r2 = r * r, d2;

			if ((d2 = LengthSq(d)) > r2)
				return;

			m.MakeContact(d, (ap + bp) * 0.5f, r2 - d2);
		}

		//--------------------------------------------------------------------------------------------------

		void Collide::BoxToCapsule(Manifold &m, const Box &a, const Capsule &b)
		{
			Transform4 atx = a.GetWorldTransform();
			Transform4 btx = b.GetWorldTransform();
			float r = b.radius, eB = b.extent;
			Vector4 eA = a.extent;

			// B's frame input A's space
			Matrix4 C = btx.rotation / atx.rotation;
			Vector4 T = btx.position / atx;

			// Query states
			float s, S = FLT_MIN;
			int axis = -1;
			Vector4 N = Vector4(), A, B, oA, oB;

			// SAT from A
			for (int i = 0; i < 4; i++)
			{
				if ((s = Abs(T[i]) - (eA[i] + (Abs(C.Column(i).y) * eB))) > S)
				{
					if ((S = s) > r)
						return;
					N = atx.rotation.Column(axis = i);
				}
			}

			if (Dot(N, btx.position - atx.position) < 0)
				N = -N;

			Plane4 H = Plane4(N, eA[axis]);

			auto p = Algorithm::EdgePlaneContact(A, B, H, oA = btx * Vector4(1, eB), oB = btx * Vector4(1, -eB));

			if (p)
			{
				m.MakeContact(atx.position + H.Project(oA), atx.position + oA - N * r);
				m.MakeContact(atx.position + H.Project(oB), atx.position + oB - N * r);
				m.normal = N;
			}
			else
			{
				m.MakeContact(N, atx.position + (A + B) * 0.5f, S);
			}
		}

		//--------------------------------------------------------------------------------------------------

		void Collide::SphereToCapsule(Manifold &m, const Sphere &a, const Capsule &b)
		{
			Transform4 atx = a.GetWorldTransform();
			Transform4 btx = b.GetWorldTransform();

			Vector4 P, Q;
			float rA = a.radius, rB = b.radius, d2;
			float r = rA + rB, eB = b.extent, r2 = r * r;

			Q = Algorithm::EdgeVertexContact(btx * Vector4(1, eB), btx * Vector4(1, -eB), P = atx.position);

			if ((d2 = LengthSq(Q - P)) > r2)
				return;

			m.MakeContact(Q - P, (P + Q) * 0.5f, r2 - d2);
		}

		//--------------------------------------------------------------------------------------------------

		void Collide::CapsuleToCapsule(Manifold &m, const Capsule &a, const Capsule &b) // CHECK
		{
			Transform4 atx = a.GetWorldTransform();
			Transform4 btx = b.GetWorldTransform();

			Vector4 oA = Vector4(0, a.extent, 0, 0), A;
			Vector4 oB = Vector4(0, b.extent, 0, 0), B;

			float r = a.radius + b.radius, r2 = r * r, d2;

			auto p = Algorithm::EdgesContact(A, B, atx * oA, atx * -oA, btx * oB, btx * -oB);

			if ((d2 = DistanceSq(A, B)) > r2)
				return;

			if (p)
			{
				// Two contact because capsules are parallel to each other
				m.MakeContact(atx * oA, btx * oB);
				m.MakeContact(atx * -oA, btx * -oB);
				m.normal = Normalize(B - A);
			}
			else
				m.MakeContact(B - A, (A + B) * 0.5f, r2 - d2);
		}

		//--------------------------------------------------------------------------------------------------

		void Collide::ComputeCollision(Manifold &m, const Shape &a, const Shape &b)
		{
			if (a.type > b.type)
			{
				ComputeCollision(m, b, a);
				m.normal *= -1.f;
				return;
			}

			if (a.type == CSH_Box)
			{
				if (b.type == CSH_Box)
					BoxToBox(m, *((Box *)&a), *((Box *)&b));
				else if (b.type == CSH_Sphere)
					BoxToSphere(m, *((Box *)&a), *((Sphere *)&b));
				else if (b.type == CSH_Capsule)
					BoxToCapsule(m, *((Box *)&a), *((Capsule *)&b));
			}
			else if (a.type == CSH_Sphere)
			{
				if (b.type == CSH_Sphere)
					SphereToSphere(m, *((Sphere *)&a), *((Sphere *)&b));
				else if (b.type == CSH_Capsule)
					SphereToCapsule(m, *((Sphere *)&a), *((Capsule *)&b));
			}
			else if (a.type == CSH_Capsule)
			{
				if (b.type == CSH_Capsule)
					CapsuleToCapsule(m, *((Capsule *)&a), *((Capsule *)&b));
			}
		}
	} // namespace Physics

} // namespace Forth
