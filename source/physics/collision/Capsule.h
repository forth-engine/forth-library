#pragma once

#include "Shape.h"
#include "../dynamics/RaycastHit4.h"

namespace Forth
{
	namespace Physics
	{

		// The capsule uses Y as the polar caps
		class Capsule : public Shape
		{
		public:
			float extent = 1;

			float radius = 0.5f;

			Bounds4 ComputeAABB(const Transform4& tx);

			void ComputeMass(MassData& md);

			bool Raycast(RaycastHit4& raycast);

			bool TestPoint(const Transform4& tx, const Vector4& p);

			// -- Public accessor

			Capsule() : Shape(CSH_Capsule) {}
		};
	} // namespace Physics
} // namespace Forth