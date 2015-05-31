#include "ReadableEntity.hpp"

#include <Entities/EntityData.hh>

#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>

#include <Components/ComponentRegistrationManager.hpp>
#include <Core/AScene.hh>
#include <Managers/ArchetypesEditorManager.hpp>

#include <Utils/Debug.hpp>

#include <Entities/BinaryEntity.hpp>

namespace AGE
{
	ReadableEntity::ReadableEntity()
	{}

	ReadableEntity::~ReadableEntity()
	{}

	void ReadableEntity::save(cereal::JSONOutputArchive &ar, const std::uint32_t version) const
	{

		AGE::Link link = entity.getLink();
		ENTITY_FLAGS flags = entity.getFlags();
		ar(cereal::make_nvp("link", link)
			, cereal::make_nvp("children", children)
			, cereal::make_nvp("flags", flags)
			, cereal::make_nvp("components_number", componentTypes)
			, CEREAL_NVP(archetypesDependency)
			);
		for (auto &e : components)
		{
			ComponentRegistrationManager::getInstance().serializeJson(e, ar);
		}
	}

	void ReadableEntity::load(cereal::JSONInputArchive &ar, const std::uint32_t version)
	{
		Link link;
		ENTITY_FLAGS flags;

		AGE_ASSERT(typesMap != nullptr);

		ar(link
			, children
			, flags
			, componentTypes
			, archetypesDependency);

		auto archetypeManager = entity.getScene()->getInstance<AGE::WE::ArchetypeEditorManager>();

		// we load archetypes dependency
		if (!archetypesDependency.empty())
		{
			for (auto &dep : archetypesDependency)
			{
				archetypeManager->loadOne(dep);
			}
		}

		entity.getLink().setPosition(link.getPosition());
		entity.getLink().setOrientation(link.getOrientation());
		entity.getLink().setScale(link.getScale());
		//entity.setFlags(f);
		for (auto &e : componentTypes)
		{
			auto hashType = (*typesMap)[e];
			auto newComponent = ComponentRegistrationManager::getInstance().loadJson(hashType, entity, ar);
		}
		if (entity.haveComponent<ArchetypeComponent>())
		{
			auto archetypeName = entity.getComponent<ArchetypeComponent>()->archetypeName;
			archetypeManager->spawn(entity, archetypeName);
		}
	}

	BinaryEntity ReadableEntity::toBinary()
	{
		BinaryEntity bin;

		bin.entity = entity;

		bin.children = children;

		if (entity.haveComponent<ArchetypeComponent>())
		{
			auto archetypeCpt = entity.getComponent<ArchetypeComponent>();
			bin.componentTypes.push_back(archetypeCpt->getType());
			bin.components.push_back(archetypeCpt);
			bin.archetypesDependency.push_back(archetypeCpt->archetypeName);
			return bin;
		}

		for (auto &c : components)
		{
			if (!c->serializeInExport())
			{
				continue;
			}
			bin.componentTypes.push_back(c->getType());
			bin.components.push_back(c);
		}
		return bin;
	}
}