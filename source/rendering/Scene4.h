#pragma once

#include "Model4.h"
#include <vector>

namespace Forth
{
	class Scene4
	{
	  public:
		std::vector<Model4*> models;
		Physics::Scene physics;

		Scene4(void);
		~Scene4();
	};

	Scene4::Scene4(void)
	{
	}

	Scene4::~Scene4()
	{
	}

} // namespace Forth