#include "EntitySerializationInfos.hpp"
#include <Entities/ArchetypeLibrary.hpp>
#include <Components/ComponentRegistrationManager.hpp>
#include <Core/AScene.hh>

namespace AGE
{
	void EntitySerializationInfos::_postLoad(Link &link, ENTITY_FLAGS flags, cereal::JSONInputArchive &ar)
	{
		// we load archetypes dependency
		auto archetypeLibrary = entity.getScene()->getInstance<AGE::ArchetypeLibrary>();
		for (auto &dep : archetypesDependency)
		{
			archetypeLibrary->loadOne(dep);
		}

		entity.getLink().setPosition(link.getPosition());
		entity.getLink().setOrientation(link.getOrientation());
		entity.getLink().setScale(link.getScale());
		//entity.setFlags(f);
		for (auto &e : componentTypes)
		{
			auto hashType = (*typesMap)[e];
			ComponentRegistrationManager::getInstance().loadJson(hashType, entity, ar);
			if (e == Component<ArchetypeComponent>::getTypeId())
			{
				auto archetypeName = entity.getComponent<ArchetypeComponent>()->archetypeName;
				bool success = entity.getScene()->getInstance<AGE::ArchetypeLibrary>()->spawn(archetypeName, entity);
				AGE_ASSERT(success);
			}
		}
	}
}