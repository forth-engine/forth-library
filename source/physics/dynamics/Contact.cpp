#include "Contact.h"
#include "Body.h"

namespace Forth
{
	namespace Physics
	{

		Contact::Contact() : manifold(), edgeA(), edgeB(), state()
		{
		}

		void Contact::Setup(Shape *A, Shape *B)
		{
			this->A = A;
			this->B = B;
			bodyA = A->body;
			bodyB = B->body;
			friction = Common::MixFriction(A, B);
			restitution = Common::MixRestitution(A, B);
			sensor = A->sensor || B->sensor;
			hash = A->hash ^ B->hash;
			flags = 0;

			manifold.Setup(A, B);
			edgeA.Setup(bodyA, bodyB, this);
			edgeB.Setup(bodyB, bodyA, this);
			state.Setup(*this);
		}
		void Contact::SolveCollision()
		{
			manifold.contacts = 0;

			Collide::ComputeCollision(manifold, *A, *B);

			//{
			//    if (manifold.contacts > 0)
			//        Debug.LogWarning("Contact N: " + manifold.normal + " D:" + manifold.depth[0]);
			//}

			if (manifold.contacts > 0)
				flags |= (flags & CF_Colliding) > 0 ? CF_WasColliding : CF_Colliding;
			else
				flags &= (flags & CF_Colliding) > 0 ? ~CF_Colliding : ~CF_WasColliding;
		}
		void Manifold::MakeContact(const Vector4 &n, const Vector4 &p, float d)
		{
			normal = Normalize(n);
			depth[contacts] = d;
			position[contacts] = p;
			contacts++;
		}
		void Manifold::MakeContact(const Vector4 &p, float d)
		{
			depth[contacts] = d;
			position[contacts] = p;
			contacts++;
		}
		void Manifold::MakeContact(const Vector4 &p0, const Vector4 &p1)
		{
			depth[contacts] = LengthSq(p1 - p0);
			position[contacts] = (p0 + p1) * 0.5f;
			contacts++;
		}
		void Manifold::MakeContact(const Vector4 &n)
		{
			normal = Normalize(n);
		}
		void ContactEdge::Setup(Body *b, Body *o, Contact *c)
		{
			b->contactList.push_back(this);
			other = o;
			contact = c;
		}
		ContactState::ContactState()
		{
			for (int i = 0; i < contacts; i++)
			{
				units[i] = ContactStateUnit();
			}
		}
		void ContactState::UpdateInput(const Contact &c)
		{
			const Manifold &m = c.manifold;

			if ((contacts = m.contacts) > 0)
			{

				A.Update(*c.bodyA);
				B.Update(*c.bodyB);

				for (int i = 0; i < contacts; i++)
				{
					units[i].Update(m, c, i);
				}

				vectors[0] = m.normal;

				if (Common::ENABLE_FRICTION)
					ComputeBasis(vectors);
			}
		}
		void ContactState::Setup(const Contact &c)
		{
			friction = c.friction;
			restitution = c.restitution;
		}
		void ContactStateUnit::Update(const Manifold &m, const Contact &c, int i)
		{
			rA = m.position[i] - c.bodyA->P;
			rB = m.position[i] - c.bodyB->P;

			depth = m.depth[i];
			bias = 0;
		}
		void ContactStateBody::Update(const Body &body)
		{
			m = body.invMass;
			index = body.islandIndex;
			i = body.invInertiaWorld;
		}
	} // namespace Physics
} // namespace Forth