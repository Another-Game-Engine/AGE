#include "EntityManager.hpp"
#include <imgui/imgui.h>
#include <Components/EntityRepresentation.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Components/ComponentRegistrar.hpp>

//FOR TEST ! TO REMOVE
#include <Components/Light.hh>

namespace AGE
{
	namespace WE
	{
			EntityManager::EntityManager(AScene *scene)
				: System(std::move(scene))
				, _filter(std::move(scene))
				, _selectedEntity(0)
			{
				_name = "we_entity_manager";
			}
			EntityManager::~EntityManager(){}

			void EntityManager::updateBegin(double time)
			{}

			void EntityManager::updateEnd(double time)
			{}

			void EntityManager::mainUpdate(double time)
			{
				ImGui::BeginChild("Entity list", ImVec2(ImGui::GetWindowWidth() * 0.25f, 0));

				float t = static_cast<float>(time);
				auto scene = _scene;
				EntityFilter::Lock lock(_filter);

				// Disgusting but fuck it ! :)
				_entityNames.clear();
				_entities.clear();
				for (auto e : _filter.getCollection())
				{
					_entityNames.push_back(e.getComponent<AGE::WE::EntityRepresentation>()->name);
					_entities.push_back(e);
				}
				if (_selectedEntity >= _entities.size())
					_selectedEntity = 0;

				if (_entities.size() > 0)
				{
					ImGui::PushItemWidth(-1);
					//ImGui::ListBoxHeader("##empty");
					ImGui::ListBox("##empty", &_selectedEntity, &(_entityNames.front()), (int)(_entityNames.size()));
					//ImGui::ListBoxFooter();
					ImGui::PopItemWidth();

					ImGui::Separator();
				}

				ImGui::BeginChild("Edit entity");

				if (_entities.size() > 0 && _selectedEntity < _entities.size())
				{
					auto e = _entities[_selectedEntity];
					auto cpt = e.getComponent<AGE::WE::EntityRepresentation>();

					ImGui::InputText("Name", cpt->name, ENTITY_NAME_LENGTH);
					if (ImGui::InputFloat3("Position", glm::value_ptr(cpt->position)))
					{
						e.getLink().setPosition(cpt->position);
					}
					if (ImGui::InputFloat3("Rotation", glm::value_ptr(cpt->rotation)))
					{
						e.getLink().setOrientation(glm::quat(cpt->rotation));
					}
					if (ImGui::InputFloat3("Scale", glm::value_ptr(cpt->scale)))
					{
						e.getLink().setScale(cpt->scale);
					}

					ImGui::Separator();

					auto &components = e.getComponentList();
					for (ComponentType i = 0; i < components.size(); ++i)
					{
						if (e.haveComponent(i))
						{
							auto ptr = e.getComponent(i);
							if (ptr->exposedInEditor)
							{
								if (ImGui::TreeNode(ComponentRegistrar::getInstance().getComponentName(ptr->getType()).c_str()))
								{
									ptr->editorUpdate(scene);
									if (ptr->deletableInEditor)
									{
										ImGui::PushID(i);
										if (ImGui::Button("Delete"))
										{
											ptr->editorDelete(scene);
											e.removeComponent(i);
										}
										ImGui::PopID();
									}
									ImGui::TreePop();
								}
							}
						}
					}

					ImGui::Separator();

					auto &types = ComponentRegistrar::getInstance().getSystemIdToAgeIdMap();
					auto &creationFn = ComponentRegistrar::getInstance().getCreationFunctions();

					for (auto &t : types)
					{
						if (!e.haveComponent(t.second))
						{
							if (ImGui::Button(std::string("Add : " + ComponentRegistrar::getInstance().getComponentName(t.second)).c_str()))
							{
								creationFn.at(t.first)(&e);
							}
						}
					}

					ImGui::Separator();

					if (ImGui::Button("Delete entity"))
					{
						_scene->destroy(e);
					}
				}

				
				if (ImGui::Button("Create entity"))
				{
					_scene->createEntity();
				}

				if (ImGui::Button("Save scene"))
				{
					scene->saveToJson("WorldEditorSceneTest.json");
				}
				if (ImGui::Button("Load scene"))
				{
					scene->loadFromJson("WorldEditorSceneTest.json");
				}

				ImGui::EndChild();

				ImGui::EndChild(); // Entity List
			}

			bool EntityManager::initialize()
			{
				_filter.setOnAdd(std::function<void(Entity e)>([this](Entity en)
				{
					en.addComponent<AGE::WE::EntityRepresentation>(std::string("Entity " + std::to_string(en.getId()) + "\0").c_str());
				}));

				_filter.setOnRemove(std::function<void(Entity e)>([this](Entity en)
				{
				}));
				return true;
			}
	}
}