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
			EntityManager::EntityManager(std::weak_ptr<AScene> &&scene)
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
				float t = static_cast<float>(time);
				auto scene = _scene.lock();
				EntityFilter::Lock lock(_filter);

				ImGui::ListBox("Entities list", &_selectedEntity, &(_entityNames.front()), (int)(_entityNames.size()), 4);

				for (auto e : _filter.getCollection())
				{
					auto cpt = e.getComponent<AGE::WE::EntityRepresentation>();
					if (ImGui::TreeNode((void*)(cpt), "%s", cpt->name))
					{
						ImGui::InputText("Name", cpt->name, ENTITY_NAME_LENGTH);
						if (ImGui::SliderFloat3("Position", glm::value_ptr(cpt->position), -1000000, 1000000))
						{
							e.getLink().setPosition(cpt->position);
						}
						if (ImGui::SliderFloat3("Rotation", glm::value_ptr(cpt->rotation), -360, 360))
						{
							// temporary TODO
							//cpt->rotation.x = std::fmod(cpt->rotation.x, 360); cpt->rotation.y = std::fmod(cpt->rotation.y, 360); cpt->rotation.z = std::fmod(cpt->rotation.z, 360);
							e.getLink().setOrientation(glm::quat(cpt->rotation));
						}
						if (ImGui::SliderFloat3("Scale", glm::value_ptr(cpt->scale), 0.0f, 1000000))
						{
							e.getLink().setScale(cpt->scale);
						}

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
										ptr->editorUpdate(scene.get());
										if (ptr->deletableInEditor)
										{
											ImGui::PushID(i);
											if (ImGui::Button("Delete"))
											{
												ptr->editorDelete(scene.get());
												e.removeComponent(i);
											}
											ImGui::PopID();
										}
										ImGui::TreePop();
									}
								}
							}
						}

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

						ImGui::TreePop();
					}
					//ImGui::TreeNode(e)
					//ImGui::TreeNode()
					//ImGui::TreePop()
				}
				if (ImGui::Button("Save scene"))
				{
					scene->saveToJson("WorldEditorSceneTest.json");
				}
				if (ImGui::Button("Load scene"))
				{
					scene->loadFromJson("WorldEditorSceneTest.json");
				}
			}

			bool EntityManager::initialize()
			{
				_filter.setOnAdd(std::function<void(Entity e)>([this](Entity en){
					_entityNames.clear();
					_entities.clear();
					en.addComponent<AGE::WE::EntityRepresentation>(std::string("Entity " + std::to_string(en.getId()) + "\0").c_str());

					for (auto e : _filter.getCollection())
					{
						_entityNames.push_back(e.getComponent<AGE::WE::EntityRepresentation>()->name);
						_entities.push_back(e);
					}
				}));

				_filter.setOnRemove(std::function<void(Entity e)>([this](Entity en){
					_entityNames.clear();
					_entities.clear();
					for (auto e : _filter.getCollection())
					{
						_entityNames.push_back(e.getComponent<AGE::WE::EntityRepresentation>()->name);
						_entities.push_back(e);
					}
				}));

				for (auto i = 0; i < 2; ++i)
				{
					auto e = _scene.lock()->createEntity();
					e.addComponent<PointLightComponent>()->set(glm::vec3((float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f), glm::vec3(1.f, 0.1f, 0.0f));
				}
				return true;
			}
	}
}