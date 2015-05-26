#include "BinaryEntityPack.hpp"

#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <fstream>
#include <Utils/Debug.hpp>
#include <Core/AScene.hh>

namespace AGE
{
	BinaryEntityPack::BinaryEntityPack()
	{}

	BinaryEntityPack::~BinaryEntityPack()
	{}

	void BinaryEntityPack::save(cereal::PortableBinaryOutputArchive  &ar, const std::uint32_t version) const
	{
		std::size_t entityNumber = entities.size();
		ar(CEREAL_NVP(componentsIdReferenceTable));
		ar(CEREAL_NVP(entityNumber));

		for (std::size_t i = 0; i < entities.size(); ++i)
		{
			ar(cereal::make_nvp(std::string(std::string("Entity_") + std::to_string(i)).c_str(), entities[i]));
		}
	}

	void BinaryEntityPack::load(cereal::PortableBinaryInputArchive &ar, const std::uint32_t version)
	{
		AGE_ASSERT(scene != nullptr);
		std::size_t entityNumber;
		ar(componentsIdReferenceTable);
		ar(entityNumber);
		entities.resize(entityNumber);
		for (std::size_t i = 0; i < entities.size(); ++i)
		{
			entities[i].entity = scene->createEntity();
			entities[i].typesMap = &componentsIdReferenceTable;
			ar(entities[i]);
		}
		for (auto &e : entities)
		{
			for (auto &c : e.children)
			{
				e.entity.getLink().attachChild(entities[c].entity.getLinkPtr());
			}
		}
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