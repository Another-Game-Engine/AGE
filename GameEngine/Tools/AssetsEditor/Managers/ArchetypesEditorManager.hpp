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
					if (_selectedEntity == nullptr)
					{
						_selectedEntity = &_selectedArchetype->archetype.getEntity();
					}

					ImGui::Checkbox("Graphnode display", &_graphNodeDisplay);
					
					auto entity = _selectedArchetype->archetype.getEntity();
					auto modified = false;
					
					GetMainThread()->setSceneAsActive(_archetypesScene.get());
					if (_graphNodeDisplay)
					{
						modified |= recursiveDisplayList(entity, _selectedEntity, _selectParent, false);
						if (_selectedEntity)
						{
							modified |= displayEntity(*_selectedEntity, scene);
						}
					}
					else
					{
						_entitiesNames.clear();
						std::vector<Entity*> tmpEntitiesList;
						listEntityTree(_selectedArchetype->archetype.getEntity(), _entitiesNames, tmpEntitiesList);

						if (tmpEntitiesList.size() > 0)
						{

							if (_selectedEntityIndex > tmpEntitiesList.size())
							{
								_selectedEntityIndex = tmpEntitiesList.size() - 1;
							}

							ImGui::PushItemWidth(-1);
							//ImGui::ListBoxHeader("##empty");
							if (ImGui::ListBox("##empty", &_selectedEntityIndex, &(_entitiesNames.front()), (int)(_entitiesNames.size())))
							{
								if (tmpEntitiesList.size() > 0 && _selectedEntityIndex < tmpEntitiesList.size())
								{
									_selectedEntity = tmpEntitiesList[_selectedEntityIndex];
								}
								else
								{
									_selectedEntity = nullptr;
								}
							}
							//ImGui::ListBoxFooter();
							ImGui::PopItemWidth();
							if (_selectedEntity)
							{
								modified |= displayEntity(*_selectedEntity, scene);
							}
						}
					}
					GetMainThread()->setSceneAsActive(scene);

					if (modified)
					{
						for (auto e : _selectedArchetype->entities)
						{
							_copyArchetypeToInstanciedEntity(_selectedArchetype->archetype.getEntity(), e);
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
			int _selectedEntityIndex = 0;
			std::vector<const char*> _entitiesNames;
			Entity *_selectedEntity = nullptr;
			bool _graphNodeDisplay = false;
			bool _selectParent = false;
			std::shared_ptr<AScene> _archetypesScene = nullptr;

			void _copyArchetypeToInstanciedEntity(Entity &archetype, Entity &entity)
			{
				auto &componentList = archetype.getComponentList();

				auto representation = entity.getComponent<EntityRepresentation>();
				auto &copyComponentList = entity.getComponentList();

				// we delete all existing components
				for (auto c : copyComponentList)
				{
					if (c != nullptr && c != representation)
					{
						entity.removeComponent(c->getType());
					}
				}

				// and replace them by fresh copy
				representation = _selectedArchetype->archetype.getEntity().getComponent<EntityRepresentation>();
				for (auto c : componentList)
				{
					if (c != nullptr && c != representation)
					{
						entity.copyComponent(c);
					}
				}

				const auto &archetypesChild = archetype.getLink().getChildren();
				const auto &entityChild = entity.getLink().getChildren();

				AGE_ASSERT(archetypesChild.size() == entityChild.size());

				for (std::size_t i = 0; i < archetypesChild.size(); ++i)
				{
					_copyArchetypeToInstanciedEntity(archetypesChild[i]->getEntity()->getEntity(), entityChild[i]->getEntity()->getEntity());
				}
			}

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