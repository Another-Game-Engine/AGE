#include "ReadableEntityPack.hpp"

#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>

#include <fstream>

namespace AGE
{
	ReadableEntityPack::ReadableEntityPack()
	{}

	ReadableEntityPack::~ReadableEntityPack()
	{}

	void ReadableEntityPack::save(cereal::JSONOutputArchive  &ar, const std::uint32_t version) const
	{
		std::size_t entityNumber = entities.size();
		ar(CEREAL_NVP(componentsIdReferenceTable)
			, CEREAL_NVP(entityNumber)
			, CEREAL_NVP(entities));
	}

	void ReadableEntityPack::load(cereal::JSONInputArchive &ar, const std::uint32_t version)
	{
		std::size_t entityNumber;
		ar(componentsIdReferenceTable, entityNumber, entities);
	}

	void ReadableEntityPack::loadFromFile(const std::string &filePath)
	{
		std::ifstream file(filePath.c_str(), std::ios::binary);
		{
			auto ar = cereal::JSONInputArchive(file);
			ar(*this);
		}
		file.close();
	}

	void ReadableEntityPack::saveToFile(const std::string &filePath)
	{
		std::ofstream file(filePath.c_str(), std::ios::binary);
		{
			auto ar = cereal::JSONOutputArchive(file);
			ar(*this);
		}
		file.close();
	}
}