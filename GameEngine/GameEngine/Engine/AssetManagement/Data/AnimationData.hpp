#pragma once

#include <Skinning/AnimationChannel.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <stdint.h>

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
	void serialize(Archive &ar, AnimationData &value, const std::uint32_t version)
	{
		ar(value.name, value.channels, value.duration, value.id);
	}
}

CEREAL_CLASS_VERSION(AGE::AnimationData, 0)