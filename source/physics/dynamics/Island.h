#pragma once

#include "../Common.h"
#include "Body.h"
#include "Contact.h"
#include "ContactSolver.h"

namespace Forth
{
	namespace Physics
	{
		struct VelocityState
		{
			Euler4 w;
			Vector4 v;

			VelocityState() : w(Euler4()), v(Vector4()) {}
			VelocityState(const Vector4 &v, const Euler4 &w)
			{
				this->v = v;
				this->w = w;
			}
		};

		class Island
		{
		  public:
			void Solve(void);

			void Add(Body *body);

			void Add(Contact *contact);

			void Clear(void);

			Island();

			::std::vector<Body *> bodies;
			::std::vector<VelocityState> velocities;
			::std::vector<Contact *> contacts;
			ContactSolver contactSolver;

			float Dt;
		};
	} // namespace Physics
} // namespace Forth
