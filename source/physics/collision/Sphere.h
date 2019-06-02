#pragma once

#include "Shape.h"
#include "../dynamics/RaycastHit4.h"

namespace Forth
{
	namespace Physics
	{
		class Sphere : public Shape
		{
		  public:
			float radius = 0.5f;

			Bounds4 ComputeAABB(const Transform4 &tx);

			void ComputeMass(MassData &md);

			bool Raycast(RaycastHit4 &raycast);

			bool TestPoint(const Transform4 &tx, const Vector4 &p);

			// -- Public accessor

			Sphere() : Shape(CSH_Sphere) {}

			Sphere(float radius) : radius(radius), Shape(CSH_Sphere) {}

			//float Radius{ get { return radius; } set { radius = value; if (body != NULL) body.flags |= BFL_DirtyMass; } }
		};
	} // namespace Physics
} // namespace Forth
