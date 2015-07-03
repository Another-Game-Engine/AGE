#include "EntityBinaryPacker.hpp"
#include <Utils/Profiler.hpp>
#include <Entities/EntityData.hh>
#include <Entities/Entity.hh>
#include <Entities/EntityPacker.hpp>
#include <Entities/BinaryEntityPack.hpp>
#include <Entities/BinaryEntity.hpp>

#include <Components/ComponentRegistrationManager.hpp>
#include <Components/ArchetypeComponent.hpp>

#include <WorldEditorGlobal.hpp>

namespace AGE
{
	void CreateBinaryEntityPack(BinaryEntityPack &pack, Entity &selection)
	{
		std::vector<Entity> c;
		c.push_back(selection);
		CreateBinaryEntityPack(pack, c);
	}

	void CreateBinaryEntityPack(BinaryEntityPack &pack, std::vector<Entity> &selection)
	{
		SCOPE_profile_cpu_function("Entity packer");

		SelectionToFlatVector<BinaryEntity>(selection, pack.entities);

		auto &typesMap = ComponentRegistrationManager::getInstance().getAgeIdToSystemIdMap();

		pack.componentsIdReferenceTable = typesMap;

		for (auto &representation : pack.entities)
		{
			auto &entity = representation.entity;
			auto &components = entity->getComponentList();

			representation.entity = entity;

			bool continueEntityFiltering = true;

			// If entity is an Archetype
			// We save only :
			// - The archetype component
			if (entity->haveComponent<ArchetypeComponent>() && continueEntityFiltering)
			{
				auto archetypeCpt = entity->getComponent<ArchetypeComponent>();
				representation.componentTypes.push_back(archetypeCpt->getType());
				representation.components.push_back(archetypeCpt);
				representation.archetypesDependency.push_back(archetypeCpt->archetypeName);

				continueEntityFiltering = false;
			}

			if (continueEntityFiltering)
			{
				for (auto &c : components)
				{
					if (c)
					{
						representation.componentTypes.push_back(c->getType());
						representation.components.push_back(c);
					}
				}
			}
		}
	}
}