#include "BinaryEntityPack.hpp"

#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>

namespace AGE
{
	BinaryEntityPack::BinaryEntityPack()
	{}

	BinaryEntityPack::~BinaryEntityPack()
	{}

	void BinaryEntityPack::save(cereal::PortableBinaryOutputArchive  &ar, const std::uint32_t version) const
	{}

	void BinaryEntityPack::load(const cereal::PortableBinaryInputArchive &ar, const std::uint32_t version)
	{}
}