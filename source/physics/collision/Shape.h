#pragma once

#include "../Common.h"

namespace Forth
{
	namespace Physics
	{

		struct MassData
		{
			Tensor4 inertia;
			Vector4 center;
			float mass;

			MassData() : inertia(Tensor4()), center(Vector4()), mass(0) {}

			MassData(const Tensor4& I, const Transform4& local, float mass);
		};

		enum ShapeType
		{
			CSH_Box = 0,
			CSH_Sphere = 1,
			CSH_Capsule = 2,
		};

		class Shape
		{
		  public:
			  Shape() {}
			Shape(ShapeType type)
			{
				this->type = type;
			}

			class Body* body = NULL;

			int hash = 0;

			int broadPhaseIndex = 0;

			ShapeType type;

			float density = 1.f;

			bool sensor = false;

			float friction = 0.2f;

			float restitution = 0.1f;

			Transform4 local = Transform4::identity;

			virtual bool TestPoint(const Transform4& tx, const Vector4& p) = 0;

			virtual bool Raycast(struct RaycastHit4 &raycast) = 0;

			virtual Bounds4 ComputeAABB(const Transform4& tx) = 0;

			virtual void ComputeMass(MassData& md) = 0;

			void CheckHash();

			void RemoveHash();

			Transform4 GetWorldTransform() const;

			// -- Public accessor

			void *Tag = NULL;

			static Shape* CreateShape(ShapeType type);
		};

	} // namespace Physics
} // namespace Forth