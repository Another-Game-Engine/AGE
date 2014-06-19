#pragma once

#include "AnimationChannel.hpp"

namespace AGE
{
	struct Animation
	{
		std::string name;
		std::vector<AnimationChannel> channels;
		float duration;
		unsigned int id;
	};
}