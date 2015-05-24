#pragma once

#include <vector>

namespace AGE
{
	class Entity;
	struct BinaryEntityPack;

	//struct ReadableEntity
	//{
	//	std::vector <std::size_t> children;
	//	std::vector <ComponentBase*> components;
	//	std::vector <ComponentType> componentTypes;
	//	std::vector<std::string> archetypesDependency;
	//	Entity entity;
	//	std::map<ComponentType, std::size_t> *typesMap; // used to unserialize
	//};

	class BinaryEntity;

	//struct ReadableEntityPack
	//{
	//	typedef std::map<ComponentType, std::size_t> CptIdsRefTable;
	//	CptIdsRefTable componentsIdReferenceTable;
	//	std::vector<ReadableEntity> entities;
	//	AScene *scene;
	//};


	static void CreateBinaryEntityPack(BinaryEntityPack &pack, std::vector<Entity> &selection);


	//static void CreateReadableEntityPack(ReadableEntityPack &pack, std::vector<Entity> &selection)
	//{
	//	SCOPE_profile_cpu_function("Entity packer");

	//	SelectionToFlatVector(selection);

	//	auto &typesMap = ComponentRegistrationManager::getInstance().getAgeIdToSystemIdMap();

	//	pack.componentsIdReferenceTable = typesMap;

	//	for (auto &entity : selection)
	//	{
	//		auto &components = entity.getComponentList();
	//		pack.entities.resize(pack.entities.size() + 1);
	//		auto &representation = pack.entities.back();

	//		representation.entity = entity;

	//		bool continueEntityFiltering = true;

	//		// If entity is an Archetype
	//		// We save only :
	//		// - The archetype component
	//		// - The entity representation (used in editor)
	//		if (entity.haveComponent<ArchetypeComponent>() && continueEntityFiltering)
	//		{
	//			auto archetypeCpt = entity.getComponent<ArchetypeComponent>();
	//			representation.componentTypes.push_back(archetypeCpt->getType());
	//			representation.components.push_back(archetypeCpt);
	//			representation.archetypesDependency.push_back(archetypeCpt->archetypeName);

	//			auto representationCpt = entity.getComponent<WE::EntityRepresentation>();
	//			AGE_ASSERT(representationCpt != nullptr);

	//			representation.componentTypes.push_back(representationCpt->getType());
	//			representation.components.push_back(representationCpt);

	//			continueEntityFiltering = false;
	//		}

	//		if (continueEntityFiltering)
	//		{
	//			for (auto &c : components)
	//			{
	//				if (c)
	//				{
	//					if (WESerialization::SerializeForEditor() == false && !c->serializeInExport())
	//					{
	//						continue;
	//					}
	//					representation.componentTypes.push_back(c->getType());
	//					representation.components.push_back(c);
	//				}
	//			}
	//		}
	//	}
	//}
}