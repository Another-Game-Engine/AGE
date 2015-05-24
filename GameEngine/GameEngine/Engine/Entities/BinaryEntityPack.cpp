#include "BinaryEntityPack.hpp"

#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <fstream>

namespace AGE
{
	BinaryEntityPack::BinaryEntityPack()
	{}

	BinaryEntityPack::~BinaryEntityPack()
	{}

	void BinaryEntityPack::save(cereal::PortableBinaryOutputArchive  &ar, const std::uint32_t version) const
	{
		ar(componentsIdReferenceTable, entities);
	}

	void BinaryEntityPack::load(cereal::PortableBinaryInputArchive &ar, const std::uint32_t version)
	{
		ar(componentsIdReferenceTable, entities);
	}

	void BinaryEntityPack::loadFromFile(const std::string &filePath)
	{
		std::ifstream file(filePath.c_str(), std::ios::binary);
		{
			auto ar = cereal::PortableBinaryInputArchive(file);
			ar(*this);
		}
		file.close();
	}

	void BinaryEntityPack::saveToFile(const std::string &filePath)
	{
		std::ofstream file(filePath.c_str(), std::ios::binary);
		{
			auto ar = cereal::PortableBinaryOutputArchive(file);
			ar(*this);
		}
		file.close();
	}
}