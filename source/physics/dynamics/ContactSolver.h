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
			ContactSolver();

			void PreSolve(class Island &island, float dt);

			void Solve(class Island &island);
		};

	} // namespace Physics
} // namespace Forth