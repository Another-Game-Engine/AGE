#pragma once

#include <Utils/Dependency.hpp>
#include <set>
#include <map>
#include <Entities/Archetype.hpp>
#include <Entities/ReadableEntityPack.hpp>

//#include <Components/EntityRepresentation.hpp>
//#include <Entities/Entity.hh>
//#include <Core/AScene.hh>
//#include <EntityHelpers/EntityImgui.hpp>
//#include <Scenes/ArchetypeScenes.hpp>
//#include <Threads/MainThread.hpp>
//#include <Threads/ThreadManager.hpp>
//#include <Entities/ArchetypeLibrary.hpp>
//#include <Components/ArchetypeComponent.hpp>

namespace AGE
{
	namespace WE
	{
		struct ArchetypeEditorRepresentation
		{
			std::set<Entity> entities;
			ReadableEntityPack collection;
			Archetype archetype;
		};

		class ArchetypesEditorManager : public Dependency < ArchetypesEditorManager >
		{
		public:
			ArchetypesEditorManager();
			virtual ~ArchetypesEditorManager();
			void load();
			void transformToArchetype(Entity &entity, const std::string &name);
			void update(AScene *scene);
		private:
			std::map<std::string, std::shared_ptr<ArchetypeEditorRepresentation>> _archetypesCollection;
			std::vector<const char*> _archetypesImGuiNamesList;
			int _selectedArchetypeIndex = 0;
			std::shared_ptr<ArchetypeEditorRepresentation> _selectedArchetype = nullptr;
			int _selectedEntityIndex = 0;
			std::vector<const char*> _entitiesNames;
			Entity *_selectedEntity = nullptr;
			bool _graphNodeDisplay = false;
			bool _selectParent = false;
			std::shared_ptr<AScene> _archetypesScene;

			void _copyArchetypeToInstanciedEntity(Entity &archetype, Entity &entity);
			void _regenerateImGuiNamesList();
			
			std::shared_ptr<AScene> getScene();
			void enableScene();
			void disableScene();
		};
	}
}