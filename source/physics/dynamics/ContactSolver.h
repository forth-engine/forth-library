#pragma once

#include "../Common.h"
#include "Contact.h"

namespace Forth
{
	namespace Physics
	{
		// Contact solver
		class ContactSolver
		{
		public:
			ContactSolver(){}
			ContactSolver(class Island& island);

			void PreSolve(float dt);

			void Solve();

			::std::vector<Contact>* contacts;
			::std::vector<struct VelocityState>* velocities;
		};

	} // namespace Physics
}