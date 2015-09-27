#pragma once

#include <memory>

namespace AGE
{
	struct CookingTask;

	class MeshLoader
	{
	public:
		static bool save(std::shared_ptr<CookingTask> cookingTask);
		static bool load(std::shared_ptr<CookingTask> cookingTask);
	};
}