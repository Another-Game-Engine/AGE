#include "ReadableEntityPack.hpp"

#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>

namespace AGE
{
	ReadableEntityPack::ReadableEntityPack()
	{}

	ReadableEntityPack::~ReadableEntityPack()
	{}

	void ReadableEntityPack::save(cereal::JSONOutputArchive  &ar, const std::uint32_t version) const
	{}

	void ReadableEntityPack::load(const cereal::JSONInputArchive &ar, const std::uint32_t version)
	{}
}