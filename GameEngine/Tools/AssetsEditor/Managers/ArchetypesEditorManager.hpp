#pragma once

#include <Utils/Dependency.hpp>
#include <Entities/Archetype.hpp>
#include <set>
#include <map>
#include <Components/EntityRepresentation.hpp>
#include <Entities/Entity.hh>
#include <Core/AScene.hh>
#include <EntityHelpers/EntityImgui.hpp>
#include <Scenes/ArchetypeScenes.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/ThreadManager.hpp>

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
				auto &archetype = representation->archetype;
				auto &destination = archetype.getEntity();


				//we copy the original entity into the archetype entity
				//for that we set the ArchetypeScene as the current scene
				//so that messages will be not passed to the renderScene of the WE's scene
				//but to the ArchetypeScene.
				GetMainThread()->setSceneAsActive(_archetypesScene.get());
				bool success = _archetypesScene->copyEntity(entity, destination, true, false);
				GetMainThread()->setSceneAsActive(scene);
				AGE_ASSERT(success);

				destination.getComponent<EntityRepresentation>()->editorOnly = true;

				auto componentsCopy = entity.getComponentList();

				auto entityRepresentationComponent = entity.getComponent<EntityRepresentation>();
				AGE_ASSERT(entityRepresentationComponent != nullptr);

				//we remove original entity's components
				//replacing them by archetypes ones
				for (auto &e : componentsCopy)
				{
					if (e != nullptr && e != entityRepresentationComponent)
					{
						entity.copyComponent(destination.getComponent(e->getType()));
					}
				}

				entityRepresentationComponent->archetypeLinked = representation;

				representation->entities.insert(entity);

				_archetypesCollection.insert(std::make_pair(name, representation));
				_regenerateImGuiNamesList();
			}

			void update(AScene *scene)
			{
				if (_archetypesScene == nullptr)
				{
					_archetypesScene = std::make_shared<ArchetypeScene>(GetEngine());
					GetEngine()->addScene(_archetypesScene, "ARCHETYPES_SCENE");
					GetEngine()->initScene("ARCHETYPES_SCENE");
					GetEngine()->enableScene("ARCHETYPES_SCENE", 0);
				}

				ImGui::SameLine();
				ImGui::BeginChild("Archetypes", ImVec2(ImGui::GetWindowWidth() * 0.35f, 0));

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
					ImGui::Checkbox("Graphnode display", &_graphNodeDisplay);
					
					auto entity = _selectedArchetype->archetype.getEntity();
					auto modified = true;
					
					GetMainThread()->setSceneAsActive(_archetypesScene.get());
					if (_graphNodeDisplay)
					{
						modified &= recursiveDisplayList(entity, entity.getPtr(), _selectParent);
					}
					else
					{
						modified &= displayEntity(entity, scene);
					}
					GetMainThread()->setSceneAsActive(scene);

					if (modified)
					{
						auto &componentList = _selectedArchetype->archetype.getEntity().getComponentList();

						for (auto e : _selectedArchetype->entities)
						{
							auto representation = e.getComponent<EntityRepresentation>();
							auto &copyComponentList = e.getComponentList();

							// we delete all existing components
							for (auto c : copyComponentList)
							{
								if (c != nullptr && c != representation)
								{
									e.removeComponent(c->getType());
								}
							}

							// and replace them by fresh copy
							representation = _selectedArchetype->archetype.getEntity().getComponent<EntityRepresentation>();
							for (auto c : componentList)
							{
								if (c != nullptr && c != representation)
								{
									e.copyComponent(c);
								}
							}
						}
					}
				}

				ImGui::EndChild();
			}
		private:
			std::map<std::string, std::shared_ptr<ArchetypeEditorRepresentation>> _archetypesCollection;
			std::vector<const char*> _archetypesImGuiNamesList;
			int _selectedArchetypeIndex = 0;
			std::shared_ptr<ArchetypeEditorRepresentation> _selectedArchetype = nullptr;
			bool _graphNodeDisplay = false;
			bool _selectParent = false;
			std::shared_ptr<AScene> _archetypesScene = nullptr;

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