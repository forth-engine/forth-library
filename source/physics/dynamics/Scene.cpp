#include "Scene.h"

namespace Forth
{
	namespace Physics
	{
		Scene::Scene() : contactManager(), island(), stack(), bodies(), shapes() {}

		// Run the simulation forward in time by dt

		void Scene::Step(float Dt)
		{
			// Mitigate the old and look for contacts
			contactManager.TestCollisions();

			contactManager.broadphase.UpdatePairs();

			island.Dt = Min(Dt, Common::MAX_DT);

			// Build each active island and then solve each built island
			for (auto seed : bodies)
			{
				// Skip if this seed has been sweeped out
				// Skip if object static
				if ((seed->flags & (BFL_Island | BFL_Static)) > 0)
					continue;

				// Seed must be awake & active
				if (!seed->IsAwake())
					continue;

				// Clear the environemt
				island.Clear();

				// Sweep related bodies
				CollectRelatedBodies(seed);

				// Nuke it
				island.Solve();
			}

			// Update the broadphase AABBs
			// Clear all forces
			// Clear island marks
			for (auto body : bodies)
			{
				body->SynchronizeProxies();
				body->force = Vector4::zero;
				body->torque = Euler4::zero();
				body->flags &= ~BFL_Island;
			}
		}
		void Scene::CollectRelatedBodies(Body *seed)
		{
			//stack.Clear();
			stack.push(seed);

			// Mark seed as apart of island
			seed->flags |= BFL_Island;

			// Perform DFS (Depth First Search) on constraint graph
			while (stack.size() > 0)
			{
				// Decrement stack to implement iterative backtracking
				Body *body = stack.top();
				stack.pop();
				island.Add(body);

				// Awaken all bodies connected to the island
				body->SetToAwake();

				// Do not search across static bodies to keep island
				// formations as small as possible, however the static
				// body itself should be apart of the island in order
				// to properly represent a full contact
				if ((body->flags & BFL_Static) > 0)
					continue;

				// Search all contacts connected to this body
				for (auto &edge : body->contactList)
				{
					Contact &contact = *edge->contact;

					// Skip contacts that have been added to an island already
					if ((contact.flags & CF_Island) > 0)
						continue;

					// Can safely skip this contact if it didn't actually collide with anything
					if ((contact.flags & CF_Colliding) == 0)
						continue;

					// Skip sensors
					if (contact.sensor)
						continue;

					// Mark island flag and add to island
					contact.flags |= CF_Island;
					island.Add(&contact);

					// Attempt to add the other body in the contact to the island
					// to simulate contact awakening propogation
					Body *other = edge->other;

					if ((other->flags & BFL_Island) > 0)
						continue;

					stack.push(other);

					other->flags |= BFL_Island;
				}
			}
		}

		void Scene::CreateBody(Body *body)
		{
			if (body->scene != NULL)
				throw;

			body->scene = this;

			bodies.push_back(body);
		}

		// Frees a body, removes all shapes associated with the body and frees
		// all shapes and contacts associated and attached to this body->

		void Scene::RemoveBody(Body *body)
		{
			contactManager.RemoveContactsFromBody(body);

			body->RemoveAllShapes();

			// Remove body from scene Bodies
			bodies.erase(::std::find(bodies.begin(), bodies.end(), body));
		}

		// Removes all bodies from the scene.

		void Scene::ClearBodies()
		{
			for (auto body : bodies)
			{
				body->RemoveAllShapes();
				body->scene = NULL;
			}

			bodies.clear();
		}

		/// <summary>
		/// Query the world to find any shapes intersecting a ray.
		/// </summary>

		void Scene::QueryRaycast(RaycastHit4 &rayCast)
		{
			contactManager.broadphase.Tree.Query(rayCast);
		}
	} // namespace Physics
} // namespace Forth