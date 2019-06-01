#include "Shape.h"

#include "Box.h"
#include "Capsule.h"
#include "Sphere.h"
#include "../dynamics/Body.h"
#include "../dynamics/RaycastHit4.h"
#include "../dynamics/Scene.h"
#include <random>

namespace Forth
{
	namespace Physics
	{
		MassData::MassData(const Tensor4& I, const Transform4& local, float mass)
		{
			// Transform4 Inertia based on local space

			center = local.position;
			inertia = Transform(I, local, mass);
			this->mass = mass;
		}

		void Shape::CheckHash()
		{
			if (body == NULL || body->scene == NULL)
				throw;

			auto &hashes = body->scene->shapes;
			if (hashes.count(hash))
				return;
			if (hash == 0)
			{
				if (hashes.size() >= INT16_MAX - 1)
					throw "Dude. Too much objects!";

				do
					hash = ::std::rand();
				while (hashes.count(hash) || hash == 0);

				hashes[hash] = this;
			}
			else
			{
				// Something wrong?
				throw;
			}
		}

		void Shape::RemoveHash()
		{
			if (body == NULL || body->scene == NULL)
				throw;
			auto &hashes = body->scene->shapes;
			if (hashes.count(hash))
			{
				hashes.erase(hash);
				hash = 0;
			}
		}

		Transform4 Shape::GetWorldTransform() const
		{
			return (body->flags & BFL_Identity) > 0 ? body->Tx : body->Tx * local;
		}

		Shape *Shape::CreateShape(ShapeType type)
		{
			switch (type)
			{
			case CSH_Box:
				return new Box();
			case CSH_Sphere:
				return new Sphere();
			case CSH_Capsule:
				return new Capsule();
			default:
				throw;
			}
		}
	} // namespace Physics
} // namespace Forth