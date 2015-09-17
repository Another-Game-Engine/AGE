#include "EntityReadablePacker.hpp"
#include <Entities/EntityData.hh>
#include <Entities/Entity.hh>
#include <Entities/ReadableEntityPack.hpp>
#include <Entities/ReadableEntity.hpp>
#include <Managers/ArchetypesEditorManager.hpp>

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
		std::vector<ReadableEntity> entities;
		SelectionToFlatReadableVector(selection, entities);

		auto &typesMap = ComponentRegistrationManager::getInstance().getAgeIdToSystemIdMap();

		pack.componentsIdReferenceTable = typesMap;

		for (auto &e : entities)
		{
			if (e.entity->haveComponent<ArchetypeComponent>()
				&& e.entity->getComponent<ArchetypeComponent>()->parentIsAnArchetype == true)
			{
				continue;
			}
			pack.entities.push_back(e);
			auto &representation = pack.entities.back();
			auto &entity = representation.entity;
			auto &components = entity->getComponentList();

			bool continueEntityFiltering = true;

			bool saveArchetypes = false;

			// If entity is an Archetype
			// We save only :
			// - The archetype component
			// - The entity representation (used in editor)
			if (entity->haveComponent<ArchetypeComponent>() && continueEntityFiltering)
			{
				saveArchetypes = true;

				auto archetypeCpt = entity->getComponent<ArchetypeComponent>();
				representation.componentTypes.push_back(archetypeCpt->getType());
				representation.components.push_back(archetypeCpt);
				representation.archetypesDependency.push_back(archetypeCpt->archetypeName);

				auto representationCpt = entity->getComponent<WE::EntityRepresentation>();
				AGE_ASSERT(representationCpt != nullptr);

				representation.componentTypes.push_back(representationCpt->getType());
				representation.components.push_back(representationCpt);

				continueEntityFiltering = false;
			}

			if (continueEntityFiltering)
			{
				for (auto &c : components)
				{
					if (c && c->doSerialize())
					{
						representation.componentTypes.push_back(c->getType());
						representation.components.push_back(c);
					}
				}
			}

			//if (saveArchetypes)
			//{
			//	auto archetypeManager = entity->getScene()->getInstance<AGE::WE::ArchetypeEditorManager>();
			//	archetypeManager->save();
			//}
		}
	}

	void EntityToFlatReadableVector(std::vector<ReadableEntity> &vector, const Entity &e)
	{
		SCOPE_profile_cpu_function("Entity packer");
		
		vector.push_back(ReadableEntity());
		vector.back().entity = e;
		auto &children = e->getLink().getChildren();
		auto parentId = vector.size() - 1;
		for (auto &c : children)
		{
			auto archetypeComponent = c->getEntity()->getEntity()->getComponent<ArchetypeComponent>();
			if (archetypeComponent && archetypeComponent->parentIsAnArchetype)
			{
				continue;
			}
			vector[parentId].children.push_back(vector.size());
			auto child = c->getEntity()->getEntity();
			EntityToFlatReadableVector(vector, child);
		}
	}

	void SelectionToFlatReadableVector(const std::vector<Entity> &selection, std::vector<ReadableEntity> &result)
	{
		SCOPE_profile_cpu_function("Entity packer");

		for (auto &e : selection)
		{
			EntityToFlatReadableVector(result, e);
		}
	}

}