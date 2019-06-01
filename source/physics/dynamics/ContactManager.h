#pragma once

#include "../Common.h"
#include "../broadphase/BroadPhase.h"
#include "Contact.h"
#include "Body.h"

namespace Forth
{
	namespace Physics
	{
		class ContactManager
		{
		  public:
			  ContactManager();

			// Add a contact contact for a pair of objects
			// unless the contact contact already exists
			  void AddContact(Shape *A, Shape *B);

			// Remove a specific contact
			  void RemoveContact(const Contact &contact);

			// Remove all contacts from a body
			  void RemoveContactsFromBody(const Body& body);

			  void RemoveFromBroadphase(Body body);

			// Remove contacts without broadphase overlap
			// Solves contact manifolds
			  void TestCollisions();

			  void CheckCollision(Contact c);

			::std::map<int, Contact> contactList;
			BroadPhase broadphase;
			struct IContactListener* contactListener;
		};
	} // namespace Physics
} // namespace Forth