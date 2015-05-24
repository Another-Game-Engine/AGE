#include "ReadableEntity.hpp"

#include <Entities/EntityData.hh>

#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>

namespace AGE
{
	ReadableEntity::ReadableEntity()
	{}

	ReadableEntity::~ReadableEntity()
	{}

	void ReadableEntity::save(cereal::JSONOutputArchive  &ar, const std::uint32_t version) const
	{

	}

	void ReadableEntity::load(const cereal::JSONInputArchive &ar, const std::uint32_t version)
	{

	}
}