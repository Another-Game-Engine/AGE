#include "EntityReadablePacker.hpp"
#include <Entities/EntityData.hh>
#include <Entities/Entity.hh>
#include <Entities/EntityPacker.hpp>
#include <Entities/ReadableEntityPack.hpp>
#include <Entities/ReadableEntity.hpp>

#include <Components/ComponentRegistrationManager.hpp>
#include <Components/ArchetypeComponent.hpp>
#include <Components/EntityRepresentation.hpp>

#include <WorldEditorGlobal.hpp>

namespace AGE
{
	void CreateReadableEntityPack(ReadableEntityPack &pack, Entity &selection)
	{
		std::vector<Entity> c;
		c.push_back(selection);
		CreateReadableEntityPack(pack, c);
	}

	void CreateReadableEntityPack(ReadableEntityPack &pack, std::vector<Entity> &selection)
	{
		SelectionToFlatVector<ReadableEntity>(selection, pack.entities);

		auto &typesMap = ComponentRegistrationManager::getInstance().getAgeIdToSystemIdMap();

		pack.componentsIdReferenceTable = typesMap;

		for (auto &representation : pack.entities)
		{
			auto &entity = representation.entity;
			auto &components = entity.getComponentList();

			bool continueEntityFiltering = true;

			// If entity is an Archetype
			// We save only :
			// - The archetype component
			// - The entity representation (used in editor)
			if (entity.haveComponent<ArchetypeComponent>() && continueEntityFiltering)
			{
				auto archetypeCpt = entity.getComponent<ArchetypeComponent>();
				representation.componentTypes.push_back(archetypeCpt->getType());
				representation.components.push_back(archetypeCpt);
				representation.archetypesDependency.push_back(archetypeCpt->archetypeName);

				auto representationCpt = entity.getComponent<WE::EntityRepresentation>();
				AGE_ASSERT(representationCpt != nullptr);

				representation.componentTypes.push_back(representationCpt->getType());
				representation.components.push_back(representationCpt);

				continueEntityFiltering = false;
			}

			if (continueEntityFiltering)
			{
				for (auto &c : components)
				{
					if (c)
					{
						if (
							WESerialization::SerializeForEditor() == false
#ifdef EDITOR_ENABLED
							&& !c->serializeInExport()
#endif
							)
						{
							continue;
						}
						representation.componentTypes.push_back(c->getType());
						representation.components.push_back(c);
					}
				}
			}
		}
	}
}