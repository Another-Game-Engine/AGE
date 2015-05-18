#pragma once

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/list.hpp>

#include <Entities/EntitySerializationInfos.hpp>

#ifdef EDITOR_ENABLED
#include <WorldEditorGlobal.hpp>
#endif

namespace AGE
{
	// Used to load and save entire scene or chunks of it
	struct SceneChunkSerialization
	{
		typedef std::map<ComponentType, std::size_t> CptIdsRefTable;
		CptIdsRefTable componentsIdReferenceTable;
		std::list<EntitySerializationInfos> list;
		std::size_t entityNbr = 0;
		AScene *sceneToLoad = nullptr;

		static SceneChunkSerialization CreateForSerialization(const CptIdsRefTable &refTable, std::vector<EntitySerializationInfos> entities);
		static SceneChunkSerialization CreateForLoad(AScene *scene);

		template <typename Archive>
		void save(Archive &ar, const std::uint32_t version) const
		{
			ar(entityNbr, componentsIdReferenceTable);
			for (auto &e : list)
			{
				ar(e);
			}
		}

		template <typename Archive>
		void load(Archive &ar, const std::uint32_t version)
		{
			ar(entityNbr, componentsIdReferenceTable);
			list.resize(entityNbr);

			AGE_ASSERT(sceneToLoad != nullptr);

			for (auto &e : list)
			{
				e = EntitySerializationInfos(sceneToLoad->createEntity().getDataPtr(), &componentsIdReferenceTable);
				ar(e);
			}
		}
	};
}

CEREAL_CLASS_VERSION(AGE::SceneChunkSerialization, 0)