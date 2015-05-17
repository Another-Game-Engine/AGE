#pragma once

#include "Entity.hh"
#include <Components/Component.hh>
#include <vector>
#include <cereal/types/vector.hpp>
#include <Entities/EntityData.hh>

namespace AGE
{
	struct EntitySerializationInfos
	{
		std::vector <std::size_t> children;
		std::vector <ComponentBase*> components;
		std::vector <ComponentType> componentTypes;
		Entity &entity;
		std::map<ComponentType, std::size_t> *typesMap; // used to unserialize

		EntitySerializationInfos(EntityData *e)
			: entity(e->entity)
			, typesMap(nullptr)
		{}

		template < typename Archive >
		void save(Archive &ar, const std::uint32_t version) const
		{
			AGE::Link link = entity.getLink();
			ENTITY_FLAGS flags = entity.getFlags();
			ar(cereal::make_nvp("link", link)
				, cereal::make_nvp("children", children)
				, cereal::make_nvp("flags", flags)
				, cereal::make_nvp("components_number", componentTypes)
				);
			for (auto &e : components)
			{
				ComponentRegistrationManager::getInstance().serializeJson(e, ar);
			}
		}

		template < typename Archive >
		void load(Archive &ar, const std::uint32_t version)
		{
			std::size_t cptNbr = 0;
			Link l;
			ENTITY_FLAGS f;

			assert(typesMap != nullptr);
			ar(
				l
				, children
				, f
				, componentTypes);
			entity.getLink().setPosition(l.getPosition());
			entity.getLink().setOrientation(l.getOrientation());
			entity.getLink().setScale(l.getScale());
			//entity.setFlags(f);
			for (auto &e : componentTypes)
			{
				auto hashType = (*typesMap)[e];
				ComponentRegistrationManager::getInstance().loadJson(hashType, entity, ar);
			}
		}
	};
}

CEREAL_CLASS_VERSION(AGE::EntitySerializationInfos, 0)