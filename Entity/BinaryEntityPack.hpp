#pragma once

#include <Entity/BinaryEntity.hpp>

namespace AGE
{
	class AScene;

	struct BinaryEntityPack
	{
		typedef std::map<ComponentType, std::size_t> CptIdsRefTable;
		CptIdsRefTable componentsIdReferenceTable;
		std::vector<BinaryEntity> entities;
		AScene *scene;

		BinaryEntityPack();
		~BinaryEntityPack();
		void save(cereal::PortableBinaryOutputArchive  &ar, const std::uint32_t version) const;
		void load(cereal::PortableBinaryInputArchive &ar, const std::uint32_t version);
		void loadFromFile(const std::string &filePath);
		void saveToFile(const std::string &filePath);
	};
}

CEREAL_CLASS_VERSION(AGE::BinaryEntityPack, 0);