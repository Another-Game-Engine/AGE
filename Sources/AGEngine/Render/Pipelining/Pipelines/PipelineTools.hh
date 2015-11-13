#pragma once

#include <memory>
#include "Utils/debug.hpp"

namespace AGE
{
	template <typename OUTPUT, typename ...Params>
	std::shared_ptr<OUTPUT> createRenderPassOutput(Params... params)
	{
		auto const &output = std::make_shared<OUTPUT>();
		auto error = output->init(std::forward<Params>(params)...);
		AGE_ASSERT(error != false && "Texture generation error.");
		return output;
	}

}