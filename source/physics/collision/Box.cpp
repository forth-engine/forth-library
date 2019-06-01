#include "Box.h"

namespace Forth
{
	namespace Physics
	{
		Bounds4 Box::ComputeAABB(const Transform4 &tx)
		{
			return (tx * local) * Bounds4(extent);
		}

		bool Box::TestPoint(const Transform4 &tx, const Vector4 &p)
		{
			Transform4 world = tx * local;
			Vector4 p0 = p / world;

			for (int i = 0; i < 3; ++i)
			{
				if (Abs(p0[i]) > extent[i])
					return false;
			}
			return true;
		}

		bool Box::Raycast(RaycastHit4 &raycast)
		{
			Transform4 world = GetWorldTransform();
			Vector4 d = raycast.ray.direction / world.rotation;
			Vector4 p = raycast.ray.origin / world;
			float tmin = 0; //, tmax = raycast.t;

			// t = (e[ i ] - p.[ i ]) / d[ i ]
			float t0;
			Vector4 n0 = Vector4();

			for (int i = 0; i < 3; ++i)
			{
				// Check for ray parallel to and outside of Bounds4
				if (Abs(d[i]) < 1e-8)
				{
					// Detect separating axes
					if (Abs(p[i]) > extent[i])
						return false;
				}
				else
				{
					float d0 = 1 / d[i];
					float s = Sign(d[i]);
					float ei = extent[i] * s;
					Vector4 n = Vector4(i, -s);

					if ((t0 = (-ei - p[i]) * d0) > tmin)
					{
						n0 = n;
						tmin = t0;
					}
				}
			}

			raycast.collider = this;
			raycast.normal = world.rotation * n0;
			raycast.distance = tmin;

			return true;
		}

		void Box::ComputeMass(MassData &md)
		{
			// Calculate inertia tensor
			float mass = 8 * extent.x * extent.y * extent.z * extent.w * density;
			Vector4 e2 = (extent * extent) * 4;
			Euler4 e3 = Euler4(e2.y + e2.z, e2.x + e2.z, e2.x + e2.y, e2.w + e2.x, e2.w + e2.y, e2.w + e2.z);
			Euler4 e4 = e3 * (1.f / 12.f * mass);
			Tensor4 I = Tensor4(e4);

			md = MassData(I, local, mass);
		}

		Box::Box() : Shape(CSH_Box) {}
	} // namespace Physics
} // namespace Forth