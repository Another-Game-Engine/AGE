#pragma once

#include <Utils/Dependency.hpp>
#include <Entities/Archetype.hpp>
#include <set>
#include <map>
#include <Components/EntityRepresentation.hpp>
#include <Entities/Entity.hh>
#include <Core/AScene.hh>

namespace AGE
{
	namespace WE
	{
		struct ArchetypeEditorRepresentation
		{
			std::set<Entity> entities;
			Archetype archetype;
		};

		class ArchetypesEditorManager : public Dependency < ArchetypesEditorManager >
		{
		public:
			ArchetypesEditorManager()
			{

			}

			virtual ~ArchetypesEditorManager()
			{

			}

			void transformToArchetype(Entity &entity, const std::string &name)
			{
				if (_archetypesCollection.find(name) != std::end(_archetypesCollection))
				{
					return;
				}
				auto scene = entity.getScene();
				auto representation = std::make_shared<ArchetypeEditorRepresentation>();
				auto archetype = representation->archetype;
				auto &destination = archetype.getEntity();
				bool success = scene->copyEntity(entity, destination, true, true);
				AGE_ASSERT(success);
				auto componentsCopy = entity.getComponentList();

				auto entityRepresentationComponent = entity.getComponent<EntityRepresentation>();
				AGE_ASSERT(entityRepresentationComponent != nullptr);

				for (auto &e : componentsCopy)
				{
					if (e != nullptr && e != entityRepresentationComponent)
					{
						entity.removeComponent(e->getType());
					}
				}

				entityRepresentationComponent->archetype = representation;

				representation->entities.insert(entity);

				_archetypesCollection.insert(std::make_pair(name, representation));
				_regenerateImGuiNamesList();
			}

			void update()
			{
				ImGui::Begin("Archetypes");

				if (! _archetypesImGuiNamesList.empty() 
					&& ImGui::Combo("", &_selectedArchetypeIndex, &_archetypesImGuiNamesList[0], _archetypesImGuiNamesList.size()))
				{
					if (_selectedArchetypeIndex < _archetypesImGuiNamesList.size())
					{
						auto name = _archetypesImGuiNamesList[_selectedArchetypeIndex];
						auto find = _archetypesCollection.find(name);
						if (find == std::end(_archetypesCollection))
						{
							_selectedArchetype = nullptr;
						}
						else
						{
							_selectedArchetype = find->second;
						}
					}
				}

				if (_selectedArchetype != nullptr)
				{

				}

				ImGui::End();
			}
		private:
			std::map<std::string, std::shared_ptr<ArchetypeEditorRepresentation>> _archetypesCollection;
			std::vector<const char*> _archetypesImGuiNamesList;
			int _selectedArchetypeIndex = 0;
			std::shared_ptr<ArchetypeEditorRepresentation> _selectedArchetype = nullptr;

			void _regenerateImGuiNamesList()
			{
				_archetypesImGuiNamesList.clear();
				for (auto &e : _archetypesCollection)
				{
					_archetypesImGuiNamesList.push_back(e.first.c_str());
				}
			}

		};
	}
}