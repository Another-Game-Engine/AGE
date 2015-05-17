#pragma once

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/map.hpp>

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
		std::vector<EntitySerializationInfos> list;
		std::size_t entityNbr;

		static SceneChunkSerialization CreateForSerialization(const CptIdsRefTable &refTable, const std::vector<EntitySerializationInfos> &entities)
		{
			SceneChunkSerialization instance;
			instance.componentsIdReferenceTable = refTable;

			for (auto &e : entities)
			{
				auto &components = e.entity.getComponentList();
				for (auto &c : components)
				{
					if (c)
					{
#ifdef EDITOR_ENABLED
						if (WESerialization::SerializeForEditor() == false && !c->serializeInExport())
						{
							continue;
						}
#endif
						e.componentTypes.push_back(c->getType());
						e.components.push_back(c);
					}
				}
				instance.list.push_back(e);
			}

			instance.entityNbr = entities.size();
			return instance;
		}

		static SceneChunkSerialization CreateForLoad()
		{
			return SceneChunkSerialization();
		}

		template <typename Archive>
		void save(Archive &ar, const std::uint32_t version) const
		{
			ar(entityNbr, componentsIdReferenceTable, list);
		}

		template <typename Archive>
		void load(Archive &ar, const std::uint32_t version)
		{
			ar(entityNbr, componentsIdReferenceTable, list);
		}

	private:
		SceneChunkSerialization()
			: entityNbr(0)
		{}
	};
}

CEREAL_CLASS_VERSION(AGE::SceneChunkSerialization, 0)