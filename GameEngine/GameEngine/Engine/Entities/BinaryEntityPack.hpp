#pragma once

#include <Entities/BinaryEntity.hpp>

namespace AGE
{
	class AScene;
	struct BinaryEntity;

	struct BinaryEntityPack
	{
		typedef std::map<ComponentType, std::size_t> CptIdsRefTable;
		CptIdsRefTable componentsIdReferenceTable;
		std::vector<BinaryEntity> entities;
		AScene *scene;

		BinaryEntityPack();
		~BinaryEntityPack();
		void save(cereal::PortableBinaryOutputArchive  &ar, const std::uint32_t version) const;
		void load(const cereal::PortableBinaryInputArchive &ar, const std::uint32_t version);
	};
}