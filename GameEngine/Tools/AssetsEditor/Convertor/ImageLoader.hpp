#pragma once

#include <memory>

__declspec(thread) static bool tlsComInitialized = false;

namespace AGE
{
	struct CookingTask;

	class ImageLoader
	{
	public:
		static bool save(std::shared_ptr<CookingTask> cookingTask);
		static bool load(std::shared_ptr<CookingTask> cookingTask);
	};
}