#include "ContactSolver.h"
#include "Island.h"

namespace Forth
{
	namespace Physics
	{
		ContactSolver::ContactSolver()
		{
			/*contacts = &island->contacts;
			velocities = &island->velocities;*/
		}

		void ContactSolver::PreSolve(Island &island, float dt)
		{
			auto &contacts = island.contacts;
			auto &velocities = island.velocities;

			for (size_t i = 0; i < contacts.size(); ++i)
			{
				Contact &c = *contacts[i];
				ContactState &cs = c.state;

				cs.UpdateInput(c);

				if (cs.contacts == 0)
					continue;

				float nm = cs.A.m + cs.B.m, dv;

				Vector4 &vA = velocities[cs.A.index].v, &vB = velocities[cs.B.index].v;
				Euler4 &wA = velocities[cs.A.index].w, &wB = velocities[cs.B.index].w;

				for (int j = 0; j < cs.contacts; j++)
				{

					ContactStateUnit &u = cs.units[j];

					// Precalculate bias factor

					u.bias = -Common::BAUMGARTE * (1 / dt) * Min(0.f, u.depth + Common::PENETRATION_SLOP);

					if ((dv = Dot(vB + Cross(wB, u.rB) - vA - Cross(wA, u.rA), cs.vectors[0])) < -1)
						u.bias -= cs.restitution * dv;


					for (int k = Common::ENABLE_FRICTION ? 4 : 1; k-- > 0;)
					{
						// Precalculate vector masses
						Euler4 raCt = Cross(u.rA, cs.vectors[k]);
						Euler4 rbCt = Cross(u.rB, cs.vectors[k]);

						auto tm = nm + Dot(raCt, cs.A.i * raCt) + Dot(rbCt, cs.B.i * rbCt);
						u.masses[k] = Invert(tm);
						u.impulses[k] = 0;
					}

					// Warm start contact
					Vector4 P = cs.vectors[0] * u.impulses[0];

					if ( Common::ENABLE_FRICTION )
					{
						P += cs.vectors[1] * u.impulses[1];
						P += cs.vectors[2] * u.impulses[2];
						P += cs.vectors[3] * u.impulses[3];
					}

					vA -= P * cs.A.m;
					wA -= cs.A.i * Cross(u.rA, P );

					vB += P * cs.B.m;
					wB += cs.B.i * Cross(u.rB, P );
				}
			}
		}
		void ContactSolver::Solve(Island &island)
		{
			auto &contacts = island.contacts;
			auto &velocities = island.velocities;

			for (size_t i = 0; i < contacts.size(); ++i)
			{
				ContactState &cs = (*contacts[i]).state;

				if (cs.contacts == 0)
					continue;

				Vector4 &vA = velocities[cs.A.index].v, &vB = velocities[cs.B.index].v;
				Euler4 &wA = velocities[cs.A.index].w, &wB = velocities[cs.B.index].w;

				for (int j = 0; j < cs.contacts; j++)
				{
					ContactStateUnit &u = cs.units[j];

					// relative velocity at contact
					Vector4 dv = vB + Cross(wB, u.rB) - vA - Cross(wA, u.rA);

					// Friction
					for (int k = Common::ENABLE_FRICTION ? 4 : 1; k-- > 0;)
					{
						float lambda, oldP = u.impulses[k];

						// Clamp frictional impulse
						if (k == 0)
						{
							lambda = (-Dot(dv, cs.vectors[k]) + u.bias) * u.masses[k];
							lambda = Max(0.f, oldP + lambda);
						}
						else
						{
							lambda = -Dot(dv, cs.vectors[k]) * u.masses[k];
							auto c = cs.friction * u.impulses[0];
							lambda = Clamp(-c, c, oldP + lambda);
						}

						// Apply friction impulse
						Vector4 impulse = cs.vectors[k] * ((u.impulses[k] = lambda) - oldP);

						vA -= impulse * cs.A.m;
						wA -= cs.A.i * Cross(u.rA, impulse);

						vB += impulse * cs.B.m;
						wB += cs.B.i * Cross(u.rB, impulse);
					}
				}

				/*velocities[cs.A.index] = VelocityState(vA, wA);
				velocities[cs.B.index] = VelocityState(vB, wB);*/
			}
		}
	} // namespace Physics
} // namespace Forth