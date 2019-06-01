#include "ContactManager.h"
#include "Scene.h"

namespace Forth
{
	namespace Physics
	{
		ContactManager::ContactManager()
		{
			broadphase = BroadPhase(this);
			contactList = ::std::map<int, Contact>();
			contactListener = NULL;
		}

		// Add a contact contact for a pair of objects
		// unless the contact contact already exists

		void ContactManager::AddContact(Shape * A, Shape * B)
		{
			Body *bodyA = A->body, *bodyB = B->body;

			if (!Body::CanCollide(bodyA, bodyB))
				return;

			// Search for existing matching contact
			if (contactList.count(A->hash ^ B->hash))
				return;

			// Create contact
			auto contact = Contact();
			contact.Setup(A, B);
			contactList[contact.hash] = contact;

			bodyA->SetToAwake();
			bodyB->SetToAwake();
		}

		// Remove a specific contact

		void ContactManager::RemoveContact(const Contact & contact)
		{
			Body *A = contact.bodyA;
			Body *B = contact.bodyB;

			// Remove from A
			A->contactList.erase(std::find(A->contactList.begin(), A->contactList.end(), &contact.edgeA));

			// Remove from B
			A->contactList.erase(std::find(B->contactList.begin(), B->contactList.end(), &contact.edgeB));

			A->SetToAwake();
			B->SetToAwake();

			// Remove contact from the manager
			contactList.erase(contact.hash);
		}

		// Remove all contacts from a body

		void ContactManager::RemoveContactsFromBody(const Body & body)
		{
			for (size_t i = body.contactList.size(); i-- > 0;)
			{
				RemoveContact(*body.contactList[i]->contact);
			}
		}
		void ContactManager::RemoveFromBroadphase(Body body)
		{
			for (auto shape : body.shapes)
			{
				broadphase.RemoveShape(shape);
			}
		}

		// Remove contacts without broadphase overlap
		// Solves contact manifolds

		void ContactManager::TestCollisions()
		{

			for (auto const &h : contactList)
			{
				auto contact = h.second;
				Shape *A = contact.A, *B = contact.B;
				Body *bodyA = A->body, *bodyB = B->body;

				contact.flags &= ~BFL_Island;

				if (!bodyA->IsAwake() && !bodyB->IsAwake())
				{
					continue;
				}

				if (!Body::CanCollide(bodyA, bodyB))
				{
					RemoveContact(contact);
					continue;
				}

				// Check if contact should persist
				if (!broadphase.TestOverlap(A->broadPhaseIndex, B->broadPhaseIndex))
				{
					RemoveContact(contact);
					continue;
				}

				contact.SolveCollision();

				if (contactListener != NULL)
					CheckCollision(contact);
			}
		}
		void ContactManager::CheckCollision(Contact c)
		{
			bool a = (c.flags & CF_Colliding) > 0;
			bool b = (c.flags & CF_WasColliding) > 0;

			if (a & !b)
			{
				contactListener->BeginContact(&c);
				c.flags |= CF_WasColliding;
			}
			else if (b & !a)
			{
				contactListener->EndContact(&c);
				c.flags &= ~CF_WasColliding;
			}
		}
	} // namespace Physics
} // namespace Forth