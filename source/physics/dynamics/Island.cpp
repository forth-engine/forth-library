#include "Island.h"

namespace Forth
{
	namespace Physics
	{
		void Island::Solve(void)
		{
			// Apply gravity, Integrate velocities, create state buffers, calculate world inertia
			for (size_t i = 0; i < bodies.size(); ++i)
			{
				Body &body = *bodies[i];

				if ((body.flags & BFL_Dynamic) > 0)
				{
					if ((body.flags & BFL_DirtyMass) > 0)
						body.CalculateMassData();

					body.ApplyForce(Common::GRAVITY * body.gravityScale * body.mass);

					// Calculate world space intertia tensor
					body.invInertiaWorld = Transform(body.invInertiaModel, body.Tx.rotation);

					// Integrate velocity
					body.linearVelocity += (body.force * body.invMass) * Dt;
					body.angularVelocity += (body.invInertiaWorld * body.torque) * Dt;

					// Apply damping.
					body.linearVelocity *= 1 / (1 + Dt * body.linearDamping);
					body.angularVelocity *= 1 / (1 + Dt * body.angularDamping);

					velocities[i] = VelocityState(body.linearVelocity, body.angularVelocity);
				}
			}

			// Create contact solver, pass in state buffers, create buffers for contacts
			// Initialize velocity constraint for normal + friction and warm start
			contactSolver.PreSolve(*this, Dt);

			// Solve contacts
			for (int h = 0; h < Common::ITERATIONS; ++h)
				contactSolver.Solve(*this);

			// Copy back state buffers
			// Integrate positions
			// Release Island tag for statics (so it can be used for other islands)
			for (size_t i = 0; i < bodies.size(); ++i)
			{
				Body &body = *bodies[i];
				VelocityState v = velocities[i];

				if ((body.flags & BFL_Static) > 0)
				{
					body.flags &= ~BFL_Island;
					continue;
				}

				if (!Common::SIM_RANGE.Contains(body.P))
				{
					body.SetActive(false);
				}

				// Integrate position
				body.P += (body.linearVelocity = v.v) * Dt;
				body.Tx.rotation = Euler((body.angularVelocity = v.w) * (Dt * RAD2DEG)) * body.Tx.rotation;
			}

			if (Common::ALLOW_SLEEP)
			{
				// Find minimum sleep time of the entire island
				float minSleepTime = FLT_MAX;
				for (size_t i = 0; i < bodies.size(); ++i)
				{
					Body &body = *bodies[i];

					if ((body.flags & BFL_Static) > 0)
						continue;

					float sqrLinVel = LengthSq(body.linearVelocity);
					float cbAngVel = LengthSq(body.angularVelocity);

					if (sqrLinVel > Common::SLEEP_LINEAR || cbAngVel > Common::SLEEP_ANGULAR)
						body.sleepTime = minSleepTime = 0;
					else
						minSleepTime = Min(minSleepTime, body.sleepTime += Dt);
				}

				// Put entire island to sleep so long as the minimum found sleep time
				// is below the threshold. If the minimum sleep time reaches below the
				// sleeping threshold, the entire island will be reformed next step
				// and sleep test will be tried again.
				if (minSleepTime > Common::SLEEP_TIME)
				{
					for (size_t i = 0; i < bodies.size(); ++i)
						bodies[i]->SetToSleep();
				}
			}
		}
		void Island::Add(Body* body)
		{
			body->islandIndex = bodies.size();
			bodies.push_back(body);
			velocities.push_back(VelocityState());
		}
		void Island::Add(Contact* contact)
		{
			contacts.push_back(contact);
		}
		void Island::Clear(void)
		{
			bodies.clear();
			velocities.clear();
			contacts.clear();
		}

		Island::Island()
		{
			bodies = ::std::vector<Body*>();
			velocities = ::std::vector<VelocityState>();
			contacts = ::std::vector<Contact*>();
			contactSolver = ContactSolver();
			Dt = 0;
		}
	} // namespace Physics
} // namespace Forth