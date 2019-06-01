#include "Body.h"
#include "Scene.h"

namespace Forth
{
	namespace Physics
	{
		void Body::CalculateMassData()
		{

			SynchronizeProxies();

			Tensor4 inertia = Tensor4::zero();
			Vector4 lc = Vector4::zero();
			invMass = mass = 0;
			MassData md;

			flags &= ~BFL_DirtyMass;

			if (shapes.size() == 1 && shapes[0]->local == Transform4::identity())
				flags |= BFL_Identity;
			else
				flags &= ~BFL_Identity;

			if ((flags & (BFL_Static | BFL_Kinematic)) > 0)
			{
				C = Vector4::zero();
				P = Tx.position;
				invInertiaModel = invInertiaWorld = Tensor4::zero();
				return;
			}

			for (auto &shape : shapes)
			{
				if (shape->density == 0)
					continue;

				shape->ComputeMass(md);
				mass += md.mass;
				inertia += md.inertia;
				lc += md.center * md.mass;
			}

			if (mass > 0)
			{
				lc *= (invMass = 1 / mass);
				inertia -= Transform(lc, mass);
				invInertiaModel = Inverse(inertia);
			}
			else
			{
				// force the dynamic body to have some mass
				mass = 0;
				invMass = 1;
				invInertiaModel = invInertiaWorld = Tensor4::zero();
			}

			if ((flags & BFL_ZeroMass) > 0)
				invMass = 0;
			if ((flags & BFL_ZeroInertia) > 0)
				invInertiaModel = Tensor4::zero();

			P = Tx * (C = lc);
		}
		void Body::SynchronizeProxies()
		{

			if (scene == NULL)
				return;

			BroadPhase &broadphase = scene->contactManager.broadphase;

			Tx.position = P - Tx.rotation * C;

			for (auto &shape : shapes)
			{
				broadphase.Update(shape->broadPhaseIndex, shape->ComputeAABB(Tx));
			}
		}
		void Body::SetScene(Scene *scene)
		{
			this->scene = scene;
			for (auto &shape : shapes)
			{
				shape->CheckHash();
			}
		}
		void Body::AddShape(Shape *shape)
		{
			if (shape->body != NULL)
				throw;

			shapes.push_back(shape);

			shape->body = this;

			if (scene != NULL)
				shape->CheckHash();

			flags |= BFL_DirtyMass;

			scene->contactManager.broadphase.InsertShape(shape, shape->ComputeAABB(Tx));
		}
		void Body::RemoveShape(Shape *shape)
		{
			if (shape->body != this)
				throw;

			shape->RemoveHash();

			shapes.erase(std::find(shapes.begin(), shapes.end(), shape));

			// Remove all contacts associated with this shape
			for (size_t i = contactList.size(); i-- > 0;)
			{
				Contact *contact = contactList[i]->contact;

				if (shape == contact->A || shape == contact->B)
					scene->contactManager.RemoveContact(*contact);
			}

			flags |= BFL_DirtyMass;

			scene->contactManager.broadphase.RemoveShape(shape);

			delete shape;
		}
		void Body::RemoveAllShapes()
		{
			for (size_t i = shapes.size(); i-- > 0;)
			{
				scene->contactManager.broadphase.RemoveShape(shapes[i]);
			}

			shapes.clear();

			scene->contactManager.RemoveContactsFromBody(*this);
		}
		void Body::ApplyForce(Vector4 force)
		{
			SetToAwake();

			this->force += force;
		}
		void Body::ApplyForceAtWorldPoint(Vector4 force, Vector4 point)
		{
			SetToAwake();

			this->force += force;
			this->torque += Cross(point - P, force);
		}
		void Body::ApplyLinearImpulse(Vector4 impulse)
		{
			SetToAwake();

			this->linearVelocity += impulse * invMass;
		}
		void Body::ApplyLinearImpulseAtWorldPoint(Vector4 impulse, Vector4 point)
		{
			SetToAwake();

			linearVelocity += impulse * invMass;
			angularVelocity += invInertiaWorld * Cross(point - P, impulse);
		}
		void Body::ApplyTorque(Euler4 torque)
		{
			SetToAwake();

			this->torque += torque;
		}
		void Body::SetToAwake()
		{
			if ((flags & BFL_DirtyMass) > 0)
				CalculateMassData();
			if ((flags & BFL_Awake) == 0)
			{
				flags |= BFL_Awake;
				sleepTime = 0;
			}
		}
		void Body::SetToSleep()
		{
			if ((flags & BFL_Awake) > 0)
			{
				flags &= ~BFL_Awake;
				sleepTime = 0;
				linearVelocity = force = Vector4::zero();
				angularVelocity = torque = Euler4::zero();
			}
		}
		bool Body::IsAwake()
		{
			return (flags & BFL_Awake) > 0;
		}
		inline bool Body::Active(void)
		{
			return (flags & BFL_Active) > 0;
		}
		inline bool Body::Active(bool value)
		{
			if (!value)
			{
				scene->contactManager.RemoveContactsFromBody(*this);
				SetToSleep();
				flags &= ~BFL_Active;
			}
			else
			{
				flags |= BFL_Active;
				SetToAwake();
			}
		}
		bool Body::CanCollide(Body *a, Body *b)
		{
			// Can't collide self
			if (a == b)
				return false;

			// At least one of them must be dynamic
			if (((a->flags | b->flags) & BFL_Dynamic) == 0)
				return false;

			// And in the same layer
			if ((a->layers & b->layers) == 0)
				return false;

			// And both is active
			if ((a->flags & b->flags & BFL_Active) == 0)
				return false;

			return true;
		}
		int Body::GetFlags() { return flags; }
		void Body::SetFlags(BodyFlags type, bool awake, bool allowSleep, bool active)
		{

			flags = type;

			sleepTime = 0;
			flags |= awake ? BFL_Awake : 0;
			flags |= allowSleep ? BFL_AllowSleep : 0;
			flags |= active ? BFL_Active : 0;
		}
		Transform4 Body::GetTransform() { return Tx; }
		void Body::GetTransform(Transform4 t)
		{
			t.position = P;
			t.rotation = Tx.rotation;
		}
		void Body::SetTransform(Vector4 position, Matrix4 rotation)
		{
			P = position;
			Tx.rotation = rotation;

			SynchronizeProxies();
		}
	} // namespace Physics
} // namespace Forth