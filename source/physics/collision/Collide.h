#pragma once
#include "../Common.h"
#include "Box.h"
#include "Sphere.h"
#include "Capsule.h"

namespace Forth {
	namespace Physics
	{
		struct Collide
		{

			//--------------------------------------------------------------------------------------------------
			static void BoxToBox(class Manifold& m, const Box& a, const Box& b);

			//--------------------------------------------------------------------------------------------------
			static void BoxToSphere(class Manifold& m, const Box& a, const Sphere& b);

			//--------------------------------------------------------------------------------------------------
			static void SphereToSphere(class Manifold& m, const Sphere& a, const Sphere& b);

			//--------------------------------------------------------------------------------------------------
			static void BoxToCapsule(class Manifold& m, const Box& a, const Capsule& b);

			//--------------------------------------------------------------------------------------------------
			static void SphereToCapsule(class Manifold& m, const Sphere& a, const Capsule& b);

			//--------------------------------------------------------------------------------------------------
			static void CapsuleToCapsule(class Manifold& m, const Capsule& a, const Capsule& b);

			//--------------------------------------------------------------------------------------------------
			static void ComputeCollision(class Manifold &m, const Shape& a, const Shape& b);

		};
	}
}
