#pragma once

#include <Entities/ReadableEntity.hpp>

namespace AGE
{
	class AScene;

	struct ReadableEntityPack
	{
		typedef std::map<ComponentType, std::size_t> CptIdsRefTable;
		CptIdsRefTable componentsIdReferenceTable;
		std::vector<ReadableEntity> entities;
		AScene *scene;

		ReadableEntityPack();
		~ReadableEntityPack();
		void save(cereal::JSONOutputArchive  &ar, const std::uint32_t version) const;
		void load(const cereal::JSONInputArchive &ar, const std::uint32_t version);
	};
}

CEREAL_CLASS_VERSION(AGE::ReadableEntityPack, 0);