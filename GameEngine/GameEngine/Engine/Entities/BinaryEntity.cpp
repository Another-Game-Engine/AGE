#include "BinaryEntity.hpp"

#include <Entities/EntityData.hh>

#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>

namespace AGE
{
	BinaryEntity::BinaryEntity()
	{}

	BinaryEntity::~BinaryEntity()
	{}

	void BinaryEntity::save(cereal::PortableBinaryOutputArchive  &ar, const std::uint32_t version) const
	{

	}

	void BinaryEntity::load(const cereal::PortableBinaryInputArchive &ar, const std::uint32_t version)
	{

	}
}