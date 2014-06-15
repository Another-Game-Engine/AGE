#pragma once

namespace AGE
{
	struct AnimationChannel;

	struct Animation
	{
		std::string name;
		std::vector<AnimationChannel> channels;
		float duration;
		unsigned int id;
	};
}