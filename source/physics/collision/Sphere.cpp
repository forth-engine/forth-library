#include "Sphere.h"

namespace Forth
{
	namespace Physics
	{

		Bounds4 Sphere::ComputeAABB(const Transform4 &tx)
		{
			Transform4 world = tx * local;
			Vector4 axis = Vector4(radius);
			return Bounds4(world.position - axis, world.position + axis);
		}

		void Sphere::ComputeMass(MassData &md)
		{
			// Calculate inertia tensor

			float mass = 4 / 3.f * PI * radius * radius * radius * density;
			float i = 2 / 5.f * mass * radius * radius;
			Tensor4 I = Tensor4(i);

			// Transform4 tensor to local space
			md = MassData(I, local, mass);
		}

		bool Sphere::Raycast(RaycastHit4 &raycast)
		{
			Transform4 world = GetWorldTransform();
			Vector4 d = raycast.ray.direction / world.rotation;
			Vector4 p = raycast.ray.origin / world;

			float tca = Dot(p, d);
			// If this sphere behind the ray
			if (tca < 0)
				return false;

			float tcd = Dot(p, p) - tca * tca;
			// If this ray completely miss it
			if (tcd > radius * radius)
				return false;

			// 100% chance of hit. Calculate now.
			float thc = Sqrt(radius * radius - tcd);
			float toi = tca - thc;

			raycast.Set(toi, Normalize(p + d * toi), this);

			return true;
		}

		bool Sphere::TestPoint(const Transform4 &tx, const Vector4 &p)
		{
			return LengthSq(p / (tx * local)) <= radius * radius;
		}

	} // namespace Forth
} // namespace Forth