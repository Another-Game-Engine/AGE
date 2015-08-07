#include "BinaryEntity.hpp"

#include <Entities/EntityData.hh>
#include <Entities/ArchetypeManager.hpp>

#include <Core/AScene.hh>

#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>

#include <Components\ComponentRegistrationManager.hpp>

#include <Utils/Debug.hpp>

namespace AGE
{
	BinaryEntity::BinaryEntity()
	{}

	BinaryEntity::~BinaryEntity()
	{}

	void BinaryEntity::save(cereal::PortableBinaryOutputArchive  &ar, const std::uint32_t version) const
	{
		AGE::Link link = entity->getLink();
		ENTITY_FLAGS flags = entity.getFlags();
		ar(cereal::make_nvp("link", link)
			, cereal::make_nvp("children", children)
			, cereal::make_nvp("flags", flags)
			, cereal::make_nvp("components_number", componentTypes)
			, CEREAL_NVP(archetypesDependency)
			);
		for (auto &e : components)
		{
			ComponentRegistrationManager::getInstance().serializeBinary(e, ar);
		}
	}

	void BinaryEntity::load(cereal::PortableBinaryInputArchive &ar, const std::uint32_t version)
	{
		ENTITY_FLAGS flags;

		AGE_ASSERT(typesMap != nullptr);

		ar(entity->getLink()
			, children
			, flags
			, componentTypes
			, archetypesDependency);

		auto archetypeManager = entity->getScene()->getInstance<AGE::ArchetypeManager>();

		// we load archetypes dependency
		if (!archetypesDependency.empty())
		{
			for (auto &dep : archetypesDependency)
			{
				archetypeManager->loadOne(dep);
			}
		}

		entity->getLink().setPosition(entity->getLink().getPosition());
		entity->getLink().setOrientation(entity->getLink().getOrientation());
		entity->getLink().setScale(entity->getLink().getScale());
		//entity.setFlags(f);
		for (auto &e : componentTypes)
		{
			auto hashType = (*typesMap)[e];
			auto newComponent = ComponentRegistrationManager::getInstance().loadBinary(hashType, entity, ar);
		}
		if (entity->haveComponent<ArchetypeComponent>())
		{
			auto archetypeName = entity->getComponent<ArchetypeComponent>()->archetypeName;
			archetypeManager->spawn(entity, archetypeName);
		}
	}
}