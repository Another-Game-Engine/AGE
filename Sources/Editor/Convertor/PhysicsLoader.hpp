#pragma once

#include "CookingTask.hpp"

namespace AGE
{
	struct PhysicsLoader
	{
		static bool save(std::shared_ptr<CookingTask> cookingTask);

		static bool load(std::shared_ptr<CookingTask> cookingTask);
	};
}