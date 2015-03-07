#pragma once

#include <Skinning/AnimationChannel.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

namespace AGE
{
	struct AnimationData
	{
		std::string name;
		AGE::Vector<AnimationChannel> channels;
		float duration;
		std::uint32_t id;
	};

	template <class Archive>
	void serialize(Archive &ar, AnimationData &value)
	{
		ar(value.name, value.channels, value.duration, value.id);
	}
}