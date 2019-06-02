#pragma once

#include "../dynamics/RaycastHit4.h"
#include "Shape.h"

namespace Forth
{
	namespace Physics
	{

		class Box : public Shape
		{
		  public:
			Vector4 extent = Vector4(0.5f);

			Bounds4 ComputeAABB(const Transform4 &tx);

			bool TestPoint(const Transform4 &tx, const Vector4 &p);

			bool Raycast(RaycastHit4 &raycast);

			void ComputeMass(MassData &md);

			// -- Public accessor

			Box();
		};

	} // namespace Physics
} // namespace Forth