#include "Capsule.h"

namespace Forth
{
	namespace Physics
	{
		Bounds4 Capsule::ComputeAABB(const Transform4 &tx)
		{
			Transform4 world = tx * local;
			Vector4 axis = Vector4(radius, extent + radius, radius, radius);
			return world * Bounds4(axis);
		}

		void Capsule::ComputeMass(MassData &md)
		{

			float mass = density * radius * radius * PI * ((4 / 3.f * radius) + (extent * 2));

			float r = mass * ((21 / 20.f) * radius * radius + (6 / 4.f) * radius * extent + (4 / 3.f) * extent * extent);

			float h = 13 / 10 * mass * radius * radius;

			Tensor4 I = Tensor4(Euler4(r, h, r, h, r, h));

			md = MassData(I, local, mass);
		}

		bool Capsule::Raycast(RaycastHit4 &raycast)
		{
			throw;
		}

		bool Capsule::TestPoint(const Transform4 &tx, const Vector4 &p)
		{
			Vector4 p0 = p / (tx * local);

			p0.y = Abs(p.y) >= extent ? p.y - Sign(p.y) * extent : 0;

			return LengthSq(p0) <= radius * radius;
		}
	} // namespace Physics
} // namespace Forth